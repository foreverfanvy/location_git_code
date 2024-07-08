#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//定义Node结构体
typedef struct Node
{
    int val;
    struct Node *next;
}Node;
 
//初始化链表
Node *newNode(int val){
    Node *p=(Node *)malloc(sizeof(Node));
    p->val=val;
    p->next=NULL;
    return p;
}

//释放内存
int clear(Node *head){
    for(Node *p=head,*q;p;p=q){
    q=p->next;
    free(p);
    }
//1 means the function is well prograssed
    return 1;
}

//插入元素
Node *insert(Node *head, int pos, int val){
//can not insert an NULL address
    if(head==NULL&&pos!=0)return NULL;
    if(pos==0){
        Node *p=newNode(val);
        p->next=head;
        return p;
    }
//为了将新的节点p指向pos-1位置
    Node *p=head;
    for(int i=1;i<pos;i++)p=p->next;
//新建node，赋值
    Node *node=newNode(val);
//node中地址指向p后面的地址    
    node->next=p->next;
//再将p指向node
    p->next=node;
    return head;    
}

//输出链表
            /*输出链表格式
            0   1   2   3
            22->33->22->88*/
void output(Node *head){
    int len=0;
    for(Node *p=head;p;p=p->next){
        len++;
    }
    for(int i=0;i<len;i++){
        printf("%-5d",i);
    }
    printf("\n");
    for(Node *p=head;p;p=p->next){
        printf("%-3d->",p->val);
    }
    printf("\n\n");
    return;
}

//查找操作
int find(Node *head,int val){
    Node*p=head;
    int pos=0;
    if(head==NULL)return 0;
    //遍历查找
    while(p){
        if(p->val!=val){
            p=p->next;
            pos++;
        }
        else break;
    }
    if(p==NULL){
        printf("404\nNot found");
        return 0;
    }
    for(int i=0;i<pos;i++){
		printf("     ");
			}
		printf("^\n");
	for(int i=0;i<pos;i++){
		printf("     ");
			}
	    printf("|\n");
		printf("The %d is found at seq %d\n",val,pos);
		return 1;

}


//随机检测
int main(void){
    srand(time(0));
    #define MAX_OP 10
    Node *head=NULL;
    for(int i=0;i<MAX_OP;i++){
        int pos=rand()%(i+1),val=rand()%100;
        printf("insert %d at %d\n",val,pos);
        head=insert(head,pos,val);
        output(head);
    }
    int num;
    scanf("%d",&num);
    find(head,num);
    return 0;
}

/*
    1.clear函数中的不能写成死循环for(Node *p=head,*q;;p=q)中间加上p--for(Node *p=head,*q;p;p=q)当p为NULL（释放完成后退出）
    2.struct Node *next;不能写出Node *next；在结构体定义内部引用自身时，需要使用struct关键字
    3.不能int num=scanf("%d",&num);scanf的返回值是0或者1，所以会出现bug
    4.
*/