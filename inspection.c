int getMaxChildren(mpc_ast_t *tree)
{
  int own_num = tree->children_num;
  if (own_num == 0)
    {
      return 0;
    }
  int max = own_num;
  for (int i=0; i<own_num; i++)
    {
      int c;
      if ((c = getMaxChildren(tree->children[i])) > max)
	max = c;
    }
  return max;
}


int getNumBranches(mpc_ast_t *tree)
{
  if (tree->children_num == 0)
    {
      // This is a leaf node.
      return 0;
    }

  // This is a branch node.
  int total = 1;
  for (int i=0; i<tree->children_num; i++)
    {
      total += getNumBranches(tree->children[i]);
    }
  return total;
}


int getNumLeaves(mpc_ast_t *tree)
{
  if (tree->children_num == 0)
    return 1;
  
  int total = 0;
  for (int i=0; i<tree->children_num; i++)
    {
      total += getNumLeaves(tree->children[i]);
    }
  return total;
}
