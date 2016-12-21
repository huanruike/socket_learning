#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

using namespace std;

int main(){
	int serv_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	sockaddr_in serv_addr;
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;//ipv4
	//serv_addr.sin_addr.s_addr=inet_addr("10.177.165.118");//ip
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//ip
	serv_addr.sin_port=htons(8888);//端口
	bind(serv_sock,(sockaddr*)&serv_addr,sizeof(serv_addr));
	
	listen(serv_sock,20);
	sockaddr_in clnt_addr;
	socklen_t clnt_addr_sz=sizeof(clnt_addr);
	int clnt_sock=accept(serv_sock,(sockaddr*)&clnt_addr,&clnt_addr_sz);

	char str[]="hello world";
	write(clnt_sock,str,sizeof(str));
	close(clnt_sock);
	close(serv_sock);

	return 0;
}
