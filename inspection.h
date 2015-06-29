#ifndef INSPECTION_H
#define INSPECTION_H

#include "mpc.h"
int getMaxChildren(mpc_ast_t *tree);
int getNumBranches(mpc_ast_t *tree);
int getNumLeaves(mpc_ast_t *tree);

#endif
