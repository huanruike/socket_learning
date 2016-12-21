#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include"UserData.h"
#include"Msg.h"

class ServerThread{
public:
	ServerThread(int sock_ce,UserData *userdata);

private:
	void run();
	
	int sign_or_regist();
	int sign();
	void regist();
	void communicate();
	void Read(int &x);
	void Read(string &s);
	void Read(Msg &msg);
	void Write(int &x);
	void Write(string &s);
	void Write(Msg &msg);
	void send();
	void receive();
	string Print();
	
	int sock_ce;
	int user_id;
	UserData *userdata;
	int End;
	

};

#endif
