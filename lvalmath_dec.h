#ifndef LVALMATH_H
#define LVALMATH_H

#include "lval.h"

lval *add_lvals(lval *left, lval *right);
lval *subtract_lvals(lval *left, lval *right);
lval *multiply_lvals(lval *left, lval *right);
lval *divide_lvals(lval *left, lval *right);
lval *modulo_lvals(lval *left, lval *right);
lval *pow_lvals(lval *left, lval *right);
lval *max_lvals(lval *left, lval *right);
lval *min_lvals(lval *left, lval *right);

#endif
