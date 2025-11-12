#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

static RBNode* new_node(RBTree *T, int key, Color c) {
    RBNode *z = (RBNode*)calloc(1, sizeof(RBNode));
    z->key = key;
    z->color = c;
    z->left = z->right = z->parent = T->nil;
    return z;
}

RBTree* rbt_create(void) {
    RBTree *T = (RBTree*)calloc(1, sizeof(RBTree));
    T->nil = (RBNode*)calloc(1, sizeof(RBNode));
    T->nil->color = BLACK;
    T->nil->left = T->nil->right = T->nil->parent = T->nil;
    T->root = T->nil;
    return T;
}

static void free_subtree(RBTree *T, RBNode *x) {
    if (x == T->nil) return;
    free_subtree(T, x->left);
    free_subtree(T, x->right);
    free(x);
}

void rbt_free(RBTree *T) {
    if (!T) return;
    free_subtree(T, T->root);
    free(T->nil);
    free(T);
}

/* rotations */
static void left_rotate(RBTree *T, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != T->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == T->nil) T->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void right_rotate(RBTree *T, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != T->nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == T->nil) T->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void rbt_insert(RBTree *T, int key) {
    RBNode *z = new_node(T, key, RED);
    RBNode *y = T->nil;
    RBNode *x = T->root;
    while (x != T->nil) {
        y = x;
        if (z->key < x->key) x = x->left;
        else if (z->key > x->key) x = x->right;
        else { free(z); return; }
    }
    z->parent = y;
    if (y == T->nil) T->root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;

    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(T, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(T, z->parent->parent);
            }
        }
    }
    T->root->color = BLACK;
}

RBNode* rbt_search(RBTree *T, int key) {
    RBNode *x = T->root;
    while (x != T->nil) {
        if (key == x->key) return x;
        x = (key < x->key) ? x->left : x->right;
    }
    return NULL;
}

RBNode* rbt_min(RBTree *T, RBNode *x) {
    if (x == NULL) return NULL;
    while (x->left != T->nil) x = x->left;
    return x;
}

RBNode* rbt_max(RBTree *T, RBNode *x) {
    if (x == NULL) return NULL;
    while (x->right != T->nil) x = x->right;
    return x;
}

RBNode* rbt_successor(RBTree *T, RBNode *x) {
    if (!x || x==T->nil) return NULL;
    if (x->right != T->nil) return rbt_min(T, x->right);
    RBNode *y = x->parent;
    while (y != T->nil && x == y->right) { x = y; y = y->parent; }
    return (y==T->nil)? NULL : y;
}

RBNode* rbt_predecessor(RBTree *T, RBNode *x) {
    if (!x || x==T->nil) return NULL;
    if (x->left != T->nil) return rbt_max(T, x->left);
    RBNode *y = x->parent;
    while (y != T->nil && x == y->left) { x = y; y = y->parent; }
    return (y==T->nil)? NULL : y;
}

int rbt_height(RBTree *T, RBNode *x) {
    if (x == T->nil) return -1; 
    int lh = rbt_height(T, x->left);
    int rh = rbt_height(T, x->right);
    return (lh>rh?lh:rh) + 1;
}

void rbt_inorder_print(RBTree *T, RBNode *x) {
    if (x == T->nil) return;
    rbt_inorder_print(T, x->left);
    printf("%d ", x->key);
    rbt_inorder_print(T, x->right);
}

static void bst_transplant(RBTree *T, RBNode *u, RBNode *v) {
    if (u->parent == T->nil) T->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

bool rbt_delete_bst(RBTree *T, int key) {
    RBNode *z = rbt_search(T, key);
    if (!z) return false;
    if (z->left == T->nil) {
        bst_transplant(T, z, z->right);
        free(z);
    } else if (z->right == T->nil) {
        bst_transplant(T, z, z->left);
        free(z);
    } else {
        RBNode *y = rbt_min(T, z->right);  
        if (y->parent != z) {
            bst_transplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        bst_transplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        free(z);
    }
    return true;
}
