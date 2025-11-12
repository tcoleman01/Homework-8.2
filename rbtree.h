#ifndef RBTREE_H
#define RBTREE_H
#include <stdbool.h>
#include <stddef.h>

typedef enum { RED=0, BLACK=1 } Color;

typedef struct RBNode {
    int key;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct {
    RBNode *root;
    RBNode *nil;   
} RBTree;

RBTree* rbt_create(void);
void    rbt_free(RBTree *T);

void    rbt_insert(RBTree *T, int key);   
bool    rbt_delete_bst(RBTree *T, int key); 
RBNode* rbt_search(RBTree *T, int key);
RBNode* rbt_min(RBTree *T, RBNode *x);
RBNode* rbt_max(RBTree *T, RBNode *x);
RBNode* rbt_successor(RBTree *T, RBNode *x);
RBNode* rbt_predecessor(RBTree *T, RBNode *x);
int     rbt_height(RBTree *T, RBNode *x);   

void    rbt_inorder_print(RBTree *T, RBNode *x);

#endif
