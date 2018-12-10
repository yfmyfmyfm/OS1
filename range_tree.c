/**
 * Implementing Red-Black Tree by C
 * The basic building steps of Red-BLack Tree is borrow from skywang 
 * who generated the RedBlackTree library at https://www.cnblogs.com/skywang12345/p/3624177.html on 2013/11/18
 * Yifan and Siyuan adds some codes to make sure the basic red black tree library works 
 * for current oproject 4 
 */

#include <stdio.h>
#include <stdlib.h>
#include "range_tree.h"

/*define some RB Tree basic contents at the top*/
#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do {if (r) (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do {if (r) (r)->color = RED; } while (0)
#define rb_set_parent(r,p)  do { if (r) (r)->parent = (p); } while (0)
#define rb_set_color(r,c)  do { if (r) (r)->color = (c); } while (0)
//define a buffer size that we will use as the initialized capacity of delete list where we store the nodes needed to be deleted
#define buffersize 32

/*Function: 
 * create a Red Black Tree and Return the root of a Red Black Tree
 */
RBRoot* create_rbtree()
{
    RBRoot *root = (RBRoot *)malloc(sizeof(RBRoot));
    root->node = NULL;
    return root;
}


/* Function:
 * Use recursion to search the RB Tree and find the node with start address, ptr
 * 
 * Parameters:
 * x		the RB Tree
 * ptr		the pointer of the node getting search
 */
static Node* search(RBTree x, void *ptr)
{
    if (x==NULL || x->start==ptr)
        return x;

    if (ptr < x->start)
        return search(x->left, ptr);
    else
        return search(x->right, ptr);
}

int rbtree_search(RBRoot *root, void *ptr)
{
    if (root)
        return search(root->node, ptr)? 0 : -1;
    return -1;
}



/* Function: 
 * Do left rotation to RB Tree
 *
 * Here is how left rotation works for node x：
 *      px                              px
 *     /                               /
 *    x                               y                
 *   /  \     -(left rotation)->     / \               
 *  lx   y                          x  ry     
 *     /   \                       /  \
 *    ly   ry                     lx  ly  
 *
 * Parameters:
 * root:   RB tree
 * x:	   node getting to be left rotate
 */
static void rbtree_left_rotate(RBRoot *root, Node *x)
{
    // let x's right to be y
    Node *y = x->right;

    // set y's left to be x's right
    // if y's left exists, x to be y's left's new parent
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // let x's parent to be y's parent
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        root->node = y;            // if x does not have a patent, then y would be the root node
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    // if x is the left child of its parent, let y be x's parent's left child
        else
            x->parent->right = y;    // if x is the right child of its parent, let y be x's parent's right child
    }
    
    // let y's left to be x
    y->left = x;
    // let x's parent to be y
    x->parent = y;
}

/* Function:
 * Do right rotation to RB Tree
 *
 * Here is how right rotation works for node x：
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \    -(right rotation)->      /  \                    
 *        x   ry                           lx   y  
 *       / \                                   / \                  
 *      lx  rx                                rx  ry
 * Parameters:
 * root:   RB tree
 * x:	   node getting to be left rotate
 */
static void rbtree_right_rotate(RBRoot *root, Node *y)
{
    // set x to be y's left
    Node *x = y->left;

    // set x's right to be y's left
    //if x's right exists, set x's right's parent to be y
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // set y's parent to be x's parent
    x->parent = y->parent;

    if (y->parent == NULL) 
    {
        root->node = x;            // if y's parent does not exist, set x to be the root node
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    // if y is its parent's right, set y's parent's right to be x
        else
            y->parent->left = x;    // set y's parent's left to be x
    }

    // set x's right to be y
    x->right = y;

    // set y's parent to be x
    y->parent = x;
}

/* Function:
 * Fixup function for the deletion of RB Tree
 *
 * After RB tree finish its deletion, and its balance cannot keep, 
 * call this function to fix the balance so that we reget a RB Tree
 *
 * Parameters：
 *     root   RB Tree's root
 *     node   node waiting to be fixed
 */
static void rbtree_delete_fixup(RBRoot *root, Node *node, Node *parent)
{
    Node *other;

    while ((!node || rb_is_black(node)) && node != root->node)
    {
        if (parent->left == node)
        {
            other = parent->right;
            if (rb_is_red(other))
            {
                // Case 1: x's sibling is red  
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_left_rotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: x's sibling + x's newphews are black 
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->right || rb_is_black(other->right))
                {
                    // Case 3: x's sibling is black, x's left newphew is red, x's right newphew is black  
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rbtree_right_rotate(root, other);
                    other = parent->right;
                }
                // Case 4: x's sibling is black, x's right newphew is red
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                rbtree_left_rotate(root, parent);
                node = root->node;
                break;
            }
        }
        else
        {
            other = parent->left;
            if (rb_is_red(other))
            {
                // Case 1: x's sibling is red 
                rb_set_black(other);
                rb_set_red(parent);
                rbtree_right_rotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right)))
            {
                // Case 2: Case 2: x's sibling + x's newphews are black 
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!other->left || rb_is_black(other->left))
                {
                    // Case 3:x's sibling is black, x's left newphew is red, x's right newphew is black   
                    rb_set_black(other->right);
                    rb_set_red(other);
                    rbtree_left_rotate(root, other);
                    other = parent->left;
                }
                // Case 4:x's sibling is black, x's right newphew is red
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rbtree_right_rotate(root, parent);
                node = root->node;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/* Function:
 * Deleting a node from RB Tree
 *
 * Parameters：
 *     tree 	RB Tree
 *     node 	node waiting to be deleted
 */
void rbtree_delete(RBRoot *root, Node *node)
{
    Node *child, *parent;
    int color;

    // if node has 2 children
    if ( (node->left!=NULL) && (node->right!=NULL) ) 
    {
        Node *replace = node;

        // get subcessor(deleting node's right)
	//use subcessor to replace the deleting node's position
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // if node is not root
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        } 
        else 
            // if node is root node, replace root node
            root->node = replace;

        // change subcessor's right. Subcessor cannot have a left
        child = replace->right;
        parent = rb_parent(replace);
        // save subsessor's color
        color = rb_color(replace);

        // if deleting node is parent to subcessor
        if (parent == node)
        {
            parent = replace;
        } 
        else
        {
            // child exists
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            rbtree_delete_fixup(root, child, parent);
        free(node);

        return ;
    }

    if (node->left !=NULL)
        child = node->left;
    else 
        child = node->right;

    parent = node->parent;
    // save color of new node
    color = node->color;

    if (child)
        child->parent = parent;

    // if node is not root node
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root->node = child;

    if (color == BLACK)
        rbtree_delete_fixup(root, child, parent);
    free(node);
}

/* Function:
 * deleting the node with start pointer ptr
 *
 * Parameters：
 *     tree 	RB Tree
 *     ptr	start address of node waiting to be deleted 
 */
void delete_rbtree(RBRoot *root, void *ptr)
{
    Node *z; 

    if ((z = search(root->node, ptr)) != NULL)
        rbtree_delete(root, z);
}


/* Function:
 * fix function to fix color of RB tree.
 *
 *
 * Parameters：
 *     root	 RB Tree
 *     node	 node waiting to get inserted        
 */
static void rbtree_insert_fixup(RBRoot *root, Node *node)
{
    Node *parent, *gparent;

    // if father exists and father is red
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);
	
        //if father is left to grandpa
        if (parent == gparent->left)
        {
            // Case 1： uncle is red
            {
                Node *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2: uncle is black and current node is right of father
            if (parent->right == node)
            {
                Node *tmp;
                rbtree_left_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3: uncle is black and current node is the left of father
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_right_rotate(root, gparent);
        } 
        else//if father is right to grandpa
        {
            // Case 1: uncle is red
            {
                Node *uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2: uncle is black and current node is left of father
            if (parent->left == node)
            {
                Node *tmp;
                rbtree_right_rotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3: uncle is black and current node is right of father
            rb_set_black(parent);
            rb_set_red(gparent);
            rbtree_left_rotate(root, gparent);
        }
    }

    // set root color to be black
    rb_set_black(root->node);
}

/*Define a data structure called deletelist which is to
 store all free but overlap tree nodes with the pending added treenode*/
/*typedef struct deletelist{
    int cap;
    int size;
    Node **list;
}deletelist;*/

/*
 * Function: traverse the subtree that has overlap
 * Parameters: the potential deletelist which is to store all free but overlap nodes
 *             the treenode
 *             the pending added node
 */
static void nodeoverlap(Node *treenode, Node *newnode, delist *dl) {
    if ((treenode == NULL)) {
        return;
    }
    else {
////
    void *treestart = treenode->start;
    void *treeend = treestart + treenode->size - 1;
    void *newstart = newnode->start;
    void *newend = newstart + newnode->size - 1;
    //check whether there is any overlap between the pending add node and a treenode
    int overlap = 0;
    //if new's start is within tree's range
    if (newstart <= treestart && treestart <= newend) {
        overlap = 1;
    }
    //else tree's start is within new 's range
    else {
        if (newstart <= treeend && treeend <= newend) {
            overlap = 2;
        }
	else if ((treestart < newstart) && (newend <= treeend)) {
	    overlap = 3;
        }
    }
////
    if (overlap == 0){
	return;
    }else if (overlap != 1) {
    //if there is some overlap but the treenode is allocated, exit
	    if (treenode->freeFlag == 1) {
		fprintf(stderr, "Error: there is some overlap between address %p with length %d and address %p with length %d", treenode->start, treenode->size, newnode->start, newnode->size);
		exit(1);
	    }
	    else {
		//shrink the free node's size so that it do not have any overlap with pending node
		treenode->size = newnode->start-treenode->start;
		// if the new node is within the treenode range, we do not need to do anything, just return. Else, we need to keep searching 
		if (overlap == 2)
			nodeoverlap(treenode->right, newnode, dl);
		return;
	    }
	} else {//else corresponds to the case: overlap(treenode, newnode) == 1
	    //if there is some overlap but the treenode is allocated, exit
	    if (treenode->freeFlag == 1) {
		fprintf(stderr, "Error: there is some overlap between address %p with length %d and address %p with length %d", treenode->start, treenode->size, newnode->start, newnode->size);
		exit(1);
	    }
	    else {
		//if deletelist is full, double capacity
		if (dl->size >= dl->cap) {
		    dl->cap = 2*dl->cap;
		    dl->list = realloc(dl->list, dl->cap*sizeof(Node *));
		}
		//add overlap and free treenode to the delete list
		dl->list[dl->size] = treenode;
		dl->size++;
		nodeoverlap(treenode->left, newnode, dl);
		nodeoverlap(treenode->right, newnode, dl);
		return;
	    }
        }
    }
}


/*
 * Function：Add node into the rbtree
 *
 * Parameter Description：
 *     root root of the rbtree
 *     newNode node that wil be added into the rbtree
 */
static void rbtree_insert(RBRoot *root, Node *newNode)
{    
    //record whether newNode is left of treeNode, 0->right, 1->left
    int direction = 0;
    Node *treeNode = root->node;
    //record the parent node of treeNode
    Node *buffer = NULL;
    //get the start and end addr for newNode and root
    void * treeNodeStart;
    void * treeNodeEnd;
    void * newNodeStart;
    void * newNodeEnd;
    while (treeNode != NULL) {  
        treeNodeStart = treeNode->start;
        treeNodeEnd = treeNodeStart + treeNode->size - 1;
	newNodeStart = newNode->start;
        newNodeEnd = newNodeStart + newNode->size - 1;
        buffer = treeNode;
        if (treeNodeStart > newNodeEnd) {
		//newNode go to treeNodeStart's left
                treeNode = treeNode->left;
                direction = 1;
        }
        else {
            if (treeNodeEnd < newNodeStart) {
		//newNode go to treeNodeStart's right
                treeNode = treeNode->right;
                direction = 0;
            }
            else {
                //need to check if the overlapped node has been freed
		delist *dl;
	        dl = (delist*)malloc(sizeof(delist));
		dl->cap = buffersize;
                dl->size = 0;
                dl->list = malloc(dl->cap*sizeof(Node*));
		//need to check if the overlapped node has been freed
                nodeoverlap(treeNode, newNode, dl);
                for (int i = 0; i < dl->size; i++) {
                    //delete overlap free nodes
                    //after each delete, the tree's colors will all change!
                    rbtree_delete(root, dl->list[i]);
                }
		free(dl->list);
		free(dl);
                //add new node to the tree after clean the overlap free nodes
                //if root is still here
                if (root != NULL) {
                    //re-run add new node function to add node
                    rbtree_insert(root, newNode);
                } else { //if all nodes on the tree have been deleted,new node should be new tree's root
                    newNode->color = BLACK;
		    root->node = newNode;
		    
                }
                return;
            }
        }
    }
    rb_parent(newNode) = buffer;

    //newNode will be the root if treeNode is NULL
    if (buffer != NULL){
        if (direction == 0) {
            buffer->right = newNode;
        }
        else {
            buffer->left = newNode;
        }
    }
    else{
        root->node = newNode;
    }

    // Set node color to RED
    newNode->color = RED;

    // fix-up the BST
    rbtree_insert_fixup(root, newNode);
    return;
}









/* FUnction:
 * create a node
 *
 * Parameters：
 *     size 	address size of the node
 *     ptr 	node's start address
 */
static Node* create_rbtree_node(int size, void* ptr)
{
    Node* p;

    if ((p = (Node *)malloc(sizeof(Node))) == NULL)
        return NULL;
    p->size = size;
    p->left = NULL;
    p->right = NULL;
    p->parent = NULL;
    p->color = RED; // initially set color of new node to be red
    if (ptr == NULL){
        p->start = malloc(size);  
    }else{
        p->start = ptr;
    }                 
    p->freeFlag = 1;

    return p;
}

/* FUnction:
 * INsert node into the RB Tree
 *
 * Parameters：
 *     root	RB Tree
 *     size 	address size of the node
 *     ptr 	node's start address
 */
Node* insert_rbtree(RBRoot *root, int size, void* ptr)
{
    Node *node;    // initiate new node
    // if create new node failed, return NULL
    if ((node=create_rbtree_node(size, ptr)) == NULL)
        return NULL;
    
    rbtree_insert(root, node);

    return node;
}



/* Function:
 * Print RB Tree
 *
 * Parameters:
 * tree       RB Tree
 * start       node's start pointer 
 * direction      0，print node itself
 *               -1，print node's left
 *                1，print node's right
 */
static void rbtree_print(RBTree tree, void* start, int direction)
{
    if(tree != NULL)
    {
        if(direction==0)    // print node itself
            printf("%2p(B) is root, freeFlag is %2d, size is %d \n", tree->start, tree->freeFlag, tree->size);
        else                // print its left/right
            printf("%2p(%s) is %2p's %6s child, freeFlag is %2d, size is %d\n", tree->start, rb_is_red(tree)?"R":"B", start, direction==1?"right" : "left", tree->freeFlag, tree->size);

        rbtree_print(tree->left, tree->start, -1);
        rbtree_print(tree->right,tree->start,  1);
    }
}

void print_rbtree(RBRoot *root)
{
    if (root!=NULL && root->node!=NULL)
        rbtree_print(root->node, root->node->start, 0);
}
