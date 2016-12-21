#include"ServerThread.h"
#include"UserData.h"
#include"Msg.h"
#include<thread>
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

using namespace std;

ServerThread::ServerThread(int sock_ce,UserData *userdata){
	this->sock_ce=sock_ce;
	this->userdata=userdata;
	thread t(&ServerThread::run,this);t.join();
}

void ServerThread::run(){
	// sign or regist
	while(1){
		user_id=sign_or_regist();
		if(~user_id) break;
	}
	// has sign
	communicate();
}

int ServerThread::sign_or_regist(){
	int op=0;
	Read(op);
	if(op==1){
		regist();
		return -1;
	}
	return sign();
}

void ServerThread::Read(int &x){
	read(sock_ce,&x,1200);
}

void ServerThread::Read(string &s){
	read(sock_ce,&s,1200);
}

void ServerThread::Write(int &x){
	write(sock_ce,&x,1200);
}

void ServerThread::Write(string &s){
	write(sock_ce,&s,1200);
}

void ServerThread::regist(){
	string name,psd;
	Read(name);Read(psd);
	int op;
	if(userdata->insert(name,psd)) op=1;
	else op=0;
	Write(op);
	cout<<"op="<<op<<endl;
}

int ServerThread::sign(){
	string name,psd;
	Read(name);Read(psd);
	int id=userdata->sign(name,psd);
	cout<<"id="<<id<<endl;
	Write(id);
	return id;
}

string ServerThread::Print(){
	return userdata->PrintFriend(user_id);
}

void ServerThread::communicate(){
	string str=Print();
	Write(str);
	while(1){
		End=0;
		thread sd(&ServerThread::send,this);
		thread re(&ServerThread::receive,this);
		sd.join();
		re.join();
		if(End) break;
	}
}

void ServerThread::send(){
	Msg msg=userdata->getMsg(user_id);//发送给user_id的消息,get后移除
	if(msg.u==-1) return;
	Write(msg);
}

void ServerThread::Read(Msg &msg){
	read(sock_ce,&msg,1200);
}

void ServerThread::Write(Msg &msg){
	write(sock_ce,&msg,1200);
}

void ServerThread::receive(){
	Msg msg;
	Read(msg);
	if(msg.u==-1){
		End=1;
		return;
	}
	userdata->addMsg(msg);
}


