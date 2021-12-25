#include <stdio.h>
#include "../include/avl_node.h"

static void inorder(avl_node* root_ptr)
{
	if (root_ptr)
	{
		inorder(root_ptr->left);
		printf("(elem: %d, bal: %d), ", root_ptr->data, root_ptr->balance_factor);
		inorder(root_ptr->right);
	}
}

int main()
{
	avl_node* root = NULL;

	avl_node_insert(16, &root, NULL);
	avl_node_insert(7, &root,  NULL);
	avl_node_insert(19, &root, NULL);
	avl_node_insert(6, &root,  NULL);
	avl_node_insert(12, &root, NULL);
	avl_node_insert(20, &root, NULL);
	avl_node_insert(10, &root, NULL);
	avl_node_insert(13, &root, NULL);
	avl_node_insert(9, &root,  NULL);
	avl_node_insert(8, &root,  NULL);

	printf("[+] In-Order: ");
	inorder(root);
	putchar('\n');

	avl_node_release(&root);
}
