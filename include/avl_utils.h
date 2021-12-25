#ifndef AVL_UTILS_H
#define AVL_UTILS_H

#include <stdlib.h>
#include <stdbool.h>

#include "../include/avl_node.h"

typedef enum avl_direction_enum
{
	AVL_DIRECTION_LEFT,
	AVL_DIRECTION_RIGHT
} avl_direction;

/**
 * @brief Aloca um novo nó com o valor passado na heap
 *
 * @param item o "valor" do nó
 * @return um novo nó alocado na heap com o novo valor contido,
 * caso haja uma falha de alocação retorna NULL.
 */
static inline avl_node* avl_node_make(int item)
{
	avl_node* new_node = (avl_node *) malloc(sizeof(avl_node));
	if (!new_node)
		return NULL;

	new_node->data = item;
	new_node->left = new_node->right = NULL;
	new_node->balance_factor = 0;

	return new_node;
}

/**
 * @brief Essa função é a versão extendida da função "avl_rotate_right",
 * esta lida com "casos especiais" em relação a remoção de um nó na AVL
 *
 * @param p raiz da subárvore AVL a ser rotacionada
 * @param height_modified_ptr um ponteiro para uma váriavel que verifica se a altura foi modificada
 * @return a nova raiz após a rotação
 */
static avl_node* avl_rotate_right_ex(avl_node* p, bool* height_modified_ptr)
{
	avl_node* u = p->left;

	if (u->balance_factor <= 0)
	{
		p->left = u->right;
		u->right = p;

		if (u->balance_factor == -1)
		{
			u->balance_factor = p->balance_factor = 0;
			*height_modified_ptr = true;
		}
		else
		{
			u->balance_factor = 1;
			p->balance_factor = -1;
			*height_modified_ptr = false;
		}
	}
	else if (u->balance_factor == 1)
	{
		avl_node* v = u->right;

		u->right = v->left;
		v->left = u;

		p->left = v->right;
		v->right = p;

		switch (v->balance_factor)
		{
			case -1:
				u->balance_factor = 0;
				p->balance_factor = 1;
				break;
			case 0:
				u->balance_factor = 0;
				p->balance_factor = 0;
				break;
			case 1:
				u->balance_factor = -1;
				p->balance_factor = 0;
				break;
		}

		v->balance_factor = 0;
		*height_modified_ptr = true;
		return v;
	}

	return u;
}

/**
 * @brief Essa função é a versão extendida da função "avl_rotate_left",
 * esta lida com "casos especiais" em relação a remoção de um nó na AVL
 *
 * @param p raiz da subárvore AVL a ser rotacionada
 * @param height_modified_ptr um ponteiro para uma váriavel que verifica se a altura foi modificada
 * @return a nova raiz após a rotação
 */
static avl_node* avl_rotate_left_ex(avl_node* p, bool* height_modified_ptr)
{
	avl_node* u = p->right;

	// LL Case
	if (u->balance_factor >= 0)
	{
		p->right = u->left;
		u->left = p;

		if (u->balance_factor == 1)
			u->balance_factor = p->balance_factor = 0;
		else
		{
			u->balance_factor = -1;
			p->balance_factor = 1;
		}
	}
	else // LR CASE
	{
		avl_node* v = u->left;

		u->left = v->right;
		v->right = u;

		p->right = v->left;
		v->left = p;

		switch (v->balance_factor)
		{
			case -1:
				p->balance_factor = 0;
				u->balance_factor = 1;
				break;
			case 0:
				p->balance_factor = 0;
				u->balance_factor = 0;
				break;
			case 1:
				p->balance_factor = -1;
				u->balance_factor = 0;
				break;
		}

		v->balance_factor = 0;
		*height_modified_ptr = true;
		return v;
	}

	return u;
}


/**
 * @brief Rotaciona a árvore/subárvore de raiz 'p' à direita (RR e RL)
 *
 * @param p raiz da subárvore AVL a ser rotacionada
 * @see Caso 1 (Inserir)(Jayme)
 * @return o endereço do nó que será a nova raiz da subárvore que originalmente era iniciada por p
 */
avl_node* avl_rotate_right(avl_node* p)
{
	avl_node* u = p->left;

	if (u->balance_factor == -1)
	{
		p->left = u->right;
		u->right = p;
		p->balance_factor = u->balance_factor = 0;
	}
	else if (u->balance_factor == 1)
	{
		avl_node* v = u->right;

		u->right = v->left;
		v->left = u;

		p->left = v->right;
		v->right = p;

		p->balance_factor = (v->balance_factor == -1) ? 1 : 0;
		u->balance_factor = (v->balance_factor == 1)  ? -1 : 0;

		v->balance_factor = 0;
		return v;
	}

	return u;
}

/**
 * @brief Rotaciona a árvore/subárvore de raiz 'p' à esquerda (LL e LR)
 *
 * @param p raiz da subárvore AVL a ser rotacionada
 * @see Caso 2 (Inserir)(Jayme)
 * @return o endereço do nó que será a nova raiz da subárvore que originalmente era iniciada por p
 */
avl_node* avl_rotate_left(avl_node* p)
{
	avl_node* u = p->right;

	// LL Case
	if (u->balance_factor == 1)
	{
		p->right = u->left;
		u->left = p;
		p->balance_factor = u->balance_factor = 0;
	}
	else if (u->balance_factor == -1) // LR Case
	{
		avl_node* v = u->left;

		u->left = v->right;
		v->right = u;

		p->right = v->left;
		v->left = p;

		p->balance_factor = (v->balance_factor == 1) ? -1 : 0;
		u->balance_factor = (v->balance_factor == -1) ? 1 : 0;

		v->balance_factor = 0;
		return v;
	}

	return u;
}

/**
 * @brief Essa função lida com o crescimento de uma subárvore devido a uma inserção.
 *
 * @param curr A subárvore que aumentou sua altura devido a uma inserção
 * @param root_ppptr um ponteiro de tripla indireção para a raiz da avl.
 * @param height_modified_ptr um ponteiro para uma váriavel que verifica se a altura foi modificada
 * @param direction A direção onde a inserção ocorrerá (left ou right)
 */
static void avl_handle_subtree_growth(avl_node* curr,
									  avl_node*** root_ppptr,
									  bool* height_modified_ptr,
									  avl_direction direction)
{
	if (direction == AVL_DIRECTION_LEFT)
	{
		if (*height_modified_ptr)
		{
			switch (curr->balance_factor)
			{
				case -1:
					**root_ppptr = avl_rotate_right(curr);
					*height_modified_ptr = false;
					break;
				case 0:
					curr->balance_factor = -1;
					break;
				case 1:
					curr->balance_factor = 0;
					*height_modified_ptr = false;
					break;
			}
		}
	}
	else
	{
		if (*height_modified_ptr)
		{
			switch (curr->balance_factor)
			{
				case -1:
					curr->balance_factor = 0;
					*height_modified_ptr = false;
					break;
				case 0:
					curr->balance_factor = 1;
					break;
				case 1:
					**root_ppptr = avl_rotate_left(curr);
					*height_modified_ptr = false;
					break;
			}
		}
	}
}

/**
 * @brief Essa função lida com o encolhimento de uma subárvore devido a uma remoção.
 *
 * @param curr A subárvore que encolheu sua altura devido a uma remoção
 * @param root_ppptr um ponteiro de tripla indireção para a raiz da avl.
 * @param height_modified_ptr um ponteiro para uma váriavel que verifica se a altura foi modificada
 * @param direction A direção onde a remoção ocorrerá (left ou right)
 */
static void avl_handle_subtree_shrink(avl_node* curr,
									  avl_node*** root_ppptr,
									  bool* height_modified_ptr,
									  avl_direction direction)
{
	if (direction == AVL_DIRECTION_LEFT)
	{
		if (*height_modified_ptr)
		{
			switch (curr->balance_factor)
			{
				case -1:
					curr->balance_factor = 0;
					break;
				case 0:
					curr->balance_factor = 1;
					*height_modified_ptr = false;
					break;
				case 1:
					**root_ppptr = avl_rotate_left_ex(curr, height_modified_ptr);
					if ((**root_ppptr)->balance_factor == -1)
						*height_modified_ptr = false;
					break;
				default:
					break;
			}
		}
	}
	else
	{
		if (*height_modified_ptr)
		{
			switch (curr->balance_factor)
			{
				case -1:
					**root_ppptr = avl_rotate_right_ex(curr, height_modified_ptr);
					if ((**root_ppptr)->balance_factor == 1)
						*height_modified_ptr = false;
					break;
				case 0:
					curr->balance_factor = -1;
					*height_modified_ptr = false;
					break;
				case 1:
					curr->balance_factor = 0;
					break;
				default:
					break;
			}
		}
	}
}

#endif
