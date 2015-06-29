#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#ifdef _WIN32

#include <string.h>
#define BUFSIZE 2048

char *readline(const char *prompt)
{
  char buffer[BUFSIZE];
  fgets(prompt, stdout);
  fgets(buf, sizeof(buffer), stdin);

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


int main(int argc, char *argv[])
{
  mpc_parser_t *Main = mpc_new("main");
  
  mpca_lang(MPCA_LANG_DEFAULT,
	    "                                            \
              main : /.*p~it.*/  ;			 \
            ",
	    Main);

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

      printf("Your input was: %s\n", input);

      mpc_result_t r;
      if (mpc_parse("<stdin>", input, Main, &r))
	{
	  mpc_ast_print(r.output);
	  mpc_ast_delete(r.output);
	}
      else
	{
	  mpc_err_print(r.error);
	  mpc_err_print(r.error);
	}

      free(input);
    }
  mpc_cleanup(1, Main);
}
