#include<bits/stdc++.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>

using namespace std;

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8888
//服务器地址和端口

const int maxn=1000100;
int sock_sv;


struct Sysmsg{
	char str[120];
	int from_id;
};

struct UserData{
	struct Msg{
		int from_id;
		char str[120];
	};
	struct User{
		char name[50],psd[50];
		vector<int> Fri;
		int connect_to;//聊天窗口指向,-1表示没有所指
		queue<Sysmsg> sys_q[120];
		queue<Msg> msg_q[120];
	};vector<User> user;
	void init(){
		user.clear();
	}
	int find(char *name,char *psd){
		for(int i=0;i<user.size();i++){
			if(strcmp(name,user[i].name)==0&&strcmp(psd,user[i].psd)==0){
				return i;
			}
		}
		return -1;
	}
	int find(char *name){
		for(int i=0;i<user.size();i++){
			if(strcpy(name,user[i].name)==0) return i;
		}
		return -1;
	}
	int add(char *name,char *psd){
		if(find(name,psd)!=-1) return -1;
		User nu;
		strcpy(nu.name,name);
		strcpy(nu.psd,psd);
		nu.connect_to=-1;
		for(int i=0;i<user.size();i++) while(!nu.sys_q[i].empty()) nu.sys_q[i].pop();
		
		for(int i=0;i<user.size();i++) while(!nu.msg_q[i].empty()) nu.msg_q[i].pop();
		user.push_back(nu);
		return (int)user.size()-1;
	}
	void add_sysmsg(int id,char *name){
		int to_id=find(name);
		if(to_id==-1) return;
		Sysmsg smg;
		smg.from_id=id;
		smg.str[0]='#';smg.str[1]='#';smg.str[2]='\0';
		strcat(smg.str,name);
		char ss[]=" 请求添加你为好友\n";
		strcat(smg.str,ss);
		user[to_id].sys_q[id].push(smg);
	}
	void del_fri(int id,char *name){
		int to_id=find(name);
		if(to_id==-1) return;
		for(vector<int>::iterator it=user[id].Fri.begin();
				it!=user[id].Fri.end();++it){
			if((*it)==to_id){
				user[id].Fri.erase(it);
				break;
			}
		}
		for(vector<int>::iterator it=user[to_id].Fri.begin();
				it!=user[to_id].Fri.end();++it){
			if((*it)==id){
				user[to_id].Fri.erase(it);
				break;
			}
		}
		while(!user[id].msg_q[to_id].empty()) user[id].msg_q[to_id].pop();
		while(!user[to_id].msg_q[id].empty()) user[to_id].msg_q[id].pop();
	}
	void print_fri(int id,char *str){
		str[0]='\0';
		for(int i=0;i<user[id].Fri.size();i++){
			int x=user[id].Fri[i];
			strcpy(str,user[x].name);
			strcpy(str,"\n");
		}
		strcpy(str,"\n");
	}
	void get_str(int id,int to_id,char *str){
		str[0]='\0';
		if(user[to_id].msg_q[id].empty()) return;
		strcpy(str,user[to_id].msg_q[id].front().str);
		user[to_id].msg_q[id].pop();
	}
	void insert_str(int id,int to_id,char *str){
		Msg msg;
		msg.from_id=id;
		strcpy(msg.str,user[id].name);
		strcat(msg.str,": ");
		strcat(msg.str,str);
		user[to_id].msg_q[id].push(msg);
	}
	void set_connect_to(int id,int to_id){
		user[id].connect_to=to_id;
	}
	void brk(int id,int to_id){
		user[id].connect_to=-1;
	}
	int find_fri(char *name,int id){
		for(int i=0;i<user[id].Fri.size();i++){
			if(strcmp(user[user[id].Fri[i]].name,name)==0) return i;
		}
		return -1;
	}
	Sysmsg get_sysmsg(int id){
		Sysmsg smg;
		for(int i=0;i<user.size();i++){
			if(i==id) continue;
			if(user[id].sys_q[i].empty()) continue;
			smg=user[id].sys_q[i].front();
			user[id].sys_q[i].pop();
			return smg;
		}
		smg.from_id=-1;
		return smg;
	}
	void con_fri(int id,int to_id){
		user[id].Fri.push_back(to_id);
		user[to_id].Fri.push_back(id);
	}
};UserData userdata;

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

void communicate(int sock,int id);
void sign_or_regist(int sock);

void Write(char *str,int sock){
	write(sock,str,sizeof(str));
}

void Write(char *str,int sock,int op){
	write(sock,str,sizeof(str));
}

void Read(char *str,int sock){
	read(sock,str,sizeof(str));	
}

void Read(char *str,int sock,int op){
	read(sock,str,sizeof(str));
}

void print_fri(int sock,int id){
	char str[1200];
	userdata.print_fri(id,str);
	cout<<"str=\n"<<str<<endl;
	Write(str,sock);
}

void del_fri(int sock,int id){
	char name[50];
	Read(name,sock);
	userdata.del_fri(id,name);
	char op[5];
	Read(op,sock);
	communicate(sock,id);
}

void add_fri(int sock,int id){
	pthread_mutex_lock(&mtx);
	char name[50];
	Read(name,sock);
	userdata.add_sysmsg(id,name);
	pthread_mutex_unlock(&mtx);
	char op[5];
	Read(op,sock);
	communicate(sock,id);
}

struct Info{
	int sock;
	int id;
	int to_id;
	pthread_t send_th;
};

void *send_f(void *info){
	Info *ifo=(Info*)info;
	int sock=ifo->sock;
	while(1){
		pthread_mutex_lock(&mtx);
		char str[120];
		userdata.get_str(ifo->to_id,ifo->id,str);
		Write(str,sock);
		pthread_mutex_unlock(&mtx);
	}
}

void *recv_f(void *info){
	Info *ifo=(Info*)info;
	int sock=ifo->sock;
	while(1){
		pthread_mutex_lock(&mtx);
		char str[120];
		Read(str,sock);
		userdata.insert_str(ifo->id,ifo->to_id,str);
		if(strcmp(str,"bye")==0){
			userdata.brk(ifo->id,ifo->to_id);// 中断id->to_id连接
			pthread_cancel(ifo->send_th);
			pthread_mutex_unlock(&mtx);
			break;
		}
		pthread_mutex_unlock(&mtx);
	}
	communicate(ifo->sock,ifo->id);
}

void chat(int sock,int id,int to_id){
	pthread_t send_th,recv_th;
	Info info;
	info.sock=sock;
	info.id=id;
	info.to_id=to_id;
	info.send_th=send_th;
	pthread_create(&send_th,NULL,send_f,&info);
	info.send_th=send_th;
	pthread_create(&recv_th,NULL,recv_f,&info);
	pthread_join(send_th,NULL);
	pthread_join(recv_th,NULL);
}

void choose_fri(int sock,int id){
	pthread_mutex_lock(&mtx);
	char name[50];
	Read(name,sock);
	int to_id=userdata.find_fri(name,id);
	userdata.set_connect_to(id,to_id);
	pthread_mutex_unlock(&mtx);
	chat(sock,id,to_id);
}

struct MsgInfo{
	int sock,id;
};

void *sys_msg_f(void *msginfo){
	MsgInfo *mifo=(MsgInfo*)msginfo;
	int id=mifo->id;
	int sock=mifo->sock;
	while(1){
		pthread_mutex_lock(&mtx);
		Sysmsg smg=userdata.get_sysmsg(id);
		if(smg.from_id==-1){
			pthread_mutex_unlock(&mtx);
			continue;
		}
		Write(smg.str,sock,1);
		Read(smg.str,sock,1);
		if(smg.str[2]=='1') userdata.con_fri(id,smg.from_id);
		pthread_mutex_unlock(&mtx);
	}
}

void communicate(int sock,int id){
	MsgInfo msginfo;
	msginfo.sock=sock;msginfo.id=id;
	pthread_t sys_msg_th;
	pthread_create(&sys_msg_th,NULL,sys_msg_f,&msginfo);
	
	pthread_mutex_lock(&mtx);
	cout<<"print_fri"<<endl;
	print_fri(sock,id);
	cout<<"print_f--"<<endl;
	pthread_mutex_unlock(&mtx);

	pthread_mutex_lock(&mtx);
	char op[5];
	Read(op,sock);
	pthread_mutex_unlock(&mtx);
	if(op[0]=='1') choose_fri(sock,id);
	else if(op[0]=='2') add_fri(sock,id);
	else if(op[0]=='3') del_fri(sock,id);
	else if(op[0]=='4') communicate(sock,id);
	else{
		pthread_cancel(sys_msg_th);
		sign_or_regist(sock);
	}
}


void regist(int sock){
	char name[50],psd[50];
	Read(name,sock);Read(psd,sock);
	char op[5];
	if(userdata.add(name,psd)!=-1) op[0]='1';
	else op[0]='2';
	op[1]='\0';
	Write(op,sock);
	sign_or_regist(sock);
}

void sign(int sock){
	char name[50],psd[50];
	Read(name,sock);Read(psd,sock);
	char op[5];
	int id=userdata.find(name,psd);
	if(id==-1) op[0]='2';
	else op[0]='1';
	op[1]='\0';
	Write(op,sock);
	if(op[0]==1) communicate(sock,id);
	else{
		Read(op,sock);
		if(op[0]=='1') sign_or_regist(sock);
	}
}

void sign_or_regist(int sock){
	char op[5];
	Read(op,sock);
	if(op[0]=='1') sign(sock);
	else if(op[0]=='2') regist(sock);
	close(sock);
}

int main(){
	sock_sv=socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in addr_sv;
	addr_sv.sin_family=AF_INET;
	addr_sv.sin_addr.s_addr=inet_addr(SERVER_ADDRESS);
	addr_sv.sin_port=htons(SERVER_PORT);
	bind(sock_sv,(sockaddr*)&addr_sv,sizeof(addr_sv));
	listen(sock_sv,30);
	userdata.init();
	while(1){
		sockaddr_in addr;
		socklen_t sz;
		pthread_mutex_lock(&mtx);
		int sock=accept(sock_sv,(sockaddr*)&addr,&sz);
		pthread_mutex_unlock(&mtx);
		sign_or_regist(sock);
	}
	close(sock_sv);
	return 0;
}





