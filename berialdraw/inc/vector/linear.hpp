#pragma once
namespace berialdraw
{
	/** Class representing a linear equation and various related operations */
	class Linear
	{
	public:
		/** Default constructor */
		Linear();

		/** Constructor with two points
		@param p1 First point
		@param p2 Second point */
		Linear(const Point& p1, const Point& p2);

		/** Constructor with coordinates of two points
		@param x1 X-coordinate of the first point
		@param y1 Y-coordinate of the first point
		@param x2 X-coordinate of the second point
		@param y2 Y-coordinate of the second point */
		Linear(Coord x1, Coord y1, Coord x2, Coord y2);

		/** Set the coefficients of the linear equation
		@param x1 X-coordinate of the first point
		@param y1 Y-coordinate of the first point
		@param x2 X-coordinate of the second point
		@param y2 Y-coordinate of the second point
		@return True if coefficients are set successfully, otherwise false */
		bool set_coef(Coord x1, Coord y1, Coord x2, Coord y2);

		/** Get the intersection point of two lines
		@param line Reference to another Linear object
		@param point Reference to the intersection point
		@return True if intersection is found, otherwise false */
		bool get_intersection(Linear& line, Point& point);

		/** Get the Y-coordinate for a given X-coordinate using the linear equation
		@param x X-coordinate
		@return Y-coordinate */
		Coord fx(Coord x);

		/** Get the X-coordinate for a given Y-coordinate using the linear equation
		@param y Y-coordinate
		@return X-coordinate */
		Coord fy(Coord y);

		/** Calculate the cross product of three points
		@param x1 X-coordinate of the first point
		@param y1 Y-coordinate of the first point
		@param x2 X-coordinate of the second point
		@param y2 Y-coordinate of the second point
		@param x3 X-coordinate of the third point
		@param y3 Y-coordinate of the third point
		@return Cross product value */
		static int64_t cross_product(Coord x1, Coord y1, Coord x2, Coord y2, Coord x3, Coord y3);

		/** Calculate the cross product of three points
		@param p1 First point
		@param p2 Second point
		@param p3 Third point
		@return Cross product value */
		static int64_t cross_product(const Point& p1, const Point& p2, const Point& p3);

		/** Calculate the dot product of three points
		@param x1 X-coordinate of the first point
		@param y1 Y-coordinate of the first point
		@param x2 X-coordinate of the second point
		@param y2 Y-coordinate of the second point
		@param x3 X-coordinate of the third point
		@param y3 Y-coordinate of the third point
		@return Dot product value */
		static int64_t dot_product(Coord x1, Coord y1, Coord x2, Coord y2, Coord x3, Coord y3);

		/** Calculate the dot product of three points
		@param p1 First point
		@param p2 Second point
		@param p3 Third point
		@return Dot product value */
		static int64_t dot_product(const Point& p1, const Point& p2, const Point& p3);

		/** Check if a point is within a segment defined by two points
		@param p Point to check
		@param p1 First point of the segment
		@param p2 Second point of the segment
		@return True if the point is within the segment, otherwise false */
		static bool is_in_segment(const Point& p, const Point& p1, const Point& p2);

		/** Check if a point is within a segment defined by two points
		@param x X-coordinate of the point to check
		@param y Y-coordinate of the point to check
		@param x1 X-coordinate of the first point of the segment
		@param y1 Y-coordinate of the first point of the segment
		@param x2 X-coordinate of the second point of the segment
		@param y2 Y-coordinate of the second point of the segment
		@return True if the point is within the segment, otherwise false */
		static bool is_in_segment(Coord x, Coord y, Coord x1, Coord y1, Coord x2, Coord y2);

		/** Add vectors defined by three points
		@param x1 X-coordinate of the first point
		@param y1 Y-coordinate of the first point
		@param x2 X-coordinate of the second point
		@param y2 Y-coordinate of the second point
		@param x3 X-coordinate of the third point
		@param y3 Y-coordinate of the third point
		@param x4 Resulting X-coordinate
		@param y4 Resulting Y-coordinate */
		static void add_vector(Coord x1, Coord y1, Coord x2, Coord y2, Coord x3, Coord y3, Coord & x4, Coord & y4);

		/** Add vectors defined by three points
		@param p1 First point
		@param p2 Second point
		@param p3 Third point
		@param p4 Resulting point */
		static void add_vector(const Point& p1, const Point& p2, const Point& p3, Point & p4);

		/** Calculate the norms of two points
		@param x1 X-coordinate of the first point
		@param y1 Y-coordinate of the first point
		@param x2 X-coordinate of the second point
		@param y2 Y-coordinate of the second point
		@return Norm value */
		static int64_t norms(Coord x1, Coord y1, Coord x2, Coord y2);

		/** Calculate the norms of two points
		@param p1 First point
		@param p2 Second point
		@return Norm value */
		static int64_t norms(const Point& p1, const Point& p2);

#ifdef _DEBUG
		/** Unit test 1 */
		static void test1();

		/** Unit test 2 */
		static void test2();
#endif 

	protected:
		float       a;        /**< Coefficient a of the linear equation */
		float       b;        /**< Coefficient b of the linear equation */
		bool        vertical; /**< Flag indicating if the line is vertical */
	};
}
