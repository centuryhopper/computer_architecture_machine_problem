#include <stdio.h>
#include <stdlib.h>
#include "cache.h"
#include "op.h"
#include "redblacktree.h"

// This implementation file assumes the Tree is never null
// but does not assume that the Tree's root node is never null

/*
Rules That Every Red-Black Tree Follows:
-Every node has a colour either red or black.
-The root of the tree is always black.
-There are no two adjacent red nodes (A red node cannot have a red parent or red child).
-Every path from a node (including root) to any of its descendants NULL nodes has the same number of black nodes.
-length of longest path is no more than twice the length of shortest path
*/

#pragma region left rotation diagram
/*
 *	Step1: left rotate operation of tree T at node x

ex:
                    16 <- (tree->root)
                 /       \
                15       18 <- (pivotNode)
                           \
                            25 <- (pivotNodeRightChild)
                              \
                              30

Becomes:            16
                 /      \
                15       25
                        /  \
                       18   30
 */
#pragma endregion

void _left_rotate(TreePtr tree, TreeNode pivotNode)
{
    TreeNode pivotNodeRightChild = pivotNode->right;
    pivotNode->right = pivotNodeRightChild->left;

    // check for null before rewiring the right child's right subtree's parent to the pivot node
    if (pivotNode->right != NULL)
        pivotNode->right->parent = pivotNode;

    // rewire the right child's parent to its grandparent
    pivotNodeRightChild->parent = pivotNode->parent;

    // if pivot node doesn't have a parent, then it will
    // be the root node after this left rotation
    if (pivotNode->parent == NULL)
        tree->root = pivotNodeRightChild;
    // checks whether the pivotNode is a left child or not
    else if (pivotNode == pivotNode->parent->left)
        pivotNode->parent->left = pivotNodeRightChild;
    // the pivotNode's parent new right child will be the pivot's right child
    else
        pivotNode->parent->right = pivotNodeRightChild;

    pivotNodeRightChild->left = pivotNode;
    pivotNode->parent = pivotNodeRightChild;
}

#pragma region right rotate diagram
/*
 *Step2: right rotate operation of tree T at node x

ex.
                     16 <- (tree->root)
                 /        \
                15         18
                             \
                              30  <- (pivotNode)
                             /
                            25 <- (pivotNodeLeftChild)

Becomes:            16

                 /       \
                15       18
                           \
                            25
                              \
                              30
 */

#pragma endregion

void _right_rotate(TreePtr tree, TreeNode pivotNode)
{
    TreeNode pivotNodeLeftChild = pivotNode->left;
    pivotNode->left = pivotNodeLeftChild->right;

    if (pivotNode->left != null)
    {
        pivotNode->left->parent = pivotNode;
    }

    pivotNodeLeftChild->parent = pivotNode->parent;

    if (pivotNode->parent == null)
    {
        tree->root = pivotNodeLeftChild;
    }
    else if (pivotNode == pivotNode->parent->left)
    {
        pivotNode->parent->left = pivotNodeLeftChild;
    }
    else
    {
        pivotNode->parent->right = pivotNodeLeftChild;
    }

    pivotNodeLeftChild->right = pivotNode;
    pivotNode->parent = pivotNodeLeftChild;
}

/*
 * STep3: fix red black tree when insert a node into tree
 */
void rb_tree_insert_fix(TreePtr T, TreeNode x)
{
    while (x != T->root && x->parent->color == red)
    {
        if (x->parent == x->parent->parent->left)
        {
            TreeNode y = x->parent->parent->right;
            if (y != NULL && y->color == red)
            {
                x->parent->color = black;
                y->color = black;
                x->parent->parent->color = red;
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->right)
                {
                    x = x->parent;
                    _left_rotate(T, x);
                }
                x->parent->color = black;
                x->parent->parent->color = red;
                _right_rotate(T, x->parent->parent);
            }
        }
        else
        {
            TreeNode y = x->parent->parent->left;
            if (y != NULL && y->color == red)
            {
                x->parent->color = black;
                y->color = black;
                x->parent->parent->color = red;
                x = x->parent->parent;
            }
            else
            {
                if (x == x->parent->left)
                {
                    x = x->parent;
                    _right_rotate(T, x);
                }
                x->parent->color = black;
                x->parent->parent->color = red;
                _left_rotate(T, x->parent->parent);
            }
        }
    }
    T->root->color = black;
}

/*
 *	Step4: insert a node into the tree
 */

TreeNode newTreeNode(uint64_t key, uint64_t value, Color color = black, TreeNode left = null, TreeNode right = null, TreeNode parent = null)
{
    TreeNode node = (TreeNode) malloc(sizeof(rbTreeNode));
    node->color = color;
    node->left = left;
    node->right = right;
    node->parent = parent;
    node->key = key;
    node->value = value;
    return node;
}

// will be a private method and should not be called by anyone other than _rb_tree_insert()
// assumes a non-empty tree
// root is root of rb-tree and key is the key to insert
TreeNode BST_insert(TreeNode root, TreeNode newNode)
{
    // base case
    if (root == null)
    {
        return newNode;
    }

    /* Otherwise, recur down the tree */
    if (newNode->key < root->key)
    {
        root->left = BST_insert(root->left, newNode);
        root->left->parent = root;
    }
    else if (newNode->key > root->key)
    {
        root->right = BST_insert(root->right, newNode);
        root->right->parent = root;
    }

    /* return the (unchanged) node pointer */
    return root;
}

int _rb_tree_insert(TreePtr tree, uint64_t key, uint64_t value)
{
    // if root is empty, create root, color it black and return 1 to indicate success.
    if (tree->root == null)
    {
        tree->root = newTreeNode(key, value);
        return 1;
    }

    // this will never be the root node (at least initially speaking)
    TreeNode newNode = newTreeNode(key, value, red, null, null, tree->root);

    tree->root = BST_insert(tree->root, newNode);

    rb_tree_insert_fix(tree, newNode);
    return 1;
}


TreeNode BST_search(TreeNode root, uint64_t key)
{
    // base case
    if (root == null)
        return null;
    // search right subtree
    if (key > root->key)
        return BST_search(root->right, key);
    // search left subtree
    if (key < root->key)
        return BST_search(root->left, key);
    // key found in current node
    return root;
}


/*
 *	Step5: find a value in tree T
 */
TreeNode rb_tree_search(TreePtr tree, uint64_t key)
{
    if (tree->root == null)
    {
        tree->root = newTreeNode(key,0,black);
    }

    return BST_search(tree->root, key);
}

/*
 *	Step6: free the space allocated for tree node
 */
void rb_tree_forest_fire(TreeNode root)
{
    // base case
    if (root == null) { return; }

    // have every node in the tree destroy its left and right subtrees
    // before destroying itself
    rb_tree_forest_fire(root->left);
    rb_tree_forest_fire(root->right);
    free(root);
}

/*
 *	STep7: free the space allocated for tree contaiing the TreeNode root
 */
void _rb_tree_clear(TreePtr tree)
{
    rb_tree_forest_fire(tree->root);
    free(tree);
}