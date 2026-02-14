#pragma once
namespace berialdraw
{
	/** The PieStyle class defines the styling for pie-shaped elements, including angle and rope.
	 It is used for widgets like circular progress indicators or charts. */
	class PieStyle : public Style
	{
	public:
		/** Constructor */
		PieStyle();

		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		PieStyle& operator=(const PieStyle& other);

		/** Set properties with another */
		void set(const PieStyle & other);

		/** Create new pie style */
		static Style * create();


		/** Get the start angle */
		Coord start_angle() const { return m_start_angle >> 6; }

		/** Get the start angle with a precision of 64th of a degres */
		Coord start_angle_() const { return m_start_angle; }
	
		/** Set the start angle in pixels */
		void start_angle(Coord v);

		/** Set the start angle with a precision of 64th of a degres */
		void start_angle_(Coord v);



		/** Get the end angle */
		Coord end_angle() const { return m_end_angle >> 6; }

		/** Get the end angle with a precision of 64th of a degres */
		Coord end_angle_() const { return m_end_angle; }
	
		/** Set the end angle in pixels */
		void end_angle(Coord v);

		/** Set the end angle with a precision of 64th of a degres */
		void end_angle_(Coord v);


		/** Get the rope */
		bool rope() const { return m_rope; }

		/** Set the rope */
		void rope(bool v);

	protected:
///< @cond DOXYGEN_IGNORE
		Coord m_end_angle = 0;
		Coord m_start_angle = 0;
		unsigned int m_rope:1;
///< @endcond
	};
}
