#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    struct node *leftmost_child;
    struct node *right_sibling;
    int label;
} node;

typedef struct tree{
    node *root;
}tree;

typedef struct snode{
    //stack implementation (with linked lists)
    //to be used in non recursive post order
    node *n;
    struct snode *next;
    struct snode *prev;
}snode;

//node operations
node *make_node(int label); //makes a new node and give it value 'label'
tree makeroot(struct node *n, struct tree *t); //makes node n the root of a new tree with one subtree t
void attach_node(struct node *i, struct node *n); //attaches node i to n, making i a child of n
node *parent(struct node *n, struct node *ancestor); //gets the parent of node n
node *leftmost_child(struct node *n); //gets the leftmost child of node n
node *right_sibling(struct node *n); //gets the right sibling of a node n
void rec_postorder_node(struct node* n); //prints n and all its decendants in recursive post order
void postorder_node(struct node* n); //prints n and all its decendant in non-recursive post order
node *search_node(int label, struct node* a); //prints n and all its decendant in non-recursive post order
void detach_node(struct node *i, struct node *n); //detaches node i and its decendants from n
void destroy_node(struct node *n); //destroys a node and all its decendants NB detach it first

//tree operations
tree *maketree(struct node * n); //makes a new tree using node i as the root (by just puting a tree wrapper around i)
void attach_tree(struct tree *s, struct tree *t); //attaches a subtree s into tree t
node *root_tree(struct tree *t); //returns the root node of a tree
void rec_postorder_tree(struct tree *t); //prints n and all its decendants in recursive post order
void postorder_tree(struct tree *t); //prints n and all its decendants in non-recursive post order
node *search_tree(int label, struct tree* t); //prints n and all its decendant in non-recursive post order
void detach_tree(struct tree *s, struct tree *t);//detaches subtree s from tree t
void makenull_tree(struct  tree *t); //destroys all nodes in t making root null

//stack operations
snode *push(node *n, struct snode *stack);
snode *pop();


int main(){
    srand (time(NULL));

    //building a random tree: height=2, and random(2 - 4)children per node
    int l = 0; //label
    //create a root node
    struct node *r = make_node(l++);
    //make r root of new tree mytree
    struct tree mytree = makeroot(r, NULL);
    printf("\n\nbuilding a random tree: height=2, and random(2 - 5)children per node\n\n\n");
    printf("    %d\n", r->label);
    int i;
    for (i=0; i<2+rand()%4; i++){
        //making children nodes and attaching to root
        struct node *child = make_node(l++);
        attach_node(child, r);
        printf("    |\n     ----%d\n", child->label);
        int j;
        for (j=0; j< 2+rand()%4; j++){
            //making grandchildren
            struct node *g_child = make_node(l++);
            attach_node(g_child, child);
            printf("    |    |\n          ----%d\n", g_child->label);
        }
    }

    //testing parent fuction
    printf("\n\nTesting Parent function with a randomly selected node");
    struct node *testgch = make_node(rand()%l);
    struct node *p = parent(testgch, mytree.root);
    if (p!=NULL){
        printf("\ncalculated_parent of %d is %d; \n", testgch->label, p->label);
    }
    else{
        printf("calculated parent of %d returned NULL\n", testgch->label);
    }
    
    //testing the recursive postorder function
    printf("\n\ntesting the recursive postorder function\n");
    rec_postorder_node(mytree.root);
    printf("\n\n");

    //testing the non recursive postorder function
    printf("\n\ntesting the non recursive postorder function\n");
    postorder_node(mytree.root);
    printf("\n\n\n\n");
    
}


//node operations implementation
node *make_node(int label){
    //returns a node pointer: makes a new node, with label value, 
    //and NULL leftmost_child and right siblings
    struct node* n = (struct node*) malloc(sizeof(struct node));
    n->label = label;
    n->leftmost_child = NULL;
    n->right_sibling = NULL;
    return n;
}

tree makeroot(struct node *n, struct tree *t){
    //returns a new tree n_t: makes node n the root of a new tree with one subtree t
    struct tree n_t = {0};
    n_t.root = n;
    if(t != NULL){
        attach_tree(t, &n_t);
    }
    return n_t;
}

void attach_node(struct node *i, struct node *n){
    //inserts node i to be a child of node n
    //printf("trying to add child %d to %d\n", i->label, n->label);
    if(n->leftmost_child == NULL){
        n->leftmost_child = i;
        //printf("added %d to leftmost child of %d\n", i->label, n->label);
    }
    else{
        //printf("leftchild of %d is %d\n", n->label, n->leftmost_child->label);
        struct node *curr = n->leftmost_child;
        while (curr->right_sibling != NULL){
            curr = curr->right_sibling;
            //printf("new curr (right sibling) = %d\n", curr->label);
        }
        curr->right_sibling = i;
        //printf("added %d as right sibling of %d\n", i->label, curr->label);
    }
}

node *parent(struct node *n, struct node *ancestor){
    //this returns the parent of a node n by doing a search
    //through the tree recursivly. NB The highest ancestor is the root
    //printf("new call\n");
    if (ancestor == NULL){
        //ancestor must exist on n's path to root to be able to find parent
        //printf("ancestor is NULL - unacceptable option\n");
        return NULL;
    }
    if (ancestor->label == n->label){
        //if n is the ancestor we assume the ancestor is root
        //parent of root is NULL
        //printf("node is at ancestor\n");
        return NULL;
    }
    //printf("here\n");
    struct node *curr = ancestor->leftmost_child;
    //printf("leftchild = %d", curr->label);
    struct node *p = NULL;
    while (curr != NULL){
        //printf ("new curr is %d\n", curr->label);
        if (curr->label == n->label){
            //printf ("node n is found at %d\n", curr->label);
            return ancestor;
        }
        p = parent(n, curr);
        if (p != NULL)
            return p;
        else
            curr = curr->right_sibling;
    }
    return NULL;
}

node *leftmost_child(struct node *n){
    return n->leftmost_child;
}

node *right_sibling(struct node *n){
    return n->right_sibling;
}

void rec_postorder_node(struct node* n){
    if (n == NULL){
        return;
    }
    struct node *curr = n->leftmost_child;
    while (curr != NULL){
        rec_postorder_node(curr);
        //printf ("new curr is %d\n", curr->label);
        curr = curr->right_sibling;
    }
    printf ("%d, ", n->label);
    return;
}

void postorder_node(struct node* n){
    //prints n and all its decendant in non-recursive post order
    if (n == NULL){
        return;
    }
    snode *my_stack1 = NULL;
    snode *my_stack2 = NULL;
    my_stack1 = push(n, my_stack1);
    while(my_stack1 != NULL){//while stack is not empty
        struct node *p = my_stack1->n;
        my_stack1 = pop(my_stack1);
        my_stack2 = push(p, my_stack2);
        
        struct node *curr = p->leftmost_child;
        while (curr != NULL){
            // save current state to stack here
            
            my_stack1 = push(curr, my_stack1);
            curr = curr->right_sibling;
        }
    }
    printf("getting info from stack\n");
    while(my_stack2 != NULL){
        printf("%d, ",my_stack2->n->label);
        my_stack2 = pop(my_stack2);
    }
}

node* search_node( int label, struct node* a){
    if (a == NULL){
        //ancestor must exist
        return NULL;
    }
    if (a->label == label){
        //if label is found at a
        return a;
    }
    //printf("here\n");
    struct node *curr = a->leftmost_child;
    //printf("leftchild = %d", curr->label);
    struct node *p = NULL;
    while (curr != NULL){
        //printf ("new curr is %d\n", curr->label);
        if (label == a->label){
            //printf ("node n is found at %d\n", curr->label);
            return a;
        }
        p = search_node(label, curr);
        if (p != NULL)
            return p;
        else
            curr = curr->right_sibling;
    }
    return NULL;
}

void detach_node(struct node *n, struct node *p){
    //detaches node n and its decendants from its node parent node p
    if (p->leftmost_child->label == n->label){
        p->leftmost_child = p->leftmost_child->right_sibling;
    }
    else{
        struct node *curr = p->leftmost_child;
        while (curr != NULL){
            if (curr->right_sibling->label == n->label){
                curr->right_sibling = curr->right_sibling->right_sibling;
            }
        }
    }
    
}

void makenull_node(struct node *n){
    //this destroys node n and all its descendants
    //please ensure that the node has been detached from its parent
    if (n == NULL){
        return;
    }
    else{
        makenull_node(n->right_sibling);
        makenull_node(n->leftmost_child);
        free (n);
        return;
    }
}


//tree operations implementation
node *root_tree(struct tree *t)
{
    //returns the root node of a tree
    return t->root;
} 

tree *maketree(struct node * n){
    //makes a new tree using node i as the root 
    //(by just puting a tree wrapper around i)
    //turns the node and its descendants to a subtree
    struct tree *t;
    t = (struct tree *)malloc(sizeof(struct tree));
    t->root = n;
    return t;
}

void attach_tree(struct tree *s, struct tree *t){
    //attaches the tree s as a subtree of t's root
    struct node *s_root = s->root;
    struct node *t_root = t->root;
    if (t_root != NULL){
        attach_node(s_root, t_root);
    }
    else{
        t->root = s->root;
    }
}

void rec_postorder_tree(struct tree *t)
{
    rec_postorder_node(t->root);
    //prints n and all its decendants in recursive post order
}

void postorder_tree(struct tree *t){
    //prints n and all its decendants in non-recursive post order
    postorder_node(t->root);
}

void detach_tree(struct tree *s, struct tree *t){
    //detaches subtree s from tree t
    struct node *p = parent(s->root, t->root);
    detach_node(s->root, p);
}

void makenull_tree(struct  tree *t){
    makenull_node(t->root);
    t->root=NULL;
}

node *search_tree(int label, struct tree* t){
    return (search_node(label, t->root));
}



//stack operations
snode *push (node *new, struct snode *my_stack)
{
    struct snode *newNode;
    newNode = (struct snode *)malloc(sizeof(struct snode));
    newNode->n = new;
    newNode->prev = NULL; 
    newNode->next = my_stack;
    my_stack = newNode;
    //printf("%d was pushed onto the stack.\n", first->data);
    return my_stack;
}

snode *pop(snode *my_stack)
{
    struct snode *prevPtr;
    if (my_stack == NULL) // check if stack is empty
    {
        printf("Error: Stack Empty.\n");
        return NULL; 
    }
    //printf("%d was popped off the stack.\n", first->data);
    prevPtr = my_stack;
    my_stack = my_stack->next;
    free(prevPtr);
    return(my_stack);
}