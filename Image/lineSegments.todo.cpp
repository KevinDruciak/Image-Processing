#include "lineSegments.h"
#include <math.h>
#include <Util/exceptions.h>

using namespace Util;
using namespace Image;

////////////////////////////
// Image processing stuff //
////////////////////////////
double OrientedLineSegment::length( void ) const
{
	return sqrt(pow(this->endPoints[0][0] - this->endPoints[1][0], 2) + pow(this->endPoints[0][1] - this->endPoints[1][1], 2));
}

double OrientedLineSegment::distance( Point2D p ) const
{
	Point2D P = this->endPoints[0];
	Point2D Q = this->endPoints[1];

	Point2D pP = p - P;
	Point2D QP = Q - P;
	double u = pP.dot(QP) / pow(QP.length(), 2);
	
	Point2D QPperp = this->perpendicular();
	double v = pP.dot(QPperp) / QPperp.length();

	double dist = 0;

	if (u < 0)
	{
		dist = (p - this->endPoints[0]).length();
	}
	else if (u > 0)
	{
		dist = (p - this->endPoints[1]).length();
	}
	else
	{
		dist = abs(v);
	}
	return dist;
}
Point2D OrientedLineSegment::perpendicular( void ) const
{
	Point2D temp(this->endPoints[1] - this->endPoints[0]);
	return Point2D(-temp[1], temp[0]);
}

Point2D OrientedLineSegment::GetSourcePosition( const OrientedLineSegment& source , const OrientedLineSegment& destination , Point2D target )
{
	//////////////////////////////////
	// Set the source position here //
	//////////////////////////////////
	THROW( "method undefined" );
	return Point2D();
}