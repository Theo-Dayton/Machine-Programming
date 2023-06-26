#include <stdio.h>
#include <stdlib.h>

struct node {
const char *person; /* search key for binary-search tree */
const char *footwear;
struct node *left, *right; /* children */
};
typedef struct node *Tree;


int countfunc(struct node * currnode) {
    int count = 1;
    if (currnode->left == NULL && currnode->right == NULL) {
        return 1;
    }
    else {
        if (currnode->left != NULL) {
            count += countfunc(currnode->left);
        }
        if (currnode->right != NULL) {
            count += countfunc(currnode->right);
        }
    }
    printf("Num of nodes: %d \n", count);
    return count;
}

static struct node rt = {
"Townsend",
"Saucony",
NULL,
NULL
};

static struct node lt = {
"Sheldon",
"Freeds",
NULL,
NULL
};

static struct node ms = {
"Ramsey",
"Birkenstock",
&lt,
&rt /* *address* of node rt makes a pointer to that node */
};


int main() {
    countfunc(&ms);
    return 0;
}