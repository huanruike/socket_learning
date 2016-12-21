#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#include<sys/socket.h>
#include<netinet.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<thread>

using namespace std;

typedef long long ll;
const int maxn=1010;

class Client{
public:
	Client(int sock_ce,int i){
		this.th=thread(run(sock_ce));
		use=1;
		msg="";
		user_id=0;
	}
	~Client(){};
	void run();
private:
	void start();
	void Denglu();
	void th_fun_read();
	void th_fun_write();
	thread th;
	bool use;
	string msg;
	int user_id;
};Client client[maxn];int client_n;

class User{
public:
	User(){
		isUser=0;
		username=password="";
		Friend.clear();
	};
	~User(){};
	User(string username,string password);

private:
	bool isUser;
	string username;
	string password;
	bool online;
	int client_id;
	string send;
	queue<string> receive;
	vector<int> Friend;
};vector<User> user;



void Client::run(){
	while(!Denglu);
	thread th_read=thread(th_fun_read());
	thread th_write=thread(th_fun_write());
}

bool Client::Denglu(){
	printf("请选择： 1，登录	2，注册\n");
	int op;
	scanf("%d",&op);
	if(op==2) Resign();
	string username;
	string password;
	printf("请输入用户名：");cin>>username;
	printf("请输入密码：");cin>>password;
	for(int i=0;i<user.size();i++){
		if(user[i].Is(username,password)){
			flag=0;
			user[i].Up();
			user_id=user[i].getID();
		}
	}
	
}


int main(){
	int sock_sv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in addr_sv;
	memset(&addr_sv,0,sizeof(addr_sv));
	addr_sv.sin_family=AF_INET;
	addr_sv.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr_sv.sin_port=htons(8888);
	bind(sock_sv,(sockaddr*)&addr_sv,sizeof(addr_sv));
	listen(sock_sv,20);
	while(1){
		sockaddr_in addr_ce;
		socklen_t sz=sizeof(addr_ce);
		int sock_ce=accept(addr_sv,(sockaddr*)&addr_ce,&sz);
		int id=Get();
		if(id==-1) continue;
		client[id]=new Client(sock_ce);
	}
	close(sock_ce);
	close(sock_sv);
	return 0;
}
