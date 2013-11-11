/*
 * This file covers binary tree traverses and some other operations, which include:
 * Traverse:
 *    Recurrsive traverse - pre-order, in-order, post-order.
 *    Non-recurrsive, use stack - pre-order, in-order, post-order.
 *    Non-recurrsive, use parent - pre-order, in-order, post-order.
 *    Morrise - pre-order, in-order.
 * Others:
 *    Generate mirror of the tree.
 *    Least common ancestor.
 */

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct bt_ {
    unsigned long index;
    unsigned long key;
    int left;
    int right;
} bt_t;

typedef struct bt_p_ {
    unsigned long index;
    unsigned long key;
    int left;
    int right;
    int parent;
} bt_p_t;


#define TREE_SIZE 10

static bt_p_t binary_tree[TREE_SIZE] = {
    {1, 12, 6, 2, 5}, {2, 15, 7, -1, -1}, {3, 4, 9, -1, 0}, {4, 10, -1, 8, 5}, {5, 2, -1, -1, 8},
    {6, 18, 0, 3, -1}, {7, 7, -1, -1, 0}, {8, 14, 5, 1, -1}, {9, 21, -1, 4, 3}, {10, 5, -1, -1, 2},
};

static bt_t binary_tree_2[TREE_SIZE] = {
    {1, 13, 1, 7}, {2, 16, 2, 4}, {3, 5, 3, -1}, {4, 11, -1, -1}, {5, 3, 5, 6},
    {6, 19, -1, -1}, {7, 8, -1, -1}, {8, 15, 8, 9}, {9, 22, -1, -1}, {10, 7, -1, -1},
};

/*
 * 10.4-2: Write an O(n)-time recursive procedure that, given an n-node
 * binary tree, prints out the key of each node in the tree.
 * (Pre-order traversal)
 */
static void
display_recurrsive_pre_order (bt_t *node, bt_t *tree)
{
    printf("{%d, %d} ", node->index, node->key);
    if (node->left != -1) {
        display_recurrsive_pre_order(&(tree[node->left]), tree);
    }
    if (node->right != -1) {
        display_recurrsive_pre_order(&(tree[node->right]), tree);
    }
}

static void
display_recurrsive_in_order (bt_t *node, bt_t *tree)
{
    if (node->left != -1) {
        display_recurrsive_in_order(&(tree[node->left]), tree);
    }
    printf("{%d, %d} ", node->index, node->key);
    if (node->right != -1) {
        display_recurrsive_in_order(&(tree[node->right]), tree);
    }
}

void
stack_print (int *stack)
{
    unsigned short i;

    printf("\n");
    for (i = 0; i < TREE_SIZE && stack[i] != -1; i++) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}

/*
 * 10.4-3: Write an O(n)-time nonrecursive procedure that, given an n-node
 * binary tree, prints out the key of each node in the tree. Use a stack as
 * an auxiliary data structure.
 *
 * The following code is a pre-order traversal.
 * 1. Visit the root.
 * 2. Traverse the left subtree.
 * 3. Traverse the right subtree.
 *
 * The pre-order traversal can be implemented by whenever popping a node,
 * push its right child then left child onto the stack.
 */
static void
display_nonrecurrsive_pre_order (bt_t *root, bt_t *tree)
{
    /*
     * Use a stack with size log(n) + 1, which is the depth of binary tree
     */
    int stack[4] = {-1, -1, -1, -1};
    unsigned long top = -1;
    bt_t *node;

    /*
     * Push the root onto the stack
     */
    stack[++top] = root->index - 1;
    while (stack[top] != -1) {
        /*
         * Pop the top of the stack
         */
        node = &(tree[stack[top]]);
        printf("{%d, %d} ", node->index, node->key);
        stack[top--] = -1;
        /*
         * Push the right child then left child onto the stack
         */
        if (node->right != -1) {
            stack[++top] = node->right;
        }
        if (node->left != -1) {
            stack[++top] = node->left;
        }
    }
}

#define TRUE 1
#define FALSE 0

/*
 * In-order nonrecurrsive traversal.
 * 1. Traverse the left subtree.
 * 2. Visit the root.
 * 3. Traverse the right subtree.
 *
 * The steps are
 * 1. Traverse the left subtree and push each node on the way from the root.
 * 2. Pop the top node and print the value.
 * 3. If the popped node has right child, push the right child.
 * 4. Repeat step 1 by treating the right child as the root.
 *
 * A boolean 'push' is used to check whether this is a new root (the original
 * root or a right child), this is to avoid push the left subtree again after
 * popping a node whose left subtree has already pushed.
 */
static void
display_nonrecurrsive_in_order (bt_t *root, bt_t *tree)
{
    int stack[4] = {-1, -1, -1, -1};
    int top = -1;
    bt_t *node = root;
    bool push = TRUE;

    /*
     * Push the root onto the stack
     */
    stack[++top] = node->index - 1;
    while (top != -1) {
        while (push && node->left != -1) {
            /*
             * Push all nodes and their left children onto the stack
             */
            stack[++top] = node->left;
            node = &tree[node->left];
        }
        push = FALSE;
        /*
         * Pop the top of the stack
         */
        node =  &(tree[stack[top]]);
        stack[top--] = -1;
        printf("{%d, %d} ", node->index, node->key);

        if (node->right != -1) {
           /*
            * Push the right child onto the stack
            */
           stack[++top] = node->right;
           node = &tree[node->right];
           push = TRUE;
        }
    }
}

/*
 * Post-order traversal
 * 1. Traverse the left subtree.
 * 2. Traverse the right subtree.
 * 3. Visit the root.
 *
 * Steps:
 * 1. Traverse the left subtree and push each node along the way from root.
 * 2. Peek at the top of the stack. If the node does not have a right child
 *    (including leaves), or if the last popped node is the right child of
 *    this node (the right subtree has been pushed and popped), pop it and
 *    print it.
 * 3. If this node has a right child, and it hasn't been pushed yet. Push the
 *    right child and treat it as the new root. Reset the prev node to NULL
 *    in order to avoid push the left subtree of a node again.
 */
static void
display_nonrecurrsive_post_order (bt_t *root, bt_t *tree)
{
    int stack[4] = {-1, -1, -1, -1};
    int top = -1, i = 0;;
    bt_t *node = root, *prev = NULL;

    /*
     * Push the root onto the stack
     */
    stack[++top] = node->index - 1;
    while (top != -1) {
        while (prev == NULL && node->left != -1) {
            /*
             * Push the left subtree onto the stack, only if prev is NULL.
             */
            stack[++top] = node->left;
            node = &tree[node->left];
        }
        /*
         * Pop the top of the stack
         */
        node = &(tree[stack[top]]);
        if (node->right == -1 || node->right == prev->index - 1) {
            stack[top--] = -1;
            printf("{%d, %d} ", node->index, node->key);
            prev = node;
        } else {
           /*
            * Push the right child onto the stack
            */
           stack[++top] = node->right;
           node = &tree[node->right];
           prev = NULL;
        }
    }
}

static void
bt_enqueue (int *queue, int *head, int *tail, int index)
{
    if (((*tail) + 1) % TREE_SIZE == *head) {
        printf("\nQueue overflow");
    }

    if (*head == -1) {
        *head = *tail = 0;
    }
    queue[*tail] = index;
    *tail = ((*tail) + 1) % TREE_SIZE;
//    printf("\nEnqueue node %u, head %u, tail %u", index, *head, *tail);
}

static int
bt_dequeue (int *queue, int *head, int *tail)
{
    int index;

    if (*head == *tail) {
        printf("\nQueue underflow");
    }

    index = queue[*head];
    queue[*head] = -1;
    *head = ((*head) + 1) % TREE_SIZE;
//    printf("\nDequeue node %u, head %u, tail %u", index, *head, *tail);
    return (index);
}

/*
 * To traverse the tree in the same-level order, we use a queue as
 * the auxiliary storage. For each node, we first enqueue its left
 * child then right.
 */
static void
display_same_level_order (bt_t *root, bt_t *tree)
{
    int queue[TREE_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int head = -1, tail = -1, index;
    bt_t *node;

    /*
     * Enqueue the root
     */
    bt_enqueue(queue, &head, &tail, root->index - 1);
    while (queue[head] != -1 && head != tail) {
        /*
         * Dequeue the head, and print its value
         */
        index = bt_dequeue(queue, &head, &tail);
        node = &tree[index];
        printf("{%d, %d} ", node->index, node->key);
        if (node->left != -1) {
            bt_enqueue(queue, &head, &tail, node->left);
        }
        if (node->right != -1) {
            bt_enqueue(queue, &head, &tail, node->right);
        }
    }
}

/*
 * Generate a mirror tree of the given binary tree. In the recurrsive
 * algorithm, each node is treated as a new tree, and simply swap
 * the left and right child. This is done recurrsively first on the
 * left subtree and then on the right subtree till it reaches the leaves.
 */
static void
generate_mirror_recurrsive (bt_t *node, bt_t *tree)
{
    int tmp;

    if (node->left == -1 && node->right == -1) {
        return;
    }
    tmp = node->left;
    node->left = node->right;
    node->right = tmp;
    if (node->left != -1)
        generate_mirror_recurrsive(&tree[node->left], tree);
    if (node->right != -1)
        generate_mirror_recurrsive(&tree[node->right], tree);
}

/*
 * In the nonrecurrsive algorithm, we use a stack as an auxilian storage.
 * Each node is pushed onto the stack. When a node is popped, first swap
 * its left and right child and then push its left child onto the stack
 * then right child.
 */
static void
generate_mirror_nonrecurrsive (bt_t *root, bt_t *tree)
{
    int queue[TREE_SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int head = -1, tail = -1, tmp;
    bt_t *node;

    /*
     * Enqueue the root
     */
    bt_enqueue(queue, &head, &tail, root->index - 1);
    while (queue[head] != -1) {
        /*
         * Dequeue the head, and print its value
         */
        node = &tree[bt_dequeue(queue, &head, &tail)];
        tmp = node->left;
        node->left = node->right;
        node->right = tmp;
        if (node->left != -1) {
            bt_enqueue(queue, &head, &tail, node->left);
        }
        if (node->right != -1) {
            bt_enqueue(queue, &head, &tail, node->right);
        }
    }
}

static void
generate_mirror_wrapper (bt_t *root, bt_t *tree)
{
    bt_t mirror_tree[TREE_SIZE];
    bt_t *mirror_root;

    printf("\n\nGenerate tree mirror");
    printf("\nOriginal     :");
    display_same_level_order(root, tree);

    memcpy(mirror_tree, tree, (sizeof(bt_t) * TREE_SIZE));
    mirror_root = &(mirror_tree[root->index - 1]);

    generate_mirror_nonrecurrsive(mirror_root, mirror_tree);
    printf("\nNonrecurrsive:");
    display_same_level_order(mirror_root, mirror_tree);
    printf("\nRecurrsive   :");
    generate_mirror_recurrsive(mirror_root, mirror_tree);
    display_same_level_order(mirror_root, mirror_tree);
    printf("\n");
}

/*
 * Least common ancestor of two nodes in the tree.
 *
 * This can be done recurrsively. The condition that a node is the LCA
 * of two nodes in the tree is that one of the node is in the left
 * subtree of this node and the other node is in the right subtree
 * of this node.
 *
 * A static storage found is used to keep track of how many of the
 * two nodes have been found, if both found, skip the rest of the
 * traversal.
 *
 * variable node_as_root is used for cases when one of the two nodes
 * is the ancestor of the other node. Thus the other node can be in
 * either the left or the right subtree.
 */
static bool
least_common_ancestor (bt_t *root, bt_t *tree,
                       int idx_1, int idx_2,
                       bt_t **lca, bool reset)
{
    static unsigned short found;
    bool node_as_root = FALSE, left = FALSE, right = FALSE;

    if (reset == TRUE) {
        found = 0;
    }
    if (root->index == idx_1 || root->index == idx_2) {
        found++;
        node_as_root = TRUE;
    }
    if (found < 2 && root->left != -1) {
        left = least_common_ancestor(&tree[root->left], tree, idx_1, idx_2, lca, FALSE);
    }
    if (found < 2 && root->right != -1) {
        right = least_common_ancestor(&tree[root->right], tree, idx_1, idx_2, lca, FALSE);
    }

    if ((left && right) || ((left || right) && node_as_root)) {
        *lca = root;
    }
    return (left || right || node_as_root);
}

static void
traverse_with_parent_pre_order (bt_p_t *root, bt_p_t *tree)
{
    bt_p_t *node = root, *prev = NULL;

    while (node) {
        if (prev == NULL || node->parent == prev->index - 1) {
            printf("{%d, %d} ", node->index, node->key);
            prev = node;
            if (node->left != -1) {
                node = &tree[node->left];
            } else {
                if (node->right != -1) {
                    node = &tree[node->right];
                } else {
                    if (node->parent == -1) break;
                    node = &tree[node->parent];
                }
            }
        } else { //Traverse up
            if (node->left == prev->index - 1) {
                prev = node;
                if (node->right != -1) {
                    node = &tree[node->right];
                } else {
                    if (node->parent == -1) break;
                    node = &tree[node->parent];
                }
            } else {
                prev = node;
                if (node->parent == -1) break;
                node = &tree[node->parent];
            }

        }
    }
    printf("\n");
}

static void
traverse_with_parent_in_order (bt_p_t *root, bt_p_t *tree)
{
    bt_p_t *node = NULL, *prev = NULL;

    if (root == NULL || tree == NULL) {
        return;
    }
    node = root;

    while (node) {
        if (prev == NULL || prev->index - 1 == node->parent) { //Traverse down the tree
            prev = node;
            if (node->left != -1) {
                node = &tree[node->left];
                continue;
            } else { //Either a leaf or a node without left child
                printf("{%d, %d} ", node->index, node->key);
                if (node->right != -1) {
                    node = &tree[node->right];
                } else {
                    if (node->parent == -1) break;
                    node = &tree[node->parent];
                }
            }
        } else { //Traverse up the tree
            if (prev->index - 1 == node->left) { //Last visited is left child, print before go to next.
                printf("{%d, %d} ", node->index, node->key);
                prev = node;
                if (node->right != -1) {
                    node = &tree[node->right];
                } else {
                    if (node->parent == -1) break;
                    node = &tree[node->parent];
                }
            } else { //Last visited is right child, go to parent.
                prev = node;
                if (node->parent == -1) break;
                node = &tree[node->parent];
            }
        }
    }
    printf("\n");
}

static void
traverse_with_parent_post_order (bt_p_t *root, bt_p_t *tree)
{
    bt_p_t *node = root, *prev = NULL;

    while (node) {
        if (prev == NULL || prev->index - 1 == node->parent) { //Traverse down
            prev = node;
            if (node->left != -1) {
                node = &tree[node->left];
            } else {
                if (node->right != -1) { //Has right child, traverse right child first
                    node = &tree[node->right];
                } else { // No right child, print and go back to parent
                    printf("{%d, %d} ", node->index, node->key);
                    if (node->parent == -1) break;
                    node = &tree[node->parent];
                }
            }
        } else {  //Traverse up
            if (prev->index - 1 == node->left) { //Visited left child already
                prev = node;
                if (node->right != -1) {
                    node = &tree[node->right];
                } else {
                    printf("{%d, %d} ", node->index, node->key);
                    if (node->parent == -1) break;
                    node = &tree[node->parent];
                }
            } else {
                printf("{%d, %d} ", node->index, node->key);
                prev = node;
                if (node->parent == -1) break;
                node = &tree[node->parent];
            }
        }
    }
    printf("\n");
}

/*
 * Using Morris Traversal, we can traverse the tree without using stack and recursion.
 * The idea of Morris Traversal is based on Threaded Binary Tree. In this traversal,
 * we first create links to Inorder successor and print the data using these links,
 * and finally revert the changes to restore original tree.
 *
 * 1. Initialize current as root
 * 2. While current is not NULL
 *   If current does not have left child
 *      a) Print current's data
 *      b) Go to the right, i.e., current = current->right
 *   Else
 *      a) Get the rightest node in the current's left subtree
 *      b) If the rightest node's right point to current, remove thread, go to right child.
 *      c) Else, let rightest node point to current and go to left child
 */
static void
morris_traverse_preorder (bt_t *root, bt_t *tree)
{
    bt_t *node = root;
    bt_t *prev = root;

    if (root == NULL) return;
    while (node) {
        if (node->left == -1) {
            printf("{%d, %d} ", node->index, node->key);
            if (node->right == -1) break;
            node = &tree[node->right];
        } else {
            prev = &tree[node->left];
            while (prev->right != -1 && prev->right != node->index - 1) {
                prev = &tree[prev->right];
            }

            if (prev->right == -1) {
                printf("{%d, %d} ", node->index, node->key);
                prev->right = node->index - 1;
                node = &tree[node->left];
            } else {
                prev->right = -1;
                node = &tree[node->right];
            }
        }
    }
    printf("\n");
}

static void
morris_traverse_inorder (bt_t *root, bt_t *tree)
{
    bt_t *node = root;
    bt_t *prev = root;

    if (root == NULL) return;
    while (node) {
        if (node->left == -1) {
            printf("{%d, %d} ", node->index, node->key);
            if (node->right == -1) break;
            node = &tree[node->right];
        } else {
            prev = &tree[node->left];
            while (prev->right != -1 && prev->right != node->index - 1) {
                prev = &tree[prev->right];
            }

            if (prev->right == -1) {
                prev->right = node->index - 1;
                node = &tree[node->left];
            } else {
                printf("{%d, %d} ", node->index, node->key);
                prev->right = -1;
                node = &tree[node->right];
            }
        }
    }
    printf("\n");
}

int
main (void)
{
#if 0
    printf("\nPre-order");
    printf("\nRecurrsive   : ");
    display_recurrsive_pre_order(&(binary_tree_2[0]), binary_tree_2);
    printf("\nNonrecurrsive: ");
    display_nonrecurrsive_pre_order(&(binary_tree_2[0]), binary_tree_2);

    printf("\n\nIn-order");
    printf("\nRecurrsive   : ");
    display_recurrsive_in_order(&(binary_tree_2[0]), binary_tree_2);
    printf("\nNonrecurrsive: ");
    display_nonrecurrsive_in_order(&(binary_tree_2[0]), binary_tree_2);

    printf("\n\nPost-order");
    printf("\nRecurrsive   : ");
    display_recurrsive_post_order(&(binary_tree_2[0]), binary_tree_2);
    printf("\nNonrecurrsive: ");
    display_nonrecurrsive_post_order(&(binary_tree_2[0]), binary_tree_2);

    printf("\n\nSame level order\n");
    display_same_level_order(&(binary_tree_2[0]), binary_tree_2);
    printf("\n");

    generate_mirror_wrapper(&(binary_tree_2[0]), binary_tree_2);

    bt_t *lca = NULL;
    bool found = FALSE;
    found = least_common_ancestor(&binary_tree_2[0], binary_tree_2, 4, 5, &lca, TRUE);
    printf("\nLCA of {4, 5} is %d", lca ? lca->index : -1);
    found = least_common_ancestor(&binary_tree_2[0], binary_tree_2, 4, 10, &lca, TRUE);
    printf("\nLCA of {4, 10} is %d", lca ? lca->index : -1);
    found = least_common_ancestor(&binary_tree_2[0], binary_tree_2, 3, 8, &lca, TRUE);
    printf("\nLCA of {3, 8} is %d", lca ? lca->index : -1);
    found = least_common_ancestor(&binary_tree_2[0], binary_tree_2, 5, 6, &lca, TRUE);
    printf("\nLCA of {5, 6} is %d", lca ? lca->index : -1);
    found = least_common_ancestor(&binary_tree_2[0], binary_tree_2, 2, 4, &lca, TRUE);
    printf("\nLCA of {2, 4} is %d", lca ? lca->index : -1);
    found = least_common_ancestor(&binary_tree_2[0], binary_tree_2, 9, 10, &lca, TRUE);
    printf("\nLCA of {9, 10} is %d\n", lca ? lca->index : -1);

    traverse_with_parent_pre_order(&binary_tree[5], binary_tree);
    traverse_with_parent_in_order(&binary_tree[5], binary_tree);
    traverse_with_parent_post_order(&binary_tree[5], binary_tree);
#endif

    morris_traverse_preorder(&binary_tree_2[0], binary_tree_2);
    morris_traverse_inorder(&binary_tree_2[0], binary_tree_2);
}
