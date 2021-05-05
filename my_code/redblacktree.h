#pragma once
#ifndef REBTREE_H_
#define REBTREE_H_

typedef char Color;
#define red 0
#define black 1

#define null NULL

typedef struct rbTreeNode *TreeNode;
typedef struct rbTreeNode
{
	TreeNode parent;
	TreeNode left;
	TreeNode right;
	Color color;
	uint64_t key;
	uint64_t value;
} rbTreeNode;

typedef struct rbTree *TreePtr;
typedef struct rbTree
{
	TreeNode root;
} rbTree;

void _left_rotate(TreePtr T, TreeNode x);

void _right_rotate(TreePtr T, TreeNode x);

void rb_tree_insert_fix(TreePtr T, TreeNode _x);

int _rb_tree_insert(TreePtr T, uint64_t key, uint64_t value);

TreeNode rb_tree_search(TreePtr T, uint64_t key);

void rb_tree_forest_fire(TreeNode T);

void _rb_tree_clear(TreePtr T);

#endif