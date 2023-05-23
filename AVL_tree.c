/*
   AVL tree implementation.

   Author: A. Tafliovich
   Based heavily on materials developed by F. Estrada.
*/

#include "AVL_tree.h"
#include "math.h"
#include "stdlib.h"
/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/
 
 
//gcc -g -Wall -Werror AVL_tree.c -lm -o AVL_tree && AVL_tree
////////////////////////////////gcc AVL_tree_tester.c AVL_tree.c&&a.exe sample_input.txt/////////////////////////////

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.
 */
int max(int a, int b)
{
	if(b>a)
		return b;
	else 
		return a;
}
// it returns the height
int height(AVL_Node* node)
{
	if(node==NULL)
		return 0;
	return node->height;
}

/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void update_height(AVL_Node* node)
{
	int right_child_height = 0;
	int left_child_height = 0;
	if(node->right!=NULL)
		right_child_height = node->right->height;
	if(node->left!=NULL)
		left_child_height = node->left->height;
	node->height = max(left_child_height, right_child_height)+1;
}

/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 of node is NULL.
 */
 
int balance_factor(AVL_Node* node)
{
	
	return height(node->left)-height(node->right);
	//return node->left->height - node->right->height;
}

/* Returns the result of performing the corresponding rotation in the AVL
 * tree rooted at 'node'.
 */
// single rotations: right/clockwise
AVL_Node* right_rotation(AVL_Node* node)
{
	
	if(node==NULL)
	{
		return NULL;
	}
	
	AVL_Node* temp = NULL;
	temp = node->left;
	AVL_Node* right_of_target = temp->right;
	temp->right = node;
	node->left = right_of_target;
	update_height(temp);
	update_height(node);
	return temp;
}
// single rotations: left/counter-clockwise
AVL_Node* left_rotation(AVL_Node* node)
{
	if(node==NULL)
	{
		return NULL;
	}
	AVL_Node* temp = node->right;
	AVL_Node* left = temp->left;
	temp->left = node;
	node -> right = left;
	update_height(node);
	update_height(temp);
	return temp;
}
// double rotation: right/clockwise then left/counter-clockwise
AVL_Node* right_left_rotation(AVL_Node* node)
{
	if(node==NULL)
	{
		return NULL;
	}
	//AVL_Node* new = right_rotation(node->right);
	node->right = right_rotation(node->right);
	return left_rotation(node);
}
// double rotation: left/counter-clockwise then right/clockwise
AVL_Node* left_right_rotation(AVL_Node* node)
{
	if(node==NULL)
	{
		return NULL;
	}
	node->left = left_rotation(node->left);
	return right_rotation(node);
}

/* Returns the successor node of 'node'. */
AVL_Node* successor(AVL_Node* node)
{
	if(node == NULL)
	{
		return NULL;
	}
	AVL_Node* right = node->right;
	if(right==NULL)
	{
		return NULL;
	}
	while(right->left!=NULL)
		right = right->left;
	return right;
}

/* Creates and returns an AVL tree node with key 'key', value 'value', height
 * of 1, and left and right subtrees NULL.
 */
AVL_Node* create_node(int key, void* value)
{
	AVL_Node* new = (AVL_Node*)calloc(1,sizeof(AVL_Node));
	new->right=NULL;
	new->left=NULL;
	new->key = key;
	new->value = value;
	new->height = 1;
	return new;
}

/*************************************************************************
 ** Provided functions
 *************************************************************************/
void print_tree_inorder_(AVL_Node* node, int offset) {
  if (node == NULL) return;
  print_tree_inorder_(node->right, offset + 1);
  printf("%*s %d [%d]\n", offset, "", node->key, node->height);
  print_tree_inorder_(node->left, offset + 1);
}

void print_tree_inorder(AVL_Node* node) {
  print_tree_inorder_(node, 0);
}

void delete_tree(AVL_Node* node) {
  if (node == NULL) return;
  delete_tree(node->left);
  delete_tree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

AVL_Node* search(AVL_Node* node, int key) {
	if(node==NULL)
	{
		return NULL;
	}
	if(node->key==key)
	{
		return node;
	}
	if(node->key>key)
	{
		return search(node->left,key);
	}
	else if(node->key<key)
	{
		return search(node->right, key);
	}
	return node;
}

AVL_Node* insert(AVL_Node* node, int key, void* value) {
	AVL_Node* new_node = create_node(key, value);
	
	if(node==NULL)
		return new_node;
	if(key==node->key)
	{
		printf("Duplicate node\n");
		return node;
	}
	
	if(key<node->key)
	{
		
		node->left=insert(node->left, key,value);
		
	}
	
	else if(key>node->key)
	{
		node->right=insert(node->right, key,value);
	}
	update_height(node);

	//printf("******node->height\n",  node->height);
	if((height(node->left) - height(node->right)>1))
	{
		
		AVL_Node* x = node->left;
		if(height(x->left)>=height(x->right))
		{
			node = right_rotation(node);
		}
		else
		{
			node = left_right_rotation(node);
		}
	}
	else if((height(node->right)-height(node->left))>1)
	{
		
		AVL_Node* x = node->right;
		
		if(height(x->left)<=height(x->right))
		{
			
			node = left_rotation(node);
		}
		else
		{
			
			node = right_left_rotation(node);
		}
	}
	
	return node;
}

AVL_Node* delete(AVL_Node* node, int key) {
	AVL_Node* temp=NULL;
	if(node==NULL)
	{
		return NULL;
	}
	if(node->key==key)
	{
		if(node->left==NULL&&node->right==NULL)
		{
			free(node);
			return NULL;
		}
		if(node->left==NULL)
		{
			temp=node->right;
			free(node);
			return temp;
		}
		if(node->right==NULL)
		{
			temp = node->left;
			free(node);
			return temp;
		}
		if(node->left!=NULL&&node->right!=NULL)
		{
			temp=successor(node);
			node->key=temp->key;
			node->height=temp->height;
			node->value = temp->value;
			node->right=delete(node->right,temp->key);
			//return node;
		}
	}
	if(node->key>key)
	{
		node->left= delete(node->left,key);
		
	}
	else if(node->key<key)
	{
		node->right= delete(node->right,key);
	}
	update_height(node);
	if((height(node->left) - height(node->right)>1))
	{
		AVL_Node* x = node->left;
		if(height(x->left)>=height(x->right))
		{
			node = right_rotation(node);
		}
		else
		{
			node = left_right_rotation(node);
		}
	}
	else if((height(node->right)-height(node->left))>1)
	{
		
		AVL_Node* x = node->right;
		
		if(height(x->left)<=height(x->right))
		{
			
			node = left_rotation(node);
		}
		else
		{
			
			node = right_left_rotation(node);
		}
	}
	update_height(node);
	return node;
}
