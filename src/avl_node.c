#include <assert.h>
#include <stdio.h>

#include "../include/avl_node.h"
#include "../include/avl_utils.h"

bool avl_node_insert(int item, avl_node** root_pptr, avl_node_errcode* opt_errc_ptr)
{
	if (!root_pptr)
		return false;

	bool height_modified = false;
	avl_node* curr = *root_pptr;

	if (!curr)
	{
		avl_node* new_node = avl_node_make(item);

		if (!new_node)
		{
			if (opt_errc_ptr)
				*opt_errc_ptr = AVL_NODE_ERRC_NOT_ENOUGH_MEM;
			return height_modified;
		}

		*root_pptr = new_node;
		height_modified = true;
	}
	else
	{
		if (curr->data == item)
		{
			if (opt_errc_ptr)
				*opt_errc_ptr = AVL_NODE_ERRC_ALREADY_INSERT;
			return height_modified;
		}
		else if (item < curr->data)
		{
			height_modified = avl_node_insert(item, &(curr->left), opt_errc_ptr);
			avl_handle_subtree_growth(curr, &root_pptr, &height_modified, AVL_DIRECTION_LEFT);
		}
		else
		{
			height_modified = avl_node_insert(item, &(curr->right), opt_errc_ptr);
			avl_handle_subtree_growth(curr, &root_pptr, &height_modified, AVL_DIRECTION_RIGHT);
		}
	}

	return height_modified;
}

bool avl_node_remove(int item, avl_node** root_pptr, avl_node_errcode* opt_errc_ptr)
{
	bool height_modified = false;
	if (opt_errc_ptr)
		*opt_errc_ptr = AVL_NODE_ERRC_INVALID_REMOVE;

	if (root_pptr && *root_pptr)
	{
		if (opt_errc_ptr)
			*opt_errc_ptr = AVL_NODE_ERRC_SUCCESS;

		avl_node* curr = *root_pptr;

		if (item == curr->data)
		{
			if (!curr->left || !curr->right)
			{
				// Caso 1: O nó que queremos remover possui 0 ou 1 filho(s)
				*root_pptr = (curr->left) ? curr->left : curr->right;
				free(curr);
				height_modified = true;
			}
			else
			{
				// Caso 2: O nó que queremos remover possui 2 filhos

				// In-Order Sucessor
				avl_node* sucessor = curr->right;

				while (sucessor->left)
					sucessor = sucessor->left;

				// copia o valor do sucessor para o nó
				curr->data = sucessor->data;

				// remove o 'antigo' sucessor
				height_modified = avl_node_remove(sucessor->data, &(curr->right), opt_errc_ptr);

				// rebalanceamos a árvore
				avl_handle_subtree_shrink(curr, &root_pptr, &height_modified, AVL_DIRECTION_RIGHT);
			}
		}
		else if (item < curr->data)
		{
			height_modified = avl_node_remove(item, &(curr->left), opt_errc_ptr);
			avl_handle_subtree_shrink(curr, &root_pptr, &height_modified, AVL_DIRECTION_LEFT);
		}
		else
		{
			height_modified = avl_node_remove(item, &(curr->right), opt_errc_ptr);
			avl_handle_subtree_shrink(curr, &root_pptr, &height_modified, AVL_DIRECTION_RIGHT);
		}
	}

	return height_modified;
}

int avl_node_height(avl_node* root_ptr)
{
	if (!root_ptr)
		return -1;
	else
	{
		int hl = avl_node_height(root_ptr->left);
		int hr = avl_node_height(root_ptr->right);
		int max_height = (hr > hl) ? hr : hl;
		return max_height + 1;
	}
}

bool is_avl_tree(avl_node* root_ptr)
{
	// uma árvore vazia é AVL
	if (!root_ptr)
		return true;

	if (is_avl_tree(root_ptr->left) && is_avl_tree(root_ptr->right))
	{
		int hl = avl_node_height(root_ptr->left);
		int hr = avl_node_height(root_ptr->right);

		// fator de balanceamento calculado por meio da diferença das alturas
		int calc_bf = hr - hl;

		// fator de balanceamento associado ao nó
		int node_bf = root_ptr->balance_factor;

		bool b1 = !(node_bf < -1 || node_bf > 1);
		bool b2 = !(calc_bf < -1 || calc_bf > 1);

		return b1 && b2;
	}

	return false;
}

size_t avl_node_count(avl_node* root_ptr)
{
	if (!root_ptr)
		return 0u;
	else
		return 1u + avl_node_count(root_ptr->left)
				  + avl_node_count(root_ptr->right);
}

void avl_node_release(avl_node** root_pptr)
{
	if (root_pptr && *root_pptr)
	{
		avl_node* curr = *root_pptr;
		avl_node_release(&(curr->left));
		avl_node_release(&(curr->right));

		free(curr);
		*root_pptr = NULL;
	}
}
