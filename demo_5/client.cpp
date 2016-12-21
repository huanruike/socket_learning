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
//#include"ServerThread.h"
//#include"UserData.h"

using namespace std;

typedef long long ll;
const int maxn=1010;

string Name,Password;
int User_id;

int get_id(char a,char b,char c){
	a-='0';b-='0';c-='0';
	return a*100+b*10+c;
}

string get_S(int x){
	int a=x%10;x/=10;
	int b=x%10;x/=10;
	int c=x%10;x/=10;
	string res="";
	res+=(c+'0');
	res+=(b+'0');
	res+=(a+'0');
}

void send(int sock_ce){
	for(int i=1;i<=100;i++) cout<<"=";cout<<endl;
	cout<<"在线好友"<<endl;
	string sss;
	read(sock_ce,&sss,maxn);
	cout<<sss<<endl;
	while(1){
		cin>>sss;
		cout<<"请选择发送对象: ";
		int id;cin>>id;
		string ids=get_S(id);
		string myids=get_S(user_id);
		string msg=myids+ids+sss;
		write(sock_ce,&msg,maxn);
	}
}

bool sign(int sock_ce){
	cout<<"请输入用户名：";cin>>Name;
	cout<<"请输入密码：";cin>>Password;
	write(sock_ce,&Name,maxn);
	write(sock_ce,&Password,maxn);
	string str;
	read(sock_ce,&str,maxn);
	if(str=="Yes"){
		read(sock_ce,&str,maxn);
		User_id=get_id(str[0],str[1],str[2]);
		return 1;
	}
	cout<<str<<endl;
	return 0;
}

void regist(int sock_ce){
	cout<<"请输入用户名：";cin>>Name;
	cout<<"请输入密码：";cin>>Password;
	write(sock_ce,&Name,maxn);
	write(sock_ce,&Password,maxn);
	string str;
	read(sock_ce,&str,maxn);
	cout<<str<<endl;
	return;
}

void Begin(int sock_ce){
	string op;
	while(1){
		cout<<"请选择： 1,登录   2,注册"<<endl;
		cin>>op;
		write(sock_ce,&op,maxn);
		if(op=="1"){
			if(!sign()) continue;
			break;
		}
		else regist(sock_ce);
	}
}


int main(){
	int sock_ce=socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in addr_sv;
	memset(&addr_sv,0,sizeof(addr_sv));
	addr_sv.sin_family=AF_INET;
	addr_sv.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr_sv.sin_port=htons(8888);
	connect(sock_ce,(sockaddr*)&addr_sv,sizeof(addr_sv));
	
	Begin(sock_ce);
	
	thread(send(sock_ce));
	thread(receive(sock_ce));
	
	close(sock_ce);
	return 0;
}





