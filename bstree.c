#include <stdlib.h>
#include <stdio.h>
#include "bstree.h"


#define MAX_BUFFER 64 // Maximum file line size

/* START [_BSTNode] */
typedef struct _BSTNode {
    void * info;
    struct _BSTNode * left;
    struct _BSTNode * right;
    struct _BSTNode * parent;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree {
    BSTNode * root;
    P_tree_ele_print print_ele;
    P_tree_ele_cmp cmp_ele;
};
/* END [_BSTree] */


/*** BSTNode TAD private functions ***/
BSTNode * _bst_node_new(){
    BSTNode * pn = NULL;

    pn = malloc(sizeof(BSTNode));
    if (!pn) {
        return NULL;
    }
 
    pn->left = pn->right = NULL;
    pn->parent =NULL;
    pn->info = NULL;
    return pn;
}

void _bst_node_free (BSTNode * pn){
    if (!pn) return;
    if(pn->info)
        free(pn->info);
    free(pn);
}

void _bst_node_free_rec (BSTNode * pn){
    if (!pn) return;

    _bst_node_free_rec(pn->left);
    _bst_node_free_rec(pn->right);
    _bst_node_free(pn);

    return;
}

int _bst_depth_rec (BSTNode * pn) {
    int depthR, depthL;
    if (!pn) return 0;

    depthL = _bst_depth_rec(pn->left);
    depthR = _bst_depth_rec(pn->right);

    if (depthR > depthL) {
        return depthR + 1;
    } else {
        return depthL + 1;
    }
}

int _bst_size_rec(BSTNode * pn) {
    int count = 0;
    if (!pn) return count;

    count += _bst_size_rec(pn->left);
    count += _bst_size_rec(pn->right);

    return count + 1;
}

int _bst_preOrder_rec (BSTNode * pn, FILE * pf, P_tree_ele_print print_ele) {
    int count = 0;
    if (!pn) return count;

    count += print_ele(pf, pn->info);
    count += _bst_preOrder_rec(pn->left, pf, print_ele);
    count += _bst_preOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_inOrder_rec (BSTNode * pn, FILE * pf, P_tree_ele_print print_ele) {
    int count = 0;
    if (!pn) return count;

    count += _bst_inOrder_rec(pn->left, pf, print_ele);
    count += print_ele(pf, pn->info);
    count += _bst_inOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_postOrder_rec (BSTNode * pn, FILE * pf, P_tree_ele_print print_ele) {
    int count = 0;
    if (!pn) return count;

    count += _bst_postOrder_rec(pn->left, pf, print_ele);
    count += _bst_postOrder_rec(pn->right, pf, print_ele);
    count += print_ele(pf, pn->info);

    return count;
}

Bool tree_contains_rec (BSTNode * node, const void * elem, P_tree_ele_cmp f);

BSTNode *tree_insert_rec (BSTNode * node, const void * elem, P_tree_ele_cmp f);

void tree_print_rec (BSTNode *node, P_tree_ele_print print_ele);

/*** BSTree TAD functions ***/
BSTree * tree_init(P_tree_ele_print print_ele, P_tree_ele_cmp cmp_ele){
    if (!print_ele || !cmp_ele) return NULL;

    BSTree * tree = malloc (sizeof(BSTree));
    if (!tree) {
        return NULL;
    }

    tree->root = NULL;
    tree->print_ele = print_ele;
    tree->cmp_ele = cmp_ele;

    return tree;
}

void tree_destroy (BSTree * tree) {
    if (!tree) return;

    _bst_node_free_rec(tree->root);

    free(tree);
    return;
}

Bool tree_isEmpty( const BSTree * tree){
    if (!tree || !tree->root) return TRUE;
    return FALSE;
}

int tree_depth (const BSTree * tree){
    if (!tree) return -1;

    return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree * tree) {
    if (!tree) return -1;

    return _bst_size_rec(tree->root);
}

int tree_preOrder (FILE *f, const BSTree * tree){
    if (!f || !tree) return -1;

    return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder (FILE *f, const BSTree * tree){
    if (!f || !tree) return -1;

    return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder (FILE *f, const BSTree * tree){
    if (!f || !tree) return -1;

    return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

void * tree_find_min (BSTree * tree){
    if (!tree) return NULL;
    
    BSTNode *nAux = tree->root;

    while (nAux->left != NULL)
        nAux = nAux->left;

    return nAux->info;
}

void * tree_find_max (BSTree * tree){
    if (!tree) return NULL;
    
    BSTNode *nAux = tree->root;
    while (nAux->right != NULL)
        nAux = nAux->right;

    return nAux->info;
}

Bool tree_contains (BSTree * tree, const void * elem){
    if (!tree||!elem) return FALSE;
    int result = 0; 
    result = tree->cmp_ele(elem, tree->root->info);
    
    if (result==0) return TRUE;
    if (result < 0)
        return tree_contains_rec(tree->root->left, elem, tree->cmp_ele);

    return tree_contains_rec(tree->root->right, elem, tree->cmp_ele);  
}

Bool tree_contains_rec (BSTNode * node, const void * elem, P_tree_ele_cmp f){
    int result = 0; 
    result = f(elem, node->info);
    if (result==0) return TRUE;
    if (result < 0)
        return tree_contains_rec(node->left, elem, f);
    return tree_contains_rec(node->right, elem, f);

}

Status tree_insert (BSTree * tree, const void * elem){
    BSTNode *nAux = NULL;

    if (!tree || !elem) return ERROR;
    nAux = tree_insert_rec(tree->root, elem, tree->cmp_ele);
    if (nAux == NULL) return ERROR;
    tree->root = nAux;
    return OK;
    
}

BSTNode *tree_insert_rec (BSTNode * node, const void * elem, P_tree_ele_cmp f){
    
    int cmp_result = 0;
    
    if (node == NULL) {
        node = _bst_node_new();
        if (node == NULL) return NULL;
        node->info = (void *) elem;
        return node;
    }

    cmp_result = f(elem, node->info);
    if (cmp_result < 0)
        node->left = tree_insert_rec(node->left, elem, f);
    else if (cmp_result > 0)
        node->right = tree_insert_rec(node->right, elem, f);
    return node;
}

void tree_print(BSTree *t){ ////////////////////CONTROL DE ERRORRESSSS DE ROOT INVALIDO
    if (!t) return;
    t->print_ele(stdout, t->root->info);
    printf("\n");
    tree_print_rec(t->root, t->print_ele);
    return;
}

void tree_print_rec (BSTNode *node, P_tree_ele_print print_ele){   
    if (node == NULL) return;
    
    
    if (node->left != NULL && node->right != NULL){ /////NO SE CONTEMPLA SOLO UN NODO POR LADO, CHECKEAR SI EL PADRE TIENE NODO DERECHO ANTES DE |N
        print_ele(stdout, node->left->info);
        print_ele(stdout, node->right->info);
        printf("\n");
    }
    if (node->left != NULL)
        tree_print_rec(node->left, print_ele);

    if (node->right != NULL)
        tree_print_rec(node->right, print_ele);
    return;

}