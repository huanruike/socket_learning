#include<bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<fcntl.h>

using namespace std;

#define SERVER_ADDRESS "10.181.149.200"
#define SERVER_PORT 8005
//服务器地址和端口

const int maxn=2100;

vector<int> sock;
char buf[maxn];
struct Msg{
	char buf[maxn];
};vector<Msg> msg;

bool isBye(char *s){
	int len=strlen(s);
	return len>=2&&s[len-1]=='G'&&s[len-2]=='G';
}

int main(){
	int sock_sv=socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in addr_sv;
	addr_sv.sin_family=AF_INET;
	addr_sv.sin_addr.s_addr=inet_addr(SERVER_ADDRESS);
	addr_sv.sin_port=htons(SERVER_PORT);
	bind(sock_sv,(sockaddr*)&addr_sv,sizeof(addr_sv));
	// 设置成非阻塞模式
	int flags=fcntl(sock_sv,F_GETFL,0);
	fcntl(sock_sv,F_SETFL,flags|O_NONBLOCK);
	listen(sock_sv,30);
	while(1){
		// add sock
		sockaddr_in addr;
		socklen_t sz;
		int new_sock=accept(sock_sv,(sockaddr*)&addr,&sz);
		if(new_sock!=-1){
			sock.push_back(new_sock);
			//int flags=fcntl(new_sock,F_GETFL,0);
			//fcntl(new_sock,F_SETFL,flags|O_NONBLOCK);
		}
		//cout<<(int)sock.size()<<endl;
		// communicate
		///  read
		msg.clear();
		int cnt=0;
		for(vector<int>::iterator it=sock.begin();it!=sock.end();){
			int res=recv(*it,buf,maxn,MSG_DONTWAIT);
			if(res<=0){
				it++;continue;
			}	
			if(isBye(buf)){
				vector<int>::iterator it1=it;
				++it;
				close(*it1);
				sock.erase(it1);
			}
			else{
				++it;
				if(strlen(buf)==0||buf[0]!='#') continue;
				Msg new_msg;
				strcpy(new_msg.buf,buf);
				msg.push_back(new_msg);
			}
		}
		///  write
		for(vector<int>::iterator it=sock.begin();it!=sock.end();++it){
			for(int i=0;i<msg.size();i++){
				send(*it,msg[i].buf,maxn-1,MSG_DONTWAIT);
			}
		}
		msg.clear();
	}
	close(sock_sv);
	return 0;
}

