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
    free(pn);
}
void _bst_node_free_2(BSTNode *pn)
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

void _bst_node_free_rec_2(BSTNode *pn)
{
    if (!pn)
        return;

    _bst_node_free_rec_2(pn->left);
    _bst_node_free_rec_2(pn->right);
    _bst_node_free_2(pn);

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

Bool tree_contains_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f);

BSTNode *tree_insert_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f);

void tree_print_level(BSTNode *node, P_tree_ele_print print_ele, int level, int numspaces, Bool left);

void *tree_find_min_rec(BSTNode *node);

void *tree_find_max_rec(BSTNode *node);

BSTNode *tree_remove_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f);

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

void tree_destroy_full(BSTree *tree)
{
    if (!tree)
        return;

    _bst_node_free_rec_2(tree->root);

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

    return tree_find_min_rec(tree->root);
}

void *tree_find_min_rec(BSTNode *node)
{
    void *info;
    if (!node)
    {
        return NULL;
    }

    if (node->left == NULL)
    {
        return node->info;
    }

    info = tree_find_min_rec(node->left);
    return info;
}

void *tree_find_max(BSTree *tree)
{
    if (!tree)
        return NULL;

    return tree_find_max_rec(tree->root);
}

void *tree_find_max_rec(BSTNode *node)
{
    void *info;
    if (!node)
    {
        return NULL;
    }

    if (node->right == NULL)
    {
        return node->info;
    }

    info = tree_find_max_rec(node->right);
    return info;
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
        return tree_contains_rec(tree->root->left, elem, tree->cmp_ele);

    return tree_contains_rec(tree->root->right, elem, tree->cmp_ele);
}

Bool tree_contains_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f)
{
    int result = 0;
    result = f(elem, node->info);
    if (result == 0)
        return TRUE;
    if (result < 0)
        return tree_contains_rec(node->left, elem, f);
    return tree_contains_rec(node->right, elem, f);
}

Status tree_insert(BSTree *tree, const void *elem)
{
    BSTNode *nAux = NULL;

    if (!tree || !elem)
        return ERROR;
    nAux = tree_insert_rec(tree->root, elem, tree->cmp_ele);
    if (nAux == NULL)
        return ERROR;
    tree->root = nAux;
    return OK;
}

BSTNode *tree_insert_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f)
{

    int cmp_result = 0;

    if (node == NULL)
    {
        node = _bst_node_new();
        if (node == NULL)
            return NULL;
        node->info = (void *)elem;
        return node;
    }

    cmp_result = f(elem, node->info);
    if (cmp_result < 0)
        node->left = tree_insert_rec(node->left, elem, f);
    else if (cmp_result > 0)
        node->right = tree_insert_rec(node->right, elem, f);
    return node;
}

void tree_print(BSTree *t)
{
    int i, depth;
    if (!t)
        return;
    if (!t->root){
        printf("\nTree is empty\n");
        return;
    }
    depth = tree_depth(t);
    for (i=0; i<depth; i++){
        
        if(i==0)
        printf("                                              ");
        else if (i == 1)
        printf("                    ");
        else if (i == 2)
        printf("       ");
        tree_print_level(t->root, t->print_ele, i, 41, FALSE);
        printf("\n");
    }
    
    return;
}

void tree_print_level(BSTNode *node, P_tree_ele_print print_ele, int level, int numspaces, Bool left){
    
    int i;
    if (node == NULL){
        if (left == TRUE){
            printf("           ");
            for (i=0; i<numspaces; i++)
            printf(" ");
            
            printf("           ");
        }
        return;
    }

    if (level == 0){
        print_ele(stdout,node->info);
    }
    else if (level == 1){
        if (node->left)
            print_ele(stdout, node->left->info);
        else 
            printf("           ");
        
        for (i=0; i<numspaces; i++)
            printf(" ");

        if (node->right)
            print_ele(stdout, node->right->info);
        else 
            printf("           ");
    }
    else if (level == 2){
        tree_print_level(node->left, print_ele, level -1, 15, TRUE);
        for (i=0; i<15; i++)
            printf(" ");
        tree_print_level(node->right, print_ele, level -1, 15, FALSE);
    }
    else if (level == 3){
        tree_print_level(node->left->left, print_ele, 1, 2, TRUE);
        for (i=0; i<2; i++)
            printf(" ");
        tree_print_level(node->left->right, print_ele, 1, 2, TRUE);
        for (i=0; i<2; i++)
            printf(" ");
        tree_print_level(node->right->left, print_ele, 1, 2, TRUE);
        for (i=0; i<2; i++)
            printf(" ");
        tree_print_level(node->right->right, print_ele, 1, 2, FALSE);
    }
    return;
}

Status tree_remove(BSTree *tree, const void *elem)
{
    if (!tree || !elem)
    {
        return ERROR;
    }

    tree->root = tree_remove_rec(tree->root, elem, tree->cmp_ele);
    return OK;
}

BSTNode *tree_remove_rec(BSTNode *node, const void *elem, P_tree_ele_cmp f)
{
    int cmp;
    void *aux, *deleter;
    BSTNode *node_aux = NULL;
    if (!node || !elem)
    {
        return NULL;
    }
    cmp = f(elem, node->info);
    if (cmp < 0)
    {   
        node->left = tree_remove_rec(node->left, elem, f);
    }
    else if (cmp > 0)
    {
        node->right = tree_remove_rec(node->right, elem, f);
    }
    else if (cmp == 0)
    {
        if (!node->right && !node->left)
        {
            _bst_node_free(node);
            return NULL;
        }
        else if (node->right && node->left)
        {
            aux = tree_find_min_rec(node->right);
            deleter = node->info;
            free(deleter);
            node->info = aux;
            node->right = tree_remove_rec(node->right, aux, f);
            return node;
        }
        else if (node->right)
        {
            node_aux = node->right;
            _bst_node_free_2(node);
            return node_aux;
        }
        else if (node->left)
        {
            node_aux = node->left;
            _bst_node_free_2(node);
            return node_aux;
        }
    }
    return node;
}