#ifndef range_tree_h
#define range_tree_h

#define RED        0    // color is 0 if color is red
#define BLACK    1    // color is 1 if color is black


// Define Treenode
typedef struct RBTreeNode{
    unsigned char color;        // color is Red or Black
    int   size;                    // address size
    struct RBTreeNode *left;    // left children
    struct RBTreeNode *right;    // right children
    struct RBTreeNode *parent;    // parent
	void *start;					
    int freeFlag;


}Node, *RBTree;

// Define RB Tree
typedef struct rb_root{
    Node* node;
}RBRoot;


/*Define a data structure called deletelist which is to
 store all free but overlap tree nodes with the pending added treenode*/
typedef struct delete1{
    int cap;
    int size;
    Node **list;
}delist;


// create RB Tree
RBRoot* create_rbtree();

// Insert start pointer and address size as a node to the RB Tree
Node* insert_rbtree(RBRoot *root, int size, void* ptr);

// delete node with start pointer ptr
void delete_rbtree(RBRoot *root, void *ptr);

// search RB Tree's treenode which has start pointer ptr
int rbtree_search(RBRoot *root, void *ptr);

// print RB Tree
void print_rbtree(RBRoot *root);

#endif
