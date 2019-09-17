#include<stdio.h>
#include<stdlib.h>

// An RAVL tree node
struct Node
{
    int key;
    struct Node *left;
    struct Node *right;
    int rank;
    int count;
};

//performance measure variables
static int no_rot=0;
static float len=0,no_ops=0;
static int bal_update=0;
static int max_len=0;
static int miss=0;

// A utility function to get maximum of two integers
int max(int a, int b);
// A utility function to calculate average length
void avg_len(struct Node *node)
{
    len+=node->rank+1;
    if(node->rank>max_len)
    {
        max_len=node->rank;
    }
    no_ops++;
}
// A utility function to get the height of the tree
int rank(struct Node *N)
{
    if (N == NULL)
        return -1;
    return N->rank;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct Node* newNode(int key)
{
    struct Node* node = (struct Node*)
                        malloc(sizeof(struct Node));
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->rank = 0;  // new node is initially added at leaf
    node->count = 1;
    return(node);
}

// A utility function to right rotate subtree rooted with y
struct Node *rightRotate(struct Node *y)
{
    no_rot++;
    struct Node *x = y->left;
    struct Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // demote rank of node

    y->rank-=1;

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x

struct Node *leftRotate(struct Node *x)
{
    no_rot++;
    struct Node *y = x->right;
    struct Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    //  demote rank of node

    x->rank-=1;

    // Return new root
    return y;
}


// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
struct Node* insert(struct Node* node, int key)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(key));

    if (key == node->key)
    {
        (node->count)++;
        return node;
    }

    if (key < node->key)
        node->left  = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    //calculating rank difference
    int left_diff=rank(node)-rank(node->left);
    int right_diff=rank(node)-rank(node->right);

    // Case for unbalanced tree

    if((left_diff==0 && right_diff==1)||(left_diff==1 && right_diff==0))
    {
        node->rank+=1;

        return node;
    }

        if(right_diff==0)
        {
            int temp_left=rank(node->right)-rank(node->right->left);
            int temp_right=rank(node->right)-rank(node->right->right);
            if((temp_left==2) || (rank(node->right->left)==-1))
            {
                bal_update++;
               return leftRotate(node);
            }
            else
            {
                node->right = rightRotate(node->right);
                node->right->rank+=1;
                bal_update++;
                return leftRotate(node);

            }

        }
        if(left_diff==0)
        {
            int temp_left=rank(node->left)-rank(node->left->left);
            int temp_right=rank(node->left)-rank(node->left->right);
            if((temp_right==2) || (rank(node->left->right)==-1))
            {
                 bal_update++;
                return rightRotate(node);
            }
            else
            {
                node->left =  leftRotate(node->left);
                node->left->rank+=1;
                 bal_update++;
                return rightRotate(node);
            }
        }

        return node;

}
struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}
struct Node* deleteNode(struct Node* root, int key)
{
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
    {
        miss++;
        return root;
    }

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key);

    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key);

    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {

        if (root->count > 1)
        {
            (root->count)--;
            return;
        }
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Node* temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
      return root;



    return root;
}

// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void preOrder(struct Node *root)
{
    if(root != NULL)
    {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }

}

/* Drier program to test above function*/
int main()
{
  struct Node *root = NULL;

    FILE* fptr=fopen("test.csv","r");
     if (fptr == NULL) {
        fprintf(stderr, "Error reading file\n");
        return 1;
    }
    char op;
    int inp;
    while(fscanf(fptr,"%c%d",&op,&inp)!=EOF)
    {
        if(op=='F')
            {
                root=insert(root,inp);
                avg_len(root);
            }
            else if(op=='A')
            {
                root=deleteNode(root,inp);
                avg_len(root);
            }
    }

    printf("Preorder traversal of the constructed RAVL"
            " tree is \n");
    preOrder(root);
    float avg_length=len/no_ops;
    printf("\nno of rotations:%d\n",no_rot);
    printf("average length :%f\n",avg_length);
    printf("balance update :%d\n",bal_update);
    printf("maximum length :%d\n",max_len+1);
    printf("node miss :%d\n",miss);
  return 0;
}

