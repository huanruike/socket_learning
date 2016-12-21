#include<bits/stdc++.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;

const int n=5;
char str[30][120];
pthread_t th[30];

void init_str(){
	for(int i=1;i<=n;i++){
		str[i][0]='.';
		str[i][1]='/';
		str[i][2]='\0';
		strcat(str[i],"S/server");
		int len=strlen(str[i]);
		str[i][len++]=(i+'0');
		str[i][len]='\0';
	}
}

void *f(void *str){
	char *s=(char*)str;
	system(s);
}

int main(){
	init_str();
	for(int i=1;i<=n;i++){
		pthread_create(&th[i],NULL,f,(void*)(str[i]));
	}
	for(int i=1;i<=n;i++) pthread_join(th[i],NULL);
	return 0;
}
