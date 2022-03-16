#include "OGL_Shapes.h"
#include <math.h>

constexpr auto PI = 3.1415926535897;
constexpr auto TWO_PI = PI * 2;

namespace shapes
{
	Point::Point(double x_)
		:x(x_), y(x_) {}

	Point::Point(double x_, double y_)
		:x(x_), y(y_) {}

	Point Point::operator+(const Point& p2) const
	{
		return Point(this->x + p2.x, this->y + p2.y);
	}

	Point Point::operator-(const Point& p2) const
	{
		return Point(this->x - p2.x, this->y - p2.y);
	}

	Point Point::operator*(double f) const
	{
		return Point(this->x * f, this->y * f);
	}

	Point Point::operator/(double f) const
	{
		return Point(this->x / f, this->y / f);
	}

	Point& Point::operator+=(const Point& p2)
	{
		this->x += p2.x;
		this->y += p2.y;
		return *this;
	}

	Point& Point::operator-=(const Point& p2)
	{
		this->x -= p2.x;
		this->y -= p2.y;
		return *this;
	}

	Point& Point::operator*=(double f)
	{
		this->x *= f;
		this->y *= f;
		return *this;
	}

	Point& Point::operator/=(double f)
	{
		this->x /= f;
		this->y /= f;
		return *this;
	}

	double Point::operator*(const Point& p2) const
	{
		return this->x * p2.x + this->y * p2.y;
	}

	Polygon::Polygon(const Point* p_, int count_)
		:count(count_)
	{
		centre = Point(0.0);
		for (int i = 0; i < count; i++)
		{
			centre += *(p_ + i);
			p.push_back(*(p_ + i));
		}
		centre /= count;
		Point temp = p[0] - centre;
		angle = temp.x == 0? PI/2:atan(temp.y/temp.x);

	}

	Polygon::Polygon(const Point& centre_,double radius,  int count_, double angle_)
		:centre(centre_), count(count_), angle(angle_)
	{
		double theta = TWO_PI / count;
		for (int i = 0; i < count; i++)
		{
			p.push_back(centre + Point(cos(i * theta + angle), sin(i * theta + angle)) * radius);
		}

	}

	Mat22::Mat22(const Point& p1, const Point& p2)
		:a(p1), b(p2) {}

	Mat22::Mat22(double ax, double ay, double bx, double by)
	{
		a.x = ax;
		a.y = ay;
		b.x = bx;
		b.y = by;
	}

	Mat22::Mat22(double angle)
	{
		a = {  cos(angle), sin(angle) };
		b = { -sin(angle), cos(angle) };
	}

	Point Mat22::operator*(const Point& p) const
	{
		return Point(a.x * p.x + b.x * p.y, a.y * p.x + b.y * p.y);
	}
}


