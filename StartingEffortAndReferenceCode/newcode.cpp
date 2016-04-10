#include<stdlib.h>
#include<stdio.h>
#include<conio.h>

struct tree_el {
   int val;
   struct tree_el * right, * left;
};// define a node to make tree

typedef struct tree_el node; // typedef it
// for making a tree
void insert(node ** tree, node * item) { //
   if(!(*tree)) {
      *tree = item;
      return;
   }
   if(item->val<(*tree)->val)
      insert(&(*tree)->left, item);
   else if(item->val>(*tree)->val)
      insert(&(*tree)->right, item);
}
//printing values of tree
void printout(node * tree) {
   if(tree->left) printout(tree->left);
   printf("%d\n",tree->val);
   if(tree->right) printout(tree->right);
}

//find height 
int maxDepth(node* root) 
{
   if (root==NULL) 
       return 0;
   else 
   {
       /* compute the depth of each subtree */
       int lDepth = maxDepth(root->left);
       int rDepth = maxDepth(root->right);

       /* use the larger one */
       if (lDepth > rDepth) 
           return(lDepth+1);
       else return(rDepth+1);
   }
} 
int max(int i,int j)
{
if(i>j)
return i;
return j;

}
int max(int i,int j,int k)
{
if(i>j && i>k)
return i;
else if(j>i && j>k)

return j;
return k;

}

int diameter2(node * t, int* height)  
{  
    int lh = 0, rh = 0;  
    int leftD = 0, rightD = 0;  
   
    if(t == NULL)  
    {  
        *height = 0;  
        return 0; /* diameter is also 0 */  
    }  
   
    leftD = diameter2(t->left, &lh);  
    rightD = diameter2(t->right,&rh);  
   
    /* Height of current node is max of heights of left and 
    right subtrees plus 1*/  
    *height = max(lh, rh) + 1;  
   
    return max(lh + rh + 1, leftD, rightD);  
}  
void main() {
   node * curr, * root;
   int i,height,diameter;

   root = NULL;

   for(i=1;i<=10;i++) {
      curr = (node *)malloc(sizeof(node));
      curr->left = curr->right = NULL;
      curr->val = rand();// for making random valued tree
      insert(&root, curr);
   }

   printout(root);
	height =maxDepth(root);
	printf("height is %d\n",height);

	diameter=diameter2(root,&height);
	printf("diameter is %d",diameter);
	getch();
}