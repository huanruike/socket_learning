#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

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
//const int maxn=1010;

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
#endif
