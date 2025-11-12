#include "rbtree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* read initial keys from file: one or many ints (space or newline separated) */
static void load_file(RBTree *T, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { perror("fopen"); exit(1); }
    int x;
    while (fscanf(f, "%d", &x) == 1) rbt_insert(T, x);
    fclose(f);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_numbers_file>\n", argv[0]);
        return 1;
    }
    RBTree *T = rbt_create();
    load_file(T, argv[1]);

    printf("Initial height: %d\n", rbt_height(T, T->root));
    printf("Commands: insert X | delete X | search X | min | max | successor X | predecessor X | sort | quit\n");

    char cmd[64];
    int x;
    for (;;) {
        printf("> ");
        if (!fgets(cmd, sizeof(cmd), stdin)) break;

        if (sscanf(cmd, "insert %d", &x) == 1) {
            rbt_insert(T, x);
            printf("ok\n");
        } else if (sscanf(cmd, "delete %d", &x) == 1) {
            RBNode *n = rbt_search(T, x);
            if (n) {
                rbt_delete_bst(T, x);
                printf("deleted\n");
            } else {
                printf("not found\n");
            }
        } else if (sscanf(cmd, "search %d", &x) == 1) {
            printf("%s\n", rbt_search(T, x) ? "found" : "not found");
        } else if (sscanf(cmd, "successor %d", &x) == 1) {
            RBNode *n = rbt_search(T, x);
            if (!n) printf("not found\n");
            else {
                RBNode *s = rbt_successor(T, n);
                if (s) printf("%d\n", s->key);
                else printf("none\n");
            }
        } else if (sscanf(cmd, "predecessor %d", &x) == 1) {
            RBNode *n = rbt_search(T, x);
            if (!n) printf("not found\n");
            else {
                RBNode *p = rbt_predecessor(T, n);
                if (p) printf("%d\n", p->key);
                else printf("none\n");
            }
        } else if (strncmp(cmd, "min", 3) == 0) {
            RBNode *m = rbt_min(T, T->root);
            if (m) printf("%d\n", m->key); else printf("empty\n");
        } else if (strncmp(cmd, "max", 3) == 0) {
            RBNode *m = rbt_max(T, T->root);
            if (m) printf("%d\n", m->key); else printf("empty\n");
        } else if (strncmp(cmd, "sort", 4) == 0) {
            rbt_inorder_print(T, T->root);
            printf("\n");
        } else if (strncmp(cmd, "quit", 4) == 0) {
            break;
        } else {
            printf("unknown command\n");
        }

        printf("height: %d\n", rbt_height(T, T->root));
    }

    rbt_free(T);
    return 0;
}
