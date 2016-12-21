#include"Point.h"
#include"Line.h"

Line::Line(){

}

Line::Line(const Point &A,const Point &B){
	this->A=A;
	this->B=B;
}

double Line::len(){
	return A.dist(B);
}

