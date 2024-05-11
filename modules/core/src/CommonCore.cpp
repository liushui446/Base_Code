#include "as/CommonCore.hpp"

namespace as
{
	Point::Point() {
		x = 0;
		y = 0;
	};

	Point::Point(double _x, double _y) {
		x = _x;
		y = _y;
	};

	Point::Point(const Point& pt)
		: x(pt.x), y(pt.y) {}

	Point& Point::operator = (const Point& pt) {
		this->x = pt.x;
		this->y = pt.y;
		return *this;
	}

	Point Point::operator + (const Point& pt) {
		return Point(this->x + pt.x, this->y + pt.y);
	}

	Point& Point::operator += (const Point& pt) {
		this->x = this->x + pt.x;
		this->y = this->y + pt.y;
		return *this;
	}

	Point Point::operator - (const Point& pt) {
		return Point(this->x - pt.x, this->y - pt.y);
	}

	Point& Point::operator -= (const Point& pt) {
		this->x = this->x - pt.x;
		this->y = this->y - pt.y;
		return *this;
	}

	Point Point::operator * (const int val) {
		return Point(this->x * val, this->y * val);
	}

	Point& Point::operator *= (const int val) {
		this->x *= val;
		this->y *= val;
		return *this;
	}

	Point Point::operator / (const int val) {
		return Point(this->x / val, this->y / val);
	}

	Point& Point::operator /= (const int val) {
		this->x /= val;
		this->y /= val;
		return *this;
	}

	void Point::setPoint(double _x, double _y) {
		x = _x;
		y = _y;
	};

	void Point::setPointX(double _x)
	{
		x = _x;
	}

	void Point::setPointY(double _y)
	{
		y = _y;
	}

	void Point::setPoint(Point pos) {
		x = pos.x;
		y = pos.y;
	};

	bool Point::operator==(const Point p)
	{
		return x == p.x && y == p.y;
	}

	double Point::getPintX()
	{
		return x;
	}

	double Point::getPintY()
	{
		return y;
	}

}