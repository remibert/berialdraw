#pragma once
namespace berialdraw
{
	/** The RoundStyle class customizes the visual properties of drawn shapes, such as lines, curves, or polygons. */
	class RoundStyle : public Style
	{
	public:
		/** Constructor */
		RoundStyle();
	
		/** Destructor */	
		~RoundStyle(){}


		/** Serialize the content of widget into json */
		void serialize(JsonIterator & it);

		/** Unserialize the content of widget from json */
		void unserialize(JsonIterator & it);

		/** Copy operator */
		RoundStyle& operator=(const RoundStyle& other);

		/** Set properties with another */
		void set(const RoundStyle & other);

		/** Create new shape style */
		static Style * create();


		/** Get the thickness */
		Dim thickness() const { return m_thickness >> 6; }

		/** Get the thickness with a precision of 64th of a pixel */
		Dim thickness_() const { return m_thickness; }
	
		/** Set the thickness in pixels */
		void thickness(Dim v);

		/** Set the thickness with a precision of 64th of a pixel */
		void thickness_(Dim v);


		/** Get the radius */
		Dim radius() const { return m_radius >> 6; }

		/** Get the radius with a precision of 64th of a pixel */
		Dim radius_() const { return m_radius; }
	
		/** Set the radius in pixels */
		void radius(Dim v);

		/** Set the radius with a precision of 64th of a pixel */
		void radius_(Dim v);

	protected:
/// @cond DOXYGEN_IGNORE
		Dim m_thickness = 0;
		Dim m_radius = 0;
/// @endcond 
	};
}
