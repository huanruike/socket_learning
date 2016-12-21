#include<bits/stdc++.h>
#include<pthread.h>
#include<unistd.h>

using namespace std;

pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;

void *f(void *s){
	pthread_mutex_lock(&mtx);
	for(int i=1;i<=10;i++){
		printf("i=%d id=%d s=%s\n",i,(int)pthread_self(),(char*)s);
		usleep(100000);
	}
	pthread_mutex_unlock(&mtx);
}

int main(){
	pthread_t th1,th2;
	char s1[]="first";
	char s2[]="second";
	pthread_create(&th1,NULL,f,(void*)s1);
	pthread_create(&th2,NULL,f,(void*)s2);
	pthread_join(th1,NULL);
	pthread_join(th2,NULL);
	sleep(1);
	return 0;
}




