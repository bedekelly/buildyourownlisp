#ifndef LVAL_H
#define LVAL_H

/* Enumerate the possible lvalue types. */
enum LVAL_TYPE
  {
    LVAL_SYM,
    LVAL_SEXPR,
    LVAL_INT,
    LVAL_DBL,
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
    long lnum;
    double dnum;
    struct sexpr_t *sexpr;
  };
} lval;


typedef struct sexpr_t {
  int count;
  lval **cell;
} sexpr_t;


void del_lval(lval *v);

#endif
