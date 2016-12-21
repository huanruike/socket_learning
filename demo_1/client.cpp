#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
	int sock=socket(AF_INET,SOCK_STREAM,0);
	
	sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(8888);
	connect(sock,(sockaddr*)&serv_addr,sizeof(serv_addr));

	char s[40];
	read(sock,s,sizeof(s)-1);
	
	printf("%s\n",s);

	close(sock);
	
	return 0;
}

