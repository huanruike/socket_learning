#include<bits/stdc++.h>

using namespace std;

void work(){
	printf("所有房间: \n");
	for(int i=1;i<=5;i++){
		printf("	room%d\n",i);
	}
	printf("请输入要进入的房间名称: ");
	char str[30];
	scanf("%s",str);
	char s[30]="./C/";
	strcat(s,str);
	system(s);
}

int main(){
	while(1) work();
	return 0;
}


