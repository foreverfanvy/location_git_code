#include <stdio.h>
#include <stdlib.h>
//define what is the ordered list
typedef struct vector{
    int count;
    int size;
    int *data;
}vector;

//to make an order list
vector *newList(int n){
    vector *p=(vector *)malloc(sizeof(vector));
    //create the space for the vector
    p->size=n;
    p->count=0;
    int *q=(int *)malloc(sizeof(int)*n);
    p->data=q;
    //create the space for the data_space
    return p;
}

//to insert an number into the list
int insert(vector *v,int pos,int val){
    //the temp must releated to the position and value
    if(v==NULL)return 0;
    //firstly the vector can't be NULL,as it can not insert any data
    else if(pos<0||pos>v->count)return 0;
    //to obey the define of the ordered list
    else if(v->size==v->count) {
        //for the case that the room that we give is not enough
        expand(v);}
        //move all data behind the postion 1 Byte more
    for(int i=v->count;i>=pos;i--){
        v->data[i+1]=v->data[i];  
        //to move
        }
        //insert data
    v->data[pos]=val;
    v->count+=1;
    return 1;
}

//to free the RAM
void clear(vector *v){
    if(v==NULL)return;
    free(v->data);
    free(v);
    return;
}

//to delete the data of the ordered list
// 1 means it is well run
// 0 means it is wrong 
int earse(vector *v,int pos){
    if(pos>v->count||pos<0)return 0;
    for(int i=pos;i<v->count-1;i++){
        v->data[i]=v->data[i+1];
    }
    v->count-=1;
    return 1;
}

//to expand the space of the data area
int expand(vector *v){
    if(v==NULL)return 0;
    //NULL can not be expand
    vector *p=realloc(v->data,sizeof(v->size)*2);
    //To expand the memory two multiple size of it used to be
    if(p==NULL)return 0;
    return 1;
}


int main(void){
    vector *v = newList(5);
    insert(v, 0, 10);
    insert(v, 1, 20);
    insert(v, 2, 30);
    insert(v, 1, 15); // Inserting 15 at position 1

    for (int i = 0; i < v->count; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");

    earse(v, 1); // Deleting element at position 1

    for (int i = 0; i < v->count; i++) {
        printf("%d ", v->data[i]);
    }
    printf("\n");

    clear(v);
    return 0;

  
}

//编码出现的问题
/*
    1.newList函数操作缺少返回return p操作
    2.clear 函数中应该用 == 比较而不是 =。
    3.expand 函数的内存重新分配存在错误，应使用 realloc 重新分配 data 指针，而不是 vector 本身。
    4.realloc 需要判断是否成功分配内存。
    5.else if(v->size==v->count)没有判断，顺序表装满的情况没有考虑
    6.for(int i=pos;i<v->count-1;i++)边界条件是count-1不是count
*/