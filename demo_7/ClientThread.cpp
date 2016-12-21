#include"ClientThread.h"
#include"Msg.h"
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include<algorithm>
#include<thread>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

using namespace std;

ClientThread::ClientThread(int sock_ce){
	this->sock_ce=sock_ce;
	userID=to_userID=-1;
	run();
}

void ClientThread::run(){
	while(1){
		userID=sign_or_regist();
		cout<<userID<<"--"<<endl;
		if(~userID) break;
	}
	communicate();
}

int ClientThread::sign_or_regist(){
	int op=0;
	printf("请选择: 1,注册 2,登录\n");
	scanf("%d",&op);
	Write(op);
	if(op==1){
		cout<<"A"<<endl;
		regist();
		cout<<"B"<<endl;
		return -1;
	}
	return sign();
}

void ClientThread::Read(int &x){
	read(sock_ce,&x,1200);
}

void ClientThread::Read(string &s){
	read(sock_ce,&s,1200);
}

void ClientThread::Write(int &x){
	write(sock_ce,&x,1200);
}

void ClientThread::Write(string &s){
	write(sock_ce,&s,1200);
}

void ClientThread::regist(){
	string name,psd;
	cout<<"请输入用户名：";cin>>name;
	cout<<"请输入密码：";cin>>psd;
	Write(name);Write(psd);
	int op;
	Read(op);
	if(op) cout<<"注册成功！"<<endl;
	else cout<<"注册失败，用户名已存在！"<<endl;
}

int ClientThread::sign(){
	string name,psd;
	cout<<"请输入用户名：";cin>>name;
	cout<<"请输入密码：";cin>>psd;
	int id;
	Read(id);
	if(~id) cout<<"登录成功!"<<endl;
	else cout<<"登录失败!"<<endl;
	return id;
}

void ClientThread::communicate(){
	string str="";
	Read(str);
	cout<<str;
	while(1){
		End=0;
		thread sd(&ClientThread::send,this);
		thread re(&ClientThread::receive,this);
		sd.join();
		re.join();
		if(End) break;
	}
}

void ClientThread::send(){
	Msg msg;
	msg.u=userID;
	cin>>msg.str;
	if(msg.str=="gg"){
		End=1;
		msg.u=-1;
		msg.str="";
		Write(msg);
		return;
	}
	cout<<"请选择接收信息的用户:";cin>>msg.v;
	to_userID=msg.v;
	Write(msg);
}

void ClientThread::receive(){
	Msg msg;
	Read(msg);
	cout<<msg.uname<<": "<<msg.str<<endl;
}

void ClientThread::Read(Msg &msg){
	read(sock_ce,&msg,1200);
}

void ClientThread::Write(Msg &msg){
	write(sock_ce,&msg,1200);
}
