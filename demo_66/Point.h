#ifndef POINT_H
#define POINT_H

class Point{
public:
	Point();
	Point(double A,double B);
	void out();
	double dist(const Point &B);

private:
	double x,y;

};
#endif
