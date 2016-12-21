

-----socket--------------------------------------------
int socket(int domain,int type,int protocol);
	domain(协议域): AF_INET (ipv4)
				  AF_INET6
				  AF_LOCAL(AF_UNIX)
				  AF_ROUTE
				  ...
	type(socket类型): SOCK_STREAM,
					  SOCK_RAW
					  ...
	protocol(协议): IPPROTO_TCP
					IPPROTO_UDP
				    IPPROTO_STCP
					IPPROTO_TIPC
					...
返回值: 1,调用成功: socket描述符(int)
		2,调用失败: INVALID_SOCKET(linux下返回-1)

--------bind---------------------------------------------				   
int bind(SOCKET socket,const sockaddr* address,socklen_t addresslen);
返回值: 成功：0
		失败：SOCKET_ERROR
--------listen----------------------------------------------
int listen(int socket,int backlog);
	监听状态的socket维护一个客户连接请求队列，队列最多容纳backlog个用户请求
--------------------------------------------------------------



