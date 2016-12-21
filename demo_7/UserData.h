#ifndef USERDATA_H
#define USERDATA_H

#include<string>
#include<vector>
#include<queue>
#include"Msg.h"

using namespace std;

class UserData{
public:
	UserData();
	bool insert(string name,string psd);
	int sign(string name,string psd);
	Msg getMsg(int id);
	void addMsg(Msg msg);
	string PrintFriend(int user_id);

private:
	struct User{
		string name,psd;
		bool online;
		queue<Msg> q;
	};vector<User> user;


};
#endif
