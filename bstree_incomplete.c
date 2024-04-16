#include "bstree.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 64 // Maximum file line size

/* START [_BSTNode] */
typedef struct _BSTNode
{
    void *info;
    struct _BSTNode *left;
    struct _BSTNode *right;
    struct _BSTNode *parent;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree
{
    BSTNode *root;
    P_tree_ele_print print_ele;
    P_tree_ele_cmp cmp_ele;
};
/* END [_BSTree] */

/*** BSTNode TAD private functions ***/
BSTNode *_bst_node_new()
{
    BSTNode *pn = NULL;

    pn = malloc(sizeof(BSTNode));
    if (!pn)
    {
        return NULL;
    }

    pn->left = pn->right = NULL;
    pn->parent = NULL;
    pn->info = NULL;
    return pn;
}

void _bst_node_free(BSTNode *pn)
{
    if (!pn)
        return;
    if (pn->info)
        free(pn->info);
    free(pn);
}

void _bst_node_free_rec(BSTNode *pn)
{
    if (!pn)
        return;

    _bst_node_free_rec(pn->left);
    _bst_node_free_rec(pn->right);
    _bst_node_free(pn);

    return;
}

int _bst_depth_rec(BSTNode *pn)
{
    int depthR, depthL;
    if (!pn)
        return 0;

    depthL = _bst_depth_rec(pn->left);
    depthR = _bst_depth_rec(pn->right);

    if (depthR > depthL)
    {
        return depthR + 1;
    }
    else
    {
        return depthL + 1;
    }
}

int _bst_size_rec(BSTNode *pn)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_size_rec(pn->left);
    count += _bst_size_rec(pn->right);

    return count + 1;
}

int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += print_ele(pf, pn->info);
    count += _bst_preOrder_rec(pn->left, pf, print_ele);
    count += _bst_preOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_inOrder_rec(pn->left, pf, print_ele);
    count += print_ele(pf, pn->info);
    count += _bst_inOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_postOrder_rec(pn->left, pf, print_ele);
    count += _bst_postOrder_rec(pn->right, pf, print_ele);
    count += print_ele(pf, pn->info);

    return count;
}

/*** BSTree TAD functions ***/
BSTree *tree_init(P_tree_ele_print print_ele, P_tree_ele_cmp cmp_ele)
{
    if (!print_ele || !cmp_ele)
        return NULL;

    BSTree *tree = malloc(sizeof(BSTree));
    if (!tree)
    {
        return NULL;
    }

    tree->root = NULL;
    tree->print_ele = print_ele;
    tree->cmp_ele = cmp_ele;

    return tree;
}

void tree_destroy(BSTree *tree)
{
    if (!tree)
        return;

    _bst_node_free_rec(tree->root);

    free(tree);
    return;
}

Bool tree_isEmpty(const BSTree *tree)
{
    if (!tree || !tree->root)
        return TRUE;
    return FALSE;
}

int tree_depth(const BSTree *tree)
{
    if (!tree)
        return -1;

    return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree *tree)
{
    if (!tree)
        return -1;

    return _bst_size_rec(tree->root);
}

int tree_preOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

void *tree_find_min(BSTree *tree)
{
    if (!tree)
        return NULL;

    BSTNode *nAux = tree->root;

    while (nAux->left != NULL)
        nAux = nAux->left;

    return nAux->info;
}

void *tree_find_max(BSTree *tree)
{
    if (!tree)
        return NULL;

    BSTNode *nAux = tree->root;
    while (nAux->right != NULL)
        nAux = nAux->right;

    return nAux->info;
}

Bool tree_contains(BSTree *tree, const void *elem)
{
    if (!tree || !elem)
        return FALSE;
    int result = 0;
    result = tree->cmp_ele(elem, tree->root->info);

    if (result == 0)
        return TRUE;
    if (result < 0)
        return tree_contains_rec(tree->root->left, elem);

    return tree_contains_rec(tree->root->right, elem);
}

Bool tree_contains_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f)
{
    int result = 0;
    result = f(elem, BSTNode->info);
    if (result == 0)
        return TRUE;
}

Status tree_remove(BSTree *tree, const void *elem)
{
    BSTNode *n = NULL;
    if (!tree || !elem)
    {
        return ERROR;
    }
    n = tree->root;
    if ((tree->cmp_ele(n->info, elem)) == 0)
    {
        if (!n->right && !n->left)
        {
            _bst_node_free(n);
            return OK;
        }
        else if (!n->right)
        {
            tree->root = n->left;
            _bst_node_free(n);
            return OK;
        }
        else if (!n->left)
        {
            tree->root = n->right;
            _bst_node_free(n);
            return OK;
        }
        else if (n->right && n->left)
        {
            zzz
        }
    }
}

BSTNode *tree_remove_rec(BSTNode *node, )