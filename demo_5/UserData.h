#ifndef USERDATA_H
#define USERDATA_H

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
#include<bits/stdc++.h>
#include"ServerThread.h"
#include"UserData.h"

using namespace std;

typedef long long ll;
//const int maxn=1010;


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
#endif
