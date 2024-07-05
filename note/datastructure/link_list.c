#include <stdio.h>
#include <stdlib.h>

//define the linklist structure
typedef struct Node
{
    int val;
    Node *next;
}Node;
 
//make the basic Node of the linklist
Node *newNode(int val){
    Node *p=(Node *)malloc(sizeof(Node));
    p->val=val;
    p->next=NULL;
    return p;
}

//free the memory of the linklist
int clear(Node *head){
    for(Node *p=head,*q;;p=q){
    q=p->next;
    free(p);
    }
//1 means the function is well prograssed
    return 1;
}

//A linklist to insert an element
Node *insert(Node *head, int pos, int val){
//create a new node and give the value to it
    Node *new;
    new->val=val;
    new->next=NULL;
//create p=head to get the right position
    Node *p=head;
    for(int i=0;i<pos;i++)p=p->next;
//link to the behind var
    new->next=p;

}