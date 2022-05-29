#include <stdio.h>
#include <stdlib.h>

/**
 * ATree. This program takes an array of values as input and creates a BST.
 * The first value in the array will be the root of the BST.
 */

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} node;

void addNode(node **root, const int *arr, int index) {
    if (*root == NULL) {
        *root = (node *) malloc(sizeof(node));
        (*root)->val = *arr;
        (*root)->left = NULL;
        (*root)->right = NULL;
    } else if ((*(arr + index) < (*root)->val) && (*root)->left == NULL) {
        node *leftNode = (node *) malloc(sizeof(node));
        leftNode->val = *(arr + index);
        leftNode->left = NULL;
        leftNode->right = NULL;
        (*root)->left = leftNode;
    } else if ((*(arr + index) >= (*root)->val) && (*root)->right == NULL) {
        node *rightNode = (node *) malloc(sizeof(node));
        rightNode->val = *(arr + index);
        rightNode->left = NULL;
        rightNode->right = NULL;
        (*root)->right = rightNode;
    } else if ((*(arr + index) < (*root)->val) && (*root)->left != NULL) {
        addNode(&(*root)->left, arr, index);
    } else if ((*(arr + index) >= (*root)->val) && (*root)->right != NULL) {
        addNode(&(*root)->right, arr, index);
    }
}

void printPreOrder(node *root) {
    if (root == NULL)
        return;
    printf(" %d ", root->val);
    printPreOrder(root->left);
    printPreOrder(root->right);
}

void printInOrder(node *root) {
    if (root == NULL)
        return;
    printInOrder(root->left);
    printf(" %d ", root->val);
    printInOrder(root->right);
}

void printPostOrder(node *root) {
    if (root == NULL)
        return;
    printPostOrder(root->left);
    printPostOrder(root->right);
    printf(" %d ", root->val);
}

void printOrders(node *root) {
    printf("\nPre-order:\n");
    printPreOrder(root);
    printf("\nIn order:\n");
    printInOrder(root);
    printf("\nPost-order:\n");
    printPostOrder(root);
}

void freeTree(node *root) {
    if (root == NULL)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void getSmallest(node *root, int k, int *iteration, int **arr) {
    if (root == NULL) {
        return;
    }
    getSmallest(root->left, k, iteration, arr);
    if (*iteration <= k) {
        *arr = (int *) realloc(*arr, *iteration * sizeof(int));
        (*iteration)++;
        *(*arr + *iteration - 1) = root->val;
    }
    getSmallest(root->right, k, iteration, arr);
}

int kthSmallest(node *root, int k) {
    int *arr = (int *) malloc(sizeof(int));
    int iteration = 1;
    getSmallest(root, k, &iteration, &arr);
    int kth = *(arr + k);
    free(arr);
    return kth;
}

void createTree(int *arr, size_t size) {
    node *root = NULL;
    for (int i = 0; i < size; i++) {
        addNode(&root, arr, i);
    }
    printOrders(root);
    int k = 1;
    int kth = kthSmallest(root, k);
    printf("\n%dth Smallest: %d", k, kth);
    freeTree(root);
}

int main() {
    int inputArr[] = {6, 0, 4, 7, 5, 4, 6, 2, 0, 2};
    size_t inputArrSize = sizeof(inputArr) / sizeof(inputArr[0]);
    createTree(inputArr, inputArrSize);

    return 0;
}
