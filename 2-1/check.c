#include "binaryTree.h"

static struct TreeNode *InitTree(int counter, int depth, int *i) 
{
    if (counter > depth)
        return NULL;
    
    struct TreeNode *tn = malloc(sizeof(struct TreeNode));
    tn->val = (*i)++;
    tn->left = InitTree(counter+1, depth, i);
    tn->right = InitTree(counter+1, depth, i);

    return tn;
}

/* Assume each value is unique, check if two tree are same */
static bool checkTree(struct TreeNode *a, struct TreeNode *b)
{
    if (!a && !b)
        return true;
    
    if ((a && b) && (a->val == b->val))
        return checkTree(a->left, b->left) && checkTree(a->right, b->right);

    return false;
}

void FreeTree(struct TreeNode *root) 
{
    if (!root)
        return;
    
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

void inorder(struct TreeNode *root, int *ret, int *i)
{
    if (!root)
        return;

    inorder(root->left, ret, i);
    *(ret + (*i)) = root->val;
    (*i)++;
    inorder(root->right, ret, i);
}

void preorder(struct TreeNode *root, int *ret, int *i)
{
    if (!root)
        return;

    *(ret + (*i)) = root->val;
    (*i)++;
    preorder(root->left, ret, i);
    preorder(root->right, ret, i);
}

void print_list(int *ret, int depth)
{
    for(int i=0; i < Tree_size(depth); i++)
        printf("%d -> ", *(ret+i));
    printf("\n");
}

int main(void)
{
    int i = 0;
    int depth = 10;
    struct TreeNode *root = InitTree(1, depth, &i);

    // for tree with d-depth, there are (1 << d) - 1 nodes
    int inorder_list[Tree_size(depth)], preorder_list[Tree_size(depth)]; 

    // measure time
    clock_t start, end;
    clock_t total_time_used = 0;
    int test_time = 10;
    bool success = true;

    i = 0;
    inorder(root, inorder_list, &i);
    i = 0;
    preorder(root, preorder_list, &i);

    // Now test our buildtree function
    for (int i=0; i<test_time; i++) {
        start = clock();
        struct TreeNode *test_root = buildTree(preorder_list, Tree_size(depth), inorder_list, Tree_size(depth));
        end = clock();
        if (!checkTree(root, test_root)) {
            printf("Check tree failed :(\n");
            success = false;
            break;
        }
        
        total_time_used += (end - start);
    }

    if (success)
        printf("elapsed avg time: %f\n", (double)(total_time_used) / CLOCKS_PER_SEC / test_time); 

    FreeTree(root);
}