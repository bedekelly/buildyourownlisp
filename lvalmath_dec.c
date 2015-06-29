#include "lvalmath_dec.h"

lval *add_lvals(lval *left, lval *right)
{
  if (left->type == LVAL_DBL || right->type == LVAL_DBL)
    {
      lval *result = malloc(sizeof(lval));
      result->type = LVAL_DBL;
      result->dnum = left->dnum + right->dnum;
      del_lval(left);
      del_lval(right);
      return result;
    }
  else if (left->type == LVAL_INT && right->type == LVAL_INT)
    {
      lval *result = malloc(sizeof(lval));
      result->type = LVAL_INT;
      result->lnum = left->lnum + right->lnum;
      del_lval(left);
      del_lval(right);
      return result;
    }
  
  /* Something's gone badly wrong if we get here. */
  return (lval *)NULL;
}

lval *subtract_lvals(lval *left, lval *right) {return (lval *)NULL;}
lval *multiply_lvals(lval *left, lval *right) {return (lval *)NULL;}
lval *divide_lvals(lval *left, lval *right) {return (lval *)NULL;}
lval *modulo_lvals(lval *left, lval *right) {return (lval *)NULL;}
lval *pow_lvals(lval *left, lval *right) {return (lval *)NULL;}
lval *max_lvals(lval *left, lval *right) {return (lval *)NULL;}
lval *min_lvals(lval *left, lval *right) {return (lval *)NULL;}
