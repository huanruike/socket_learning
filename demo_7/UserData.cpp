#include"UserData.h"
#include"Msg.h"
#include<string>
#include<queue>
#include<bits/stdc++.h>

using namespace std;

UserData::UserData(){
	user.clear();
}

bool UserData::insert(string name,string psd){
	User u;
	u.name=name;u.psd=psd;
	u.online=0;
	while(!u.q.empty()) u.q.pop();
	user.push_back(u);
	return 1;
}

int UserData::sign(string name,string psd){
	for(int i=0;i<user.size();i++){
		if(user[i].name==name&&user[i].psd==psd&&user[i].online==0){
			user[i].online=1;
			cout<<"i="<<i<<endl;
			return i;
		}
	}
	return -1;
}

Msg UserData::getMsg(int id){
	Msg msg;
	if(user[id].q.empty()) msg.u=-1;
	else{
		msg=user[id].q.front();
		user[id].q.pop();
	}
	return msg;
}

void UserData::addMsg(Msg msg){
	msg.uname=user[msg.u].name;
	msg.vname=user[msg.v].name;
	user[msg.v].q.push(msg);
}

string UserData::PrintFriend(int user_id){
	string str="";
	str+='\n';
	for(int i=0;i<user.size();i++){
		if(i==user_id) continue;
		str+=(i+'0');
		str+=" ";
		str+=user[i].name;
		str+='\n';
	}
	str+='\n';
	return str;
}

