#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include"ServerThread.h"

using namespace std;


int main(){
	//link addr_sv & sock_sv,listen the sock_sv
	int sock_sv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in addr_sv;
	memset(&addr_sv,0,sizeof(addr_sv));
	addr_sv.sin_family=AF_INET;
	addr_sv.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr_sv.sin_port=htons(8888);
	bind(sock_sv,(sockaddr*)&addr_sv,sizeof(addr_sv));
	listen(sock_sv,20);
	UserData userdata;
	//communicate with client
	while(1){
		sockaddr_in addr_ce;
		socklen_t sz=sizeof(addr_ce);
		int sock_ce=accept(sock_sv,(sockaddr*)&addr_ce,&sz);
		ServerThread th(sock_ce,&userdata);
	}
	return 0;
}
