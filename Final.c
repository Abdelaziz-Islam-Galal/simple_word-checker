#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define INPUT_SIZE 1000

typedef struct Node
{
    char* str;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int height(Node* n);
int int_max(int a, int b);

Node* newNode(char* str);

Node* smallestNode(Node* root);
Node* largestNode(Node* root);
int countNodes(Node* root);

Node* successor(Node* root, Node* node);
Node* predecessor(Node* root, Node* node);

Node *rightRotate(Node *y);
Node *leftRotate(Node *x);
int getBalance(Node *N);

Node *insert(Node *node, char* str);

void freeTree(Node *root);
Node* search(Node *root , char *word); 

char* get_string();
Node* getDictionary(FILE* fptr);
void sentenceChecker(Node* dict);

int main()
{
    FILE *fptr;
    fptr = fopen("Dictionary.txt","r");
    if(fptr == NULL){
        printf("Failed to load dictionary! Try again later\n");
        exit(-1);
    }
    Node* root = getDictionary(fptr);
    fclose(fptr); 

    if(root == NULL){
        printf("Dictionary is Empty!\n");
        exit(0);
    }

    puts("Dictionary Loaded Successfully...!");
    puts("..................................");
    printf("Size: %d\n", countNodes(root));
    puts("..................................");
    printf("Height: %d\n", height(root));
    puts("..................................");
    puts("Enter a sentence :");

    sentenceChecker(root);
    
freeTree(root);
return 0;
}


int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

int int_max(int a, int b)
{
    return (a > b) ? a : b;
}

Node *newNode(char* str)
{
    struct Node *new = malloc(sizeof(Node));
    new->str = str;
    new->left = NULL;
    new->right = NULL;
    new->height = 1;
    return new;
}

Node* smallestNode(Node* root)
{
    if (root == NULL) {
        return NULL;
    }
    while (root->left) {
        root = root->left;
    }
    return root;
}

Node* largestNode(Node* root)
{
    if (root == NULL) {
        return NULL;
    }
    while (root->right) {
        root = root->right;
    }
    return root;
}

int countNodes(Node* root)
{
    if (!root) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

Node* successor(Node* root, Node* node)
{
    if (node->right)
        return smallestNode(node->right);

    Node* successor = NULL;
    while (root != NULL) {
        if (strcasecmp(node->str, root->str) < 0) {
            successor = root;
            root = root->left;
        }
        else if (strcasecmp(node->str, root->str) > 0)
            root = root->right;
        else
            break;
    }

    return successor;
}

Node* predecessor(Node* root, Node* node)
{
    if (node->left)
        return largestNode(node->left);

    Node* predecessor = NULL;
    while (root != NULL) {
        if (strcasecmp(node->str, root->str) > 0) { 
            predecessor = root;
            root = root->right;
        }
        else if (strcasecmp(node->str, root->str) < 0)
            root = root->left;
        else
            break;
    }

    return predecessor;
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = int_max(height(y->left), height(y->right)) + 1;
    x->height = int_max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = int_max(height(x->left), height(x->right)) + 1;
    y->height = int_max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node *insert(Node *root, char* str)
{
    if (root == NULL)
        return newNode(str);

    if (strcasecmp(str, root->str) < 0)
        root->left = insert(root->left, str);
    else if (strcasecmp(str,root->str) > 0)
        root->right = insert(root->right, str);

    root->height = 1 + int_max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && strcasecmp(str, root->left->str) < 0)
        return rightRotate(root);

    if (balance < -1 && strcasecmp(str, root->right->str) > 0)
        return leftRotate(root);

    if (balance > 1 && strcasecmp(str, root->left->str) > 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && strcasecmp(str, root->right->str) < 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void freeTree(Node *root)
{
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->str);
    free(root);
}

Node* search(Node *root,char *word)
{
    if(root == NULL)
        return NULL;
    else if(strcasecmp(word,root->str) == 0){
        return root;
    }
    else if(strcasecmp(word,root->str) < 0)
    {
        if(root->left == NULL)
            return root;
        else
            return search(root->left,word);
    }
    else
    {
        if(root->right == NULL)
            return root;
        else
            return search(root->right,word);
    }
}

char* get_string(FILE* fptr)
{
    char* str = malloc(INPUT_SIZE+2); 

    char* error_test = fgets(str, INPUT_SIZE+2, fptr);

    if (error_test == NULL){
            free(str); 
            return NULL;
    }

    str[strcspn(str, "\n")] = '\0'; 

    if (str[0] == '\0'){
        free(str);
        return NULL;
    }

    return str;
}

Node* getDictionary(FILE* fptr)
{
    char* str = get_string(fptr);

    if (str == NULL) {
        return NULL;
    }
    Node* root = newNode(str);

    while ((str = get_string(fptr)) != NULL){
        root = insert(root, str);
    }

    return root;
}

void sentenceChecker(Node* dict)
{
    char* toFreeStr = get_string(stdin);
    if(!toFreeStr){ 
        puts("Empty Input\n");
        return;
    }

    char* str = toFreeStr; 
    char* word = strtok(str, " \n"); 

    while(word){
        Node* intree = search(dict, word); 
        if(!intree){
            printf("%s - NOT FOUND\n", word);
        }
        else if(strcasecmp(intree->str, word) == 0){
            printf("%s - CORRECT\n", word);
        }
        else{
            Node *pre = predecessor(dict, intree), *post = successor(dict, intree);
            printf("%s - INCORRECT, Suggestions:", word);
            if(intree) printf(" %s", intree->str);
            if(post) printf(" %s", post->str);
            if(pre) printf(" %s", pre->str);
            printf("\n");
        }
        word = strtok(NULL, " \n"); 
    }
    free(toFreeStr);
}

