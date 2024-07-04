#include <stdio.h>
#include <stdlib.h>
//define what is the ordered list
typedef vector{
    int count;
    int size;
    int *data;
}vector;

//to make an order list
vector *newList(int n){
    vector *p=(vector *)malloc(sizeof(vector));
    //create the space for the vector
    p-size=n;
    p->count=0;
    int *q=(int *)malloc(sizeof(int)*size);
    p->data=q;
    //create the space for the data_space

}

//to insert an number into the list
int insert(vector *v,int pos,int val){
    //the temp must releated to the position and value
    if(v==NULL)return false;
    //firstly the vector can't be NULL,as it can not insert any data
    else if(pos>count)return false;
    //to obey the define of the ordered list
    else {
        //move all data behind the postion 1 Byte more
        for(int i=v->count;i>=pos;i--){
            v->data[i+1]=v->data[i];  
            //to move
        }
        //insert data
        v->data[pos]=val;
        v->count+=1;
        return true;
        }
}

//to free the RAM
void clear(vector *v){
    if(v=NULL)return;
    free(v->data);
    free(v);
    return;
}

//to delete the data of the ordered list
int earse(vector *v,int pos){
    if(pos>v->count||pos<0)return false;
    for(int i=pos;i<v->count;i++){
        v->data[i]=v->data[i+1];
    }
    v->count-=1;
    return true;
}

//to expand the space of the data area



int main(void){



    return 0;
}