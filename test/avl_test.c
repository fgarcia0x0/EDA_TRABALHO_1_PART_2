#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "../include/avl_node.h"

#ifndef AVL_SMALL_CASE
	#define AVL_TEST_MAX_TREES (1000u)
	#define AVL_TEST_MAX_INSERTIONS (10000u)
	#define AVL_TEST_MAX_RANGE (100000u)
	#define AVL_TEST_MAX_REMOVES (1000u)
#else
	#define AVL_TEST_MAX_TREES (1u)
	#define AVL_TEST_MAX_INSERTIONS (100u)
	#define AVL_TEST_MAX_RANGE (1000u)
	#define AVL_TEST_MAX_REMOVES (10u)
#endif

int main()
{
	for (uint32_t i = 0; i < AVL_TEST_MAX_TREES; ++i)
	{
		int temp[AVL_TEST_MAX_INSERTIONS] = {0};
		int* p = temp;

		avl_node* root = NULL;
		srand(time(NULL));

		size_t node_cnt = 0;

		while (node_cnt < AVL_TEST_MAX_INSERTIONS)
		{
			avl_node_errcode errc = AVL_NODE_ERRC_SUCCESS;
			int elem = rand() % AVL_TEST_MAX_RANGE;

			avl_node_insert(elem, &root, &errc);

			if (errc == AVL_NODE_ERRC_SUCCESS)
			{
				*p++ = elem;
				++node_cnt;
			}
		}

		size_t tree_node_cnt = avl_node_count(root);
		if (tree_node_cnt && tree_node_cnt == node_cnt)
		{
			printf("[+] AVL node count: %zu\n", tree_node_cnt);
		}
		else
		{
			fprintf(stderr, "[-] Erro: Invalid tree node count\n");
			exit(EXIT_FAILURE);
		}

		bool avl_valid = is_avl_tree(root);
		printf("[+] Valid AVL: %s\n", avl_valid ? "true" : "false");

		if (!avl_valid)
			exit(EXIT_FAILURE);

		// second step: remove nodes
		uint32_t mid = AVL_TEST_MAX_INSERTIONS / 2;
		assert((mid + AVL_TEST_MAX_REMOVES) < AVL_TEST_MAX_INSERTIONS);

		for (uint32_t i = 0; i < AVL_TEST_MAX_REMOVES; ++i)
		{
			avl_node_errcode errc = AVL_NODE_ERRC_SUCCESS;
			avl_node_remove(temp[mid + i], &root, &errc);

			if (errc != AVL_NODE_ERRC_SUCCESS)
			{
				fprintf(stderr, "[-] Error to remove avl tree node\n");
				exit(EXIT_FAILURE);
			}
		}

		size_t new_tree_node_cnt = avl_node_count(root);
		if (new_tree_node_cnt && new_tree_node_cnt == (AVL_TEST_MAX_INSERTIONS - AVL_TEST_MAX_REMOVES))
		{
			printf("[+] AVL node count (Pos-Remove): %zu\n", new_tree_node_cnt);
		}
		else
		{
			fprintf(stderr, "[-] Erro (Remove): Invalid tree node count\n");
			exit(EXIT_FAILURE);
		}

		avl_valid = is_avl_tree(root);
		printf("[+] Valid AVL (Pos-Remove): %s\n", avl_valid ? "true" : "false");

		if (!avl_valid)
			exit(EXIT_FAILURE);

		avl_node_release(&root);
		putchar('\n');
	}

	printf("[+] Successfully\n");
}
