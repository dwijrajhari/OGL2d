#include <vector>

namespace shapes
{
	struct Point
	{
		double x, y;
		Point() {};
		Point(double);
		Point(double, double);
		Point operator +(const Point&) const;
		Point operator -(const Point&) const;
		Point operator *(double) const;
		Point operator /(double) const;

		Point& operator +=(const Point&);
		Point& operator -=(const Point&);
		Point& operator *=(double);
		Point& operator /=(double);

		double operator *(const Point&) const;

	
	};

	struct Polygon
	{
		std::vector<Point> p;
		int count;

		double angle;
		Point centre;

		Polygon() {};
		Polygon(const Point*, int);
		Polygon(const Point&, double, int, double angle_ = 0.0f);
	};

	struct Mat22
	{
		Point a, b;

		Mat22() {};
		Mat22(const Point&, const Point&);
		Mat22(double, double, double, double);
		Mat22(double);

		Point operator *(const Point&) const;
	};
}