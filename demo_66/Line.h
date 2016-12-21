#ifndef LINE_H
#define LINE_H
#include"Point.h"

class Line{
public:
	Line();
	Line(const Point &A,const Point &B);
	double len();

private:
	Point A,B;

};
#endif
