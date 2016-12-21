#include<iostream>
#include<cstdlib>
#include<thread>
 
using namespace std;
 
void threadFunc()
{
    for(int i=1;i<=10;++i)
        cout<<"子线程"<<"->"<<i<<endl;
}
 
int main()
{
    thread t(threadFunc);
    for(int i=1;i<=10;++i)
        cout<<"主线程"<<"~"<<i<<endl;
	cout<<"------"<<endl;
    t.join();
    return EXIT_SUCCESS;
}
