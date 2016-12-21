#include<bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>

using namespace std;

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8888
//服务器地址和端口

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;// 读,写,互斥
const int maxn=100010;
int sock;

queue<char*> str_q,sys_q;
pthread_t send_th,recv_th,sys_msg_th;//

///---声明
void Read(char*);
void Read(char*,int);
void Write(char*);
void Write(char*,int);
void *sys_msg_f(void*);
void print_fri();
void del_fri();
void add_fri();
void *send_f(void*);
void *recv_f(void*);
void chat();
void choose_fri();
void communicate();
void regist();
void sign();
void sign_or_regist();


///---

void Read(char *str){
	if(!str_q.empty()){
		strcpy(str,str_q.front());
		str_q.pop();
		return;
	}
	while(1){
		read(sock,str,sizeof(str));
		if(str[0]=='#'&&str[1]=='#') sys_q.push(str);
		else return;
	}
}

void Read(char *str,int op){
	if(!sys_q.empty()){
		strcpy(str,sys_q.front());
		sys_q.front();
		return;
	}
	while(1){
		read(sock,str,sizeof(str));
		if(str[0]!='#'||str[1]!='#') str_q.push(str);
		else return;
	}
}

void Write(char *str){
	write(sock,str,sizeof(str));
}

void Write(char *str,int op){
	write(sock,str,sizeof(str));
}

void print_fri(){
	printf("好友列表：\n");
	char Fri[maxn];
	Read(Fri);
	printf("%s",Fri);
}

void del_fri(){
	system("clear");
	print_fri();
	char name[50];
	printf("请输入要删除的好友用户名：");scanf("%s",name);
	Write(name);
	printf("删除成功!\n");
	printf("请选择： 1，返回好友列表\n");
	char op[5];
	scanf("%s",op);
	Write(op);
	communicate();
}

void add_fri(){
	pthread_mutex_lock(&mtx);
	system("clear");
	char name[50];
	printf("请输入要添加的好友用户名：");scanf("%s",name);
	Write(name);
	printf("你的请求已发送\n");
	printf("请选择： 1，返回好友列表\n");
	pthread_mutex_unlock(&mtx);
	char op[5];
	scanf("%s",op);
	Write(op);
	communicate();
}

void *send_f(void *arg){
	while(1){
		pthread_mutex_lock(&mtx);
		char str[120];
		scanf("%s",str);
		Write(str);
		if(strcmp(str,"bye")==0){
			pthread_cancel(recv_th);
			pthread_mutex_unlock(&mtx);
			break;
		}
		pthread_mutex_unlock(&mtx);
	}
	communicate();
}

void *recv_f(void *arg){
	while(1){
		pthread_mutex_lock(&mtx);
		char str[120];
		Read(str);
		printf("%s\n",str);
		pthread_mutex_unlock(&mtx);
	}
}

void chat(){
	pthread_mutex_lock(&mtx);
	system("clear");
	printf("退出请输入bye\n");
	pthread_mutex_unlock(&mtx);
	pthread_create(&send_th,NULL,send_f,NULL);
	pthread_create(&recv_th,NULL,recv_f,NULL);
	pthread_join(send_th,NULL);
	pthread_join(recv_th,NULL);
}
	
void choose_fri(){
	pthread_mutex_lock(&mtx);
	printf("请输入要聊天的好友名称:");
	char name[50];scanf("%s",name);
	Write(name);
	pthread_mutex_unlock(&mtx);
	chat();
}

void communicate(){ 
	
	pthread_mutex_lock(&mtx);
	system("clear");
	print_fri();
	printf("请选择：\n");
	printf("1，选择一位好友聊天\n");
	printf("2，添加好友\n");
	printf("3，删除好友\n");
	printf("4，刷新好友列表\n");
	printf("5，退出账号\n");
	pthread_mutex_unlock(&mtx);

	pthread_mutex_lock(&mtx);
	char op[5];
	scanf("%s",op);
	Write(op);
	pthread_mutex_unlock(&mtx);
	if(op[0]=='1') choose_fri();
	else if(op[0]=='2') add_fri();
	else if(op[0]=='3')	del_fri();
	else if(op[0]=='4') communicate();
	else sign_or_regist();
}

void regist(){
	system("clear");
	char name[50],psd[50];
	printf("请输入用户名：");scanf("%s",name);
	printf("请输入密码：");scanf("%s",psd);
	Write(name);Write(psd);
	char op[5];
	Read(op);
	if(op[0]=='1') printf("注册成功！\n");
	else printf("注册失败！\n");
	sign_or_regist();
}

void sign(){
	system("clear");
	char name[50],psd[50];
	printf("请输入用户名：");scanf("%s",name);
	printf("请输入密码：");scanf("%s",psd);
	Write(name);Write(psd);
	char op[5];
	Read(op);
	if(op[0]=='1') communicate();//登录成功
	else{
		printf("登录失败！\n");
		printf("请选择：1，回到开始页面	2，退出");
		scanf("%s",op);
		Write(op);
		if(op[0]=='1') sign_or_regist();
	}
}

void sign_or_regist(){
	system("clear");
	printf("请选择：\n");
	printf("	1，登录\n");
	printf("	2，注册\n");
	printf("	3，退出\n");
	char op[5];
	scanf("%s",op);
	Write(op);
	if(op[0]=='1') sign();
	else if(op[0]=='2') regist();
}

int main(){
	sock=socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=inet_addr(SERVER_ADDRESS);
	addr.sin_port=htons(SERVER_PORT);
	int cnt=0;
	system("clear");
	while(connect(sock,(sockaddr*)&addr,sizeof(addr))==-1){
		if(cnt==0) printf("服务器瘫痪中...\n");
		else if(cnt==1) printf("服务器正在修复...\n");
		else printf("服务器再次瘫痪!!!\n");
		cnt++;cnt%=3;
		sleep(2);
	}
	printf("连接成功\n");
	sleep(2);
	sign_or_regist();
	close(sock);
	return 0;
}
