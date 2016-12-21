#include<bits/stdc++.h>
#include<pthread.h>

using namespace std;

void *f(void *arg){
	int input=*(int*)arg;
	cout<<"*arg="<<input<<" running"<<endl;
}

int main(){
	pthread_t th;
	int input=12;
	int res=pthread_create(&th,NULL,f,(void*)&input);
	if(res!=0) cout<<"error"<<endl;
	pthread_join(th,NULL);
	cout<<"main Running"<<" _"<<endl;
	return 0;
}
