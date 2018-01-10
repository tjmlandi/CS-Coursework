#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bst.h"

int str_to_int(char* str);
size_t strlen(const char *str);
int isdigit(int c);

int main(int argc, char* argv[])
{
	//If there are no arguments, exit 
	if (argc <= 1)
	{
		printf("Usage: bst #\n");
		return 1;
	}
	int count = str_to_int(argv[1]);
	//If the count is 0 or less, exit
	if (count < 1)
	{
		printf("# must be greater than 0\n");
		return 1;
	}
	//Take input and place it in the array of integers, elements
	printf("Enter %d integer values to place in tree:\n", count);
	int elements[count];
	for (int i = 0; i < count; ++i)
	{
		char buf[256];
		fgets(buf, sizeof(buf), stdin);
		elements[i] = str_to_int(buf);
	}
	//Print out the input
	printf("Input values:\n");
	for (int i = 0; i < count; ++i)
	{
		printf("%d\n", elements[i]);
	}
	//Build, traverse, and clear the tree
	TreeNode** root = malloc(sizeof(TreeNode*));
	build_tree(root, elements, count);
	printf("Preorder:\n");
	traverse(*root, PREORDER);
	printf("Inorder:\n");
	traverse(*root, INORDER);
	printf("Postorder:\n");
	traverse(*root, POSTORDER);
	cleanup_tree(*root);
	free(root);
	return 0;

}


//Converts string parameter into returned int
int str_to_int(char* str)
{
	int value = 0;
	for (int i = 0; i < (signed int)strlen(str); ++i)
	{
		if (isdigit(str[i]))
		{
			value = (value * 10) + (str[i] - '0');
		}
		else
		{
			break;
		}
	}
	return value;
}


//Builds a binary search tree using the root pointer pointer paramter, an array of elements to 
//fill it, and the count of the elements
void build_tree(TreeNode** root, const int elements[], const int count)
{
	//Initializes the root of the tree
	*root = malloc(sizeof(TreeNode));
	TreeNode* prev = *root;
	TreeNode* current = *root;
	bool right = true;
	current = *root;
	current->data = elements[0];
	current->left = NULL;
	current->right = NULL;

	//Loops through each integer in the array to be added
	for (int i = 1; i < count; ++i)
	{
		//Navigates to the place in the tree where the next ndoe would belong
		while (current != NULL)
		{
			if (elements[i] > current->data)
			{
				right = true;
				prev = current;
				current = current->right;
			}
			else 
			{
				right = false;
				prev = current;
				current = current->left;
			}
		}

		//Adds the new node to the tree
		if (right)
		{
			prev->right = malloc(sizeof(TreeNode));
			prev->right->data = elements[i];
			prev->right->right = NULL;
			prev->right->left = NULL;
		}
		else
		{
			prev->left = malloc(sizeof(TreeNode));
			prev->left->data = elements[i];
			prev->left->right = NULL;
			prev->left->right = NULL;
		}
		prev = *root;
		current = *root;
	}
}

//Prints out a traversal of the tree
void traverse(const TreeNode* root, const TraversalType type)
{
	//Recursively traverses the tree in preorder 
	if (type == PREORDER)
	{
		printf("%d\n", root->data);
		if (root->left != NULL)
		{
			traverse(root->left, PREORDER);
		}
		if (root->right != NULL)
		{
			traverse(root->right, PREORDER);
		}	
	}
	
	//Recursively traverses the tree in inorder
	else if (type == INORDER)
	{
		if (root->left != NULL)
		{
			traverse(root->left, INORDER);
		}		
		printf("%d\n", root->data);
		if (root->right != NULL)
		{
			traverse(root->right, INORDER);
		}
	}

	//Recursively traverses the tree in postorder
	else
	{
		if (root->left != NULL)
		{
			traverse(root->left, POSTORDER);
		}
		if (root->right != NULL)
		{
			traverse(root->right, POSTORDER);
		}
		printf("%d\n", root->data);
	}

}


//Frees all the memory, recursiveley, for the tree
void cleanup_tree(TreeNode* root)
{
	if (root == NULL)
	{
		return;
	}
	
	cleanup_tree(root->right);
	cleanup_tree(root->left);

	free(root);
}


