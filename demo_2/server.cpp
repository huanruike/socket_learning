#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<thread>
#include<string>
#include<mutex>

using namespace std;

typedef long long ll;
const int maxn=1000100;
const int INF=1e9+10;

char str_se[maxn+10];
char str_re[maxn+10];

void Send(int sock_ce){
	while(cin>>str_se){
		write(sock_ce,str_se,maxn);
	}
}

void receive(int sock_ce){
	while(1){
		read(sock_ce,str_re,maxn);
		cout<<str_re<<endl;
	}
}

void communicate(int sock_ce){
	thread se(Send,sock_ce);
	thread re(receive,sock_ce);
	se.join();
	re.join();
}

int main(){
	int sock_sv=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in addr_sv;
	memset(&addr_sv,0,sizeof(addr_sv));
	addr_sv.sin_family=AF_INET;
	addr_sv.sin_addr.s_addr=inet_addr("127.0.0.1");
	addr_sv.sin_port=htons(8888);
	bind(sock_sv,(sockaddr*)&addr_sv,sizeof(addr_sv));
	listen(sock_sv,20);
	while(1){
		sockaddr_in addr_ce;
		socklen_t sz=sizeof(addr_ce);
		int sock_ce=accept(sock_sv,(sockaddr*)&addr_ce,&sz);
		communicate(sock_ce);
		close(sock_ce);
	}
	close(sock_sv);
	return 0;
}
