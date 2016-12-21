#include"Point.h"

#include<iostream>
#include<cmath>

Point::Point(){

}

Point::Point(double x,double y){
	this->x=x;this->y=y;
}

void Point::out(){
	std::cout<<"x="<<x<<" y="<<y<<std::endl;
}

double Point::dist(const Point &B){
	return fabs(x-B.x)+fabs(y-B.y);
}
