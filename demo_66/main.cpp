#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include"Point.h"
#include"Line.h"

using namespace std;

int main(){
	Point A(1.0,1.0),B(2.0,3.0);
	A.out();B.out();
	cout<<"d_AB="<<A.dist(B)<<endl;
	Line L(A,B);
	cout<<"L_len="<<L.len()<<endl;
	return 0;
}
	

