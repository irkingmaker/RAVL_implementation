# RAVL_implementation
Implementation of relaxed AVL tree data structure-

Implementation is based on "Deletion without rebalancing in binary search trees" 
by Siddhartha Sen et al

ravl.c - C implementation of the paper.

duplicate_ravl.c - C implementation of ravl with handling of duplicates.

firstfit_ravl.c - C implementation of ravl with deleting a node based on first fit algorithm.

best_fit_ravl.c - C implementation of ravl with deleting a node based on best fit algorithm.

fragmentation_firstfit_ravl.c - C implementation of ravl with first fit and fragmentation.

fragmentation_bestfit_ravl.c - C implementation of ravl with best fit and fragmentation.

Every file should be given input of a csv file of insertions and deletions.

Every file outputs the preorder of the final tree and parameters on which performance
of the tree can be compared like - Average length, Maximum length, Number of nodes, 
Number of rotations, Balance updates.
