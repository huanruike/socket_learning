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
#include"ServerThread.h"
#include"UserData.h"

using namespace std;

typedef long long ll;
const int maxn=1010;

ServerThread::ServerThread(int sock_ce){
	this.sock_ce=sock_ce;
	this.userdata=userdata;
	thread(run());
}

void ServerThread::run(){
	while(1){
		printf("请选择：1 登录   2 注册");
		int op;cin>>op;
		if(op==1){
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
	printf("请输入用户名：");cin>>name;
	printf("请输入密码：");cin>>password;
	user_id=userdata.sign(name,password);
	if(~user_id) return 1;
	printf("登录失败\n");
	return 0;
}

void ServerThread::regist(){
	string name,password;
	printf("请输入用户名：");cin>>name;
	printf("请输入密码：");cin>>password;
	if(userdata.add(name,password)) puts("注册成功");
	else puts("注册失败");
}

void ServerThread::receive(){
	while(1){
		string msg;
		read(sock_ce,&msg,maxn);
		int u=get_id(s[0],s[1],s[2]);
		int v=get_id(s[3],s[4],s[5]);
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

