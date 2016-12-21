#include<bits/stdc++.h>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<thread>
//#include"ServerThread.h"
//#include"UserData.h"

using namespace std;

typedef long long ll;
const int maxn=1010;

///---------------------------------
class UserData{
public:
	UserData();
	bool add(string name,string password);
	int sign(string name,string password);
	void addmsg(int u,int v,string msg);
	void send(int v_id,int sock_ce);
private:
	struct User{
		string name,password;
		bool online;
		queue<string> q;
		vector<int> Friend;
	};vector<User> user;
	
};

UserData::UserData(){
	user.clear();
}

bool UserData::add(string name,string password){
	for(int i=0;i<user.size();i++){
		if(user[i].name==name&&user[i].password==password) return 0;
	}
	user.push_back({name,password});
	return 1;
}

int UserData::sign(string name,string password){
	for(int i=0;i<user.size();i++){
		if(user[i].name==name&&user[i].password==password) return i;
	}
	return -1;
}

void UserData::addmsg(int u,int v,string msg){
	user[v].q.push(msg);
}

void UserData::send(int v_id,int sock_ce){
	while(!user[v_id].q.empty()){
		string str=user[v_id].q.front();
		user[v_id].q.pop();
		write(sock_ce,&str,maxn);
	}
}


///--------------------------------
UserData userdata;

///------------------------------------
class ServerThread{
public:
	ServerThread(int sock_ce);
	
private:
	int sock_ce;
	int user_id;
	void run();
	bool sign();
	void regist();
	void receive();
	void send();
	int get_id(char a,char b,char c);
};

ServerThread::ServerThread(int sock_ce){
	this->sock_ce=sock_ce;
	thread(run());
}

void ServerThread::run(){
	while(1){
		string op;
		read(sock_ce,&op,maxn);
		if(op[0]=='1'){
			if(!sign()) continue;
			break;
		}
		else regist();
	}
	thread(send());
	thread(receive());
}

bool ServerThread::sign(){
	string name,password;
	read(sock_ce,&name,maxn);
	read(sock_ce,&password,maxn);
	user_id=userdata.sign(name,password);
	if(~user_id) return 1;
	string str="登录失败\n";
	write(sock_ce,&str,maxn);
	return 0;
}

void ServerThread::regist(){
	string name,password;
	read(sock_ce,&name,maxn);
	read(sock_ce,&password,maxn);
	if(userdata.add(name,password)){
		string str="注册成功\n";
		write(sock_ce,&str,maxn);
	}
	else{
		string str="注册失败\n";
		write(sock_ce,&str,maxn);
	}
}

void ServerThread::receive(){
	while(1){
		string msg;
		read(sock_ce,&msg,maxn);
		int u=get_id(msg[0],msg[1],msg[2]);
		int v=get_id(msg[3],msg[4],msg[5]);
		userdata.addmsg(u,v,msg);
	}
}

int ServerThread::get_id(char a,char b,char c){
	a-='0';b-='0';c-='0';
	return a*100+b*10+c;
}

void ServerThread::send(){
	while(1){
		string msg;
		userdata.send(user_id,sock_ce);
	}
}


///---------------------------------------

int main(){
	int sock_sv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in addr_sv;
	memset(&addr_sv,0,sizeof(addr_sv));
	addr_sv.sin_family=AF_INET;
	addr_sv.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr_sv.sin_port=htons(8888);
	bind(sock_sv,(sockaddr*)&addr_sv,sizeof(addr_sv));
	listen(sock_sv,20);
	int sock_ce;
	while(1){
		sockaddr_in addr_ce;
		socklen_t sz=sizeof(addr_ce);
		sock_ce=accept(sock_sv,(sockaddr*)&addr_ce,&sz);
		ServerThread serverthread(sock_ce);
	}
	close(sock_ce);
	close(sock_sv);
	return 0;
}
