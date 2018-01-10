// File: $Id: bst.h,v 1.3 2014/02/27 20:46:28 csci243 Exp $
//
// Binary Search Tree definitions
//
// Author: rwd, sps

#ifndef BST_H
#define BST_H

/// The definition of the tree structure
typedef struct TreeNode {
    int data ;                  // the data stored in the node
    struct TreeNode* left ;     // node's left child
    struct TreeNode* right ;    // node's right child
} TreeNode;

/// The three supported traversals
typedef enum {
    PREORDER,           // parent -> left -> right
    INORDER,            // left -> parent -> right
    POSTORDER           // left -> right -> parent
} TraversalType;

// FUNCTIONS STUDENTS ARE REQUIRED TO IMPLEMENT

/// build_tree() 
///     Dynamically build BST by allocating nodes on the heap
///             Note that the BST will support duplicate values by
///             placing them into the left child's subtree
///
/// args -
///        root - a pointer to the pointer to the root of the tree
///               to build this tree on to.
///        elements - array of integers to add to the tree
///        count - the number of integers in the array
void build_tree(TreeNode** root, const int elements[], const int count);

/// traverse()
///    Recursively traverses the tree and prints the value of each
///    node.
///
/// args -
///        root - a pointer to the root of the tree to traverse
///        type - the traversal type
void traverse(const TreeNode* root, const TraversalType type);

/// cleanup_tree()
///    Cleanup all memory management associated with the nodes on the heap
///
/// args
///      root - the current root of the tree
void cleanup_tree(TreeNode* root);

#endif // BST_H

// Revisions: $Log: bst.h,v $
// Revisions: Revision 1.3  2014/02/27 20:46:28  csci243
// Revisions: suppost duplication in BST (dup to left)
// Revisions:
// Revisions: Revision 1.2  2013/10/03 12:49:03  csci243
// Revisions: added docstrings
// Revisions:
// Revisions: Revision 1.1  2013/10/03 02:06:55  csci243
// Revisions: Initial revision
// Revisions:
