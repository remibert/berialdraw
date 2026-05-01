#pragma once
namespace berialdraw
{
	class JsonIterator;

	/** Size policy for grid layout cell */
	enum SizePolicy
	{
		NORMAL_SIZE    =0, ///< Occupies a normal space in the layout

		ENLARGE_WIDTH  =1, ///< Occupies the maximum width space in the layout
		ENLARGE_HEIGHT =2, ///< Occupies the maximum height space in the layout
		ENLARGE_ALL    =3, ///< Occupies the maximum space in all directions of the layout

		SHRINK_WIDTH   =5, ///< Occupies the minimum width space in the layout
		SHRINK_HEIGHT  =6, ///< Occupies the minimum height space in the layout
		SHRINK_ALL     =7, ///< Occupies the minimum space in all directions of the layout
	};

	/** Unserialize the content of widget from json */
	void unserialize(JsonIterator & it, SizePolicy & size_policy);

	/** Serialize the content of widget into json */
	void serialize(JsonIterator & it, SizePolicy size_policy);
}
