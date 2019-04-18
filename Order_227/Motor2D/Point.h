// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef POINT_H
#define POINT_H

#include "Defs.h"
#include <cmath>

template<class TYPE>
class Point
{
public:

	TYPE x, y;

	Point()
	{}

	Point(const Point<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	Point(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	Point& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	Point operator -(const Point &v) const
	{
		Point<TYPE> r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	Point operator +(const Point &v) const
	{
<<<<<<< HEAD
		iPoint r;
=======
		Point<TYPE> r;
>>>>>>> GroupsMovement_Jaume

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const Point& operator -=(const Point &v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const Point& operator +=(const Point &v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const Point& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const Point& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	Point& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	Point& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx*fx) + (fy*fy));
	}

	TYPE DistanceNoSqrt(const Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx*fx) + (fy*fy);
	}

	TYPE DistanceManhattan(const Point& v) const
	{

	/*	TYPE dx = abs(v.x - x);
		TYPE dy = abs(v.y - y);
		return dx + dy;*/

		return abs(v.x - x) + abs(v.y - y);

	}

	TYPE DiagonalDistance(const Point& v) const
	{

		TYPE fx = abs(v.x - x);
		TYPE fy = abs(v.y - y);

		return (fx + fy)*min(fx, fy);

	}
};

typedef Point<int> iPoint;
typedef Point<unsigned int> uPoint;
typedef Point<float> fPoint;

#endif // POINT_H