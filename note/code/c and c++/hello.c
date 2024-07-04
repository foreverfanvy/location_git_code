#include <stdio.h>
int num(int n,int *a);
int main(){
	int n;//n表示弹簧板数量
	int a[n];//数组储存
	scanf("%d",&n);
	for(int i=0;i<n;i++){
		scanf("%d",&a[i]);
	}//数据输入
	printf("%d",num(n,a));	  
}
int num(int n,int *a){
	//语义：n个弹簧板对应被弹起的次数
	int index=0,i=0;
	if(n==1)return 1;
	else while(index<=n){
				index+=a[index];//
				i++;
			}return i;
}