#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpc.h"
#include "inspection.h"

#ifdef _WIN32

#include <string.h>
#define BUFSIZE 2048

char *readline(const char *prompt)
{
  char buffer[BUFSIZE];
  fgets(prompt, stdout);
  fgets(buf, sizeof(buffer), stdin);

  // Use strlen here to truncate the buffer.
  size_t bufsize = strlen(buffer);
  char *cpy = malloc(bufsize);
  memcpy(cpy, buffer, bufsize);
  cpy[bufsize-1] = '\0';
  return cpy;
}

void add_history(char *unused){}

#else

#include <editline/readline.h>
#include <editline/history.h>

#endif


/* Enumerate the possible lvalue types. */
enum LVAL_TYPE
  {
    LVAL_SYM,
    LVAL_SEXPR,
    LVAL_NUM,
    LVAL_ERR,
  };



/* Declare a Lisp Value type. */
struct sexpr_t;
typedef struct
{
  enum LVAL_TYPE type;
  union {
    char *error_msg;
    char *symbol;
    long num;
    struct sexpr_t *sexpr;
  };
} lval;


typedef struct sexpr_t {
  int count;
  lval **cell;
} sexpr_t;


void del_lval(lval *v)
{
  switch (v->type)
    {
    case LVAL_NUM:
      break;
    case LVAL_ERR:
      free(v->error_msg);
      break;
    case LVAL_SYM:
      free(v->symbol);
      break;
    case LVAL_SEXPR:
      for (int i=0; i < v->sexpr->count; i++)
	{
	  del_lval(v->sexpr->cell[i]);
	}
      free(v->sexpr->cell);
      free(v->sexpr);
      free(v);
      break;
    default:
      break;
    }
}


/* Define constructor functions for lvalue numbers and errors. */
lval *new_lval_num(long x)
{
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}


lval *new_lval_sym(char *sym)
{
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->symbol = malloc(strlen(sym) + 1);
  strcpy(v->symbol, sym);
  return v;
}


lval *new_lval_err(char *msg)
{
  lval *v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->error_msg = malloc(strlen(msg) + 1);
  strcpy(v->error_msg, msg);
  return v;
}


/* Define a generic function to print an lvalue. */
void _lval_print(lval *v)
{
  switch(v->type)
    {
    case LVAL_NUM:
      printf("%ld", v->num);
      break;
    case LVAL_ERR:
      printf("%s\n", v->error_msg);
      break;
    default:
      break;
    }
}


/* Wrapper function: uses the above, then prints a newline. */
void lval_print(lval *v)
{ 
  _lval_print(v);
  putchar('\n');
}


lval *eval_op(char *op, lval *left, lval *right)
{
  if (left->type == LVAL_ERR)
    return left;
  if (right->type == LVAL_ERR)
    return right;

  int x = left->num;
  int y = right->num;
  switch(*op){
  case '+': 
    return new_lval_num(x + y);
  case '-':
    return new_lval_num(x - y);
  case '*':
    return new_lval_num(x * y);
  case '/':
    {
      if (y == 0)
	return new_lval_err("Division by zero!");
      return new_lval_num(x / y);
    }
  case '%':
    return new_lval_num(x % y);
  case '^':
    return new_lval_num(pow(x, y));
  }
  
  if (!strcmp(op, "max"))
    return new_lval_num((x > y) ? x : y);

  if (!strcmp(op, "min"))
    return new_lval_num((x < y) ? x : y);

  return new_lval_err("Bad operator!");
}


/* Evaluate the expression from an abstract syntax tree. */
lval *eval_exp(mpc_ast_t *tree)
{
  // Base case: expression is a number so eval and return.
  if (strstr(tree->tag, "num"))
    return new_lval_num(atoi(tree->contents));

  // Tolerate a bracketless expression in the outermost nesting.
  if (strcmp(tree->tag, ">") == 0 && tree->children_num == 1) 
    return eval_exp(tree->children[0]);

  // Otherwise, it's an expression.
  // If the first token is a bracket, skip it.
  int i = 0;
  if (strcmp(tree->children[i]->contents, "(") == 0)
      i++;

  // Operator is always first in RPN, therefore the first child.
  char *op = tree->children[i]->contents;
  
  // Eval the first operand.
  i++;
  lval *result = eval_exp(tree->children[i]);

  // Move to the next operand and combine using operator.
  i++;
  while (i < tree->children_num
	 && strstr(tree->children[i]->tag, "expr"))
    {
      result = eval_op(op, result, eval_exp(tree->children[i]));
      i++;
    }
  return result;
}


int main(int argc, char *argv[])
{
  /* Create some parsers for mathematical expressions using RPN. */
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Symbol = mpc_new("symbol");
  mpc_parser_t *Sexpr = mpc_new("sexpr");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Line = mpc_new("line");

  /* Define them using this language specification. */
  mpca_lang(MPCA_LANG_DEFAULT,
	    "                                                    \
              number : /-?[0-9]+/ ;                              \
              symbol : '-' | '+' | '*' | '/' | '%' | '^'         \
                             | \"min\" | \"max\" ;		 \
              sexpr : '(' <expr>* ')' ;                          \
              expr : <number> | <symbol> | <sexpr> ;             \
              line : <expr>* ;                                   \
            ",
	    Number, Symbol, Sexpr, Expr, Line);

  puts("Lispy Version 0.0.1");
  puts("Press C-d to exit\n");

  /* Begin our REPL. */
  for (;;)
    {
      char *input = readline("clisp> ");
      if (input == NULL) {
	// Get rid of the visible "^D".
	printf("\b\b  \b\b\n");
	return 0;
      }
      add_history(input);

      mpc_result_t r;
      if (mpc_parse("<stdin>", input, Line, &r))
	{
	  mpc_ast_t *root = r.output;
	  lval_print(eval_exp(root));
	  mpc_ast_delete(r.output);
	}
      else
	{
	  mpc_err_print(r.error);
	  mpc_err_print(r.error);
	}

      free(input);
    }
  mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Line);
}
