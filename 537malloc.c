#include<stdlib.h>
#include<stdio.h>
#include "537malloc.h"
#include "range_tree.h"

/*This is a c library including functions malloc537, free537, memcheck537 and realloc537.*/

static RBRoot *root;
static int counter = 0;
static int freeSignal = 1; //1 when free537; 0 when realloc537


/*
In addition to actually allocating the memory by calling malloc(), this function will record a tuple (addri, leni), for the memory that you allocate in the heap. (If the allocated memory was previously freed, this will be a bit more complicated.) You will get the starting address, addri, from the return value from malloc() and the length, leni, from the size parameter. You can check the size parameter for zero length (this is not actually an error, but unusual enough that it is worth reporting).

*/
void *malloc537(int size){
    Node* ret;
    if (size < 0) {
        fprintf(stderr,"Error: cannot assign negative size");
	exit(-1);
    }
    if (size == 0) {
	printf("Warning! Trying to malloc 0 size!\n");
    }
    //initialize the root of the tree is malloc is first called
    if(!counter){
        root = create_rbtree();
        counter = 1;    
    }
    
    //exit if malloc failed
    if ((ret = insert_rbtree(root, size, NULL)) == NULL){
        fprintf(stderr, "Error: No space for malloc\n");
        exit(-1);
    }
    //return the starting addr for the malloc block
    else{ 
        return ret->start;
    }
}

/*
This function will first check to make sure that freeing the memory specified by ptr makes sense, then will call free() to do the actual free. Some of the error conditions that you should check for include:
Freeing memory that has not be allocated with malloc537().
Freeing memory that is not the first byte of the range of memory that was allocated.
Freeing memory that was previously freed (double free).
When an error is detected, then print out a detailed and informative error message and exit the program (with a -1 status). If all checks pass,then this function indicates that the tuple for addr = ptr is no longer allocated, and then calls free().

*/
void free537(void *ptr){
    //exit if NULL pointer passed in
    if (ptr == NULL){
        fprintf(stderr, "Error: cannot free NULL pointer\n");
        exit(-1);
    }
    Node* node = root->node;
   
    //search the tree
    while(node != NULL){
    	//if find the node we need to free 
        if (ptr == node->start){
        	//Double free occures
        	if (node->freeFlag == 0){
        		fprintf(stderr, "Error: Freeing memory that was previously "
        			"freed (double free)\n");
        	    exit(-1);
        	}
        	//set freeFlag of the node to 0 
        	else{
        		node->freeFlag = 0;
			if (freeSignal == 1)                	
				free(ptr);
        		return;
        	}
     	}
     	
        //otherwise keep searching the tree
     	else{
     		//target free node's addr is lower than current node
        	if(ptr < node->start){
        		node = node->left;
        	}
        	//target free node's addr is higher than current node
        	else if(ptr > (node->start + node->size -1)){
        		node = node->right;
        	}
        	//target free node's addr is within current node 
        	else{
        		fprintf(stderr, "Error: Freeing memory is not the first byte "
        		 "of the range of memory that was allocated.\n");
        		exit(-1);
        	}
     	}
   	}

    //serach the whole tree but cannot find target free addr
    	fprintf(stderr, "Error: Freeing memory that has not be allocated" 
    		" with malloc537().\n");
        exit(-1);

}


/*
If ptr is NULL,then this follows the specification of malloc537() above. If size is zero and ptr is not NULL,then this follows the specification of free537() above. Otherwise, in addition to changing the memory allocation by calling realloc(), this function will first check to see if there was a tuple for the (addr = ptr, and removes that tuple, then adds a new one where addr is the return value from realloc() and len is size

*/

void *realloc537(void *ptr, int size){
    if (ptr == NULL) {
        return malloc537(size);
    }
    else {
	freeSignal = 0;
        free537(ptr);
	freeSignal = 1;
        if (size == 0) {
	    printf("Warning! Trying to realloc 0 size! \n");
	}
  	void *a = realloc(ptr, size);
	return insert_rbtree(root, size, a)->start;
    }
}


/*
This function checks to see the address range specified by address ptr and length size are fully within a range allocated by malloc537() and memory not yet freed by free537(). When an error is detected, then print out a detailed and informative error message and exit the program (with a -1 status). 
*/
void memcheck537(void *ptr, int size){
    //signal if ptr is found
    int success = 0;
    //exit if NULL pointer passed in
    if (ptr == NULL){
        fprintf(stderr, "Error: cannot check memory space for NULL pointer\n");
        exit(-1);
    }
    Node* node = root->node;
    //search the whole tree
    while(node!=NULL){
        //target addr is smaller than current block's starting addr
        if (ptr < node->start){
            node = node->left;
        }
        //target addr is bigger than current block's ending addr
        else if (ptr > (node->start + node->size -1)){
            node = node->right;
        }
        //target addr is within the current block's addr 
        else{
            //ptr is found within the allocated block
            success = 1;
            //error if the current block has been freed
            if (node->freeFlag == 0){
                fprintf(stderr, "Error: The checking memory space has been freed\n");
                exit(-1);
            }else{  //determine if the size of searching mem is larger than current block's ending addr
                if ((ptr + size -1) > (node->start + node->size -1)){
                    fprintf(stderr,"Error: The checking memory space beyond the boundry\n");
                    exit(-1);
                }
                else{ 
                    printf("The checking memory space has been allocated\n");
                    break;
                }
            }
        }
    }

    //serach the whole tree but cannot find checking memory space
    if (!success){
       fprintf(stderr, "Error: The checking memory space has not been allocated\n");
       exit(-1);
    }

}





void printEverything(){
    print_rbtree(root);
}   
