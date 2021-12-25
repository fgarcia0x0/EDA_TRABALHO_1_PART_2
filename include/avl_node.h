#ifndef AVL_NODE_H
#define AVL_NODE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct avl_node_struct
{
	int data;
	struct avl_node_struct* left;
	struct avl_node_struct* right;
	int8_t balance_factor;
} avl_node;

typedef enum avl_node_errcode_enum
{
	AVL_NODE_ERRC_SUCCESS = 0x10,
	AVL_NODE_ERRC_NOT_ENOUGH_MEM,
	AVL_NODE_ERRC_ALREADY_INSERT,
	AVL_NODE_ERRC_INVALID_REMOVE,
	AVL_NODE_ERRC_FAILED = AVL_NODE_ERRC_NOT_ENOUGH_MEM |
						   AVL_NODE_ERRC_ALREADY_INSERT |
						   AVL_NODE_ERRC_INVALID_REMOVE
} avl_node_errcode;

/**
 * @brief Insere um nó na árvore AVL com o elemento especificado
 *
 * @param item o elemento a ser inserido
 * @param root_pptr um ponteiro que aponta para um outro ponteiro que aponta para raiz da árvore
 * @param opt_errc_ptr um ponteiro para um código de erro para a AVL
 * @return true se a altura de alguma subárvore foi modificada, false caso contrário.
 */
bool avl_node_insert(int item, avl_node** root_pptr, avl_node_errcode* opt_errc_ptr);


/**
 * @brief Remove um nó na árvore AVL com o elemento especificado
 *
 * @param item O item a ser removido
 * @param root_pptr um ponteiro para a raiz da árvore
 * @param opt_errc_ptr um ponteiro para um código de erro para a AVL
 * @return true Se a altura de alguma subárvore foi modificada, false caso contrário.
 */
bool avl_node_remove(int item, avl_node** root_pptr, avl_node_errcode* opt_errc_ptr);

/**
 * @brief Calcula a altura do nó passado
 *
 * @param root_ptr o nó alvo
 * @return A altura do nó
 */
int avl_node_height(avl_node* root_ptr);

/**
 * @brief Verifica se a árvore passada é uma AVL
 *
 * @param root_ptr um ponteiro para a raiz da AVL
 * @return true Se a árvore é AVL
 */
bool is_avl_tree(avl_node* root_ptr);

/**
 * @brief Conta a quantidade de nós em uma árvore AVL
 *
 * @param root_ptr um ponteiro para a raiz da AVL
 * @return a quantidade de nós na árvore AVL
 */
size_t avl_node_count(avl_node* root_ptr);

/**
 * @brief Deleta toda a árvore da memória (libera todos os nós da árvore)
 *
 * @param root_pptr um ponteiro para a raiz da AVL
 */
void avl_node_release(avl_node** root_pptr);

#endif
