#include<bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<fcntl.h>

using namespace std;

#define SERVER_ADDRESS "10.181.149.200"
#define SERVER_PORT 8002
//服务器地址和端口

//pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;// 读,写 互斥
const int maxn=100010;
int sock;
char name[50];

pthread_t send_th,recv_th;

void getName(){
	printf("请输入你的用户名以确认进入: ");
	scanf("%s",name);
}

bool isBye(char *s){
	int len=strlen(s);
	return len>=2&&s[len-1]=='G'&&s[len-2]=='G';
}

void *send_f(void *arg){
	while(1){
//		pthread_mutex_lock(&mtx);
		char str[120];
		scanf("%s",str);
		char msg[120];msg[0]='\0';
		strcpy(msg,"#");
		strcat(msg,name);
		strcat(msg,": ");
		strcat(msg,str);
		send(sock,msg,sizeof(msg),0);
		if(isBye(msg)){
			pthread_cancel(recv_th);
			break;
		}
		sleep(1);
//		pthread_mutex_lock(&mtx);
	}
}

void *recv_f(void *arg){
	while(1){
//		pthread_mutex_lock(&mtx);
		char str[120];
		str[0]='\0';
		recv(sock,str,sizeof(str),0);
		if(strlen(str)==0) continue;
		if(str[0]!='#') continue;
		printf("%s\n",str);
		sleep(1);
//		pthread_mutex_unlock(&mtx);
	}
}

int main(){
	sock=socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(SERVER_ADDRESS);
	addr.sin_port=htons(SERVER_PORT);
	int cnt=0;
	system("clear");
	// 
	//int flags=fcntl(sock,F_GETFL,0);
	//fcntl(sock,F_SETFL,flags|O_NONBLOCK);
	while(connect(sock,(sockaddr*)&addr,sizeof(addr))==-1){
		if(cnt==0) printf("服务器瘫痪中...\n");
		else if(cnt==1) printf("服务器正在修复...\n");
		else printf("服务器再次瘫痪!!!\n");
		cnt++;cnt%=3;
		//sleep(2);
	}
	printf("连接成功\n");
	getName();
	system("clear");
	pthread_create(&send_th,NULL,send_f,NULL);
	pthread_create(&recv_th,NULL,recv_f,NULL);
	pthread_join(send_th,NULL);
	pthread_join(recv_th,NULL);
	close(sock);
	return 0;
}
