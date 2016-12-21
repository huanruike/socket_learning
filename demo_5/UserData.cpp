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
