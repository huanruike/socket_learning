#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include"Msg.h"
#include<string>

class ClientThread{
public:
	ClientThread(int sock_ce);
	void run();
private:
	int sign_or_regist();
	int sign();
	void regist();
	void Read(int &x);
	void Read(string &s);
	void Read(Msg &msg);
	void Write(int &x);
	void Write(string &s);
	void Write(Msg &msg);
	void communicate();
	void send();
	void receive();


	int sock_ce;
	int userID;
	int to_userID;
	int End;
	
};

#endif
