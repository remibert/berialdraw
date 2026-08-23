#include "berialdraw_imp.hpp"

namespace berialdraw
{

/** Unserialize the content of widget from json */
void unserialize(JsonIterator & it, SizePolicy & size_policy)
{
	if (it[StyleNames::SIZE_POLICY] ==   "normal"        ) size_policy = SizePolicy::NORMAL_SIZE  ;
	if (it[StyleNames::SIZE_POLICY] ==   "enlarge-width" ) size_policy = SizePolicy::ENLARGE_WIDTH ;
	if (it[StyleNames::SIZE_POLICY] ==   "enlarge-height") size_policy = SizePolicy::ENLARGE_HEIGHT;
	if (it[StyleNames::SIZE_POLICY] ==   "enlarge-all"   ) size_policy = SizePolicy::ENLARGE_ALL   ;
	if (it[StyleNames::SIZE_POLICY] ==   "shrink-width"  ) size_policy = SizePolicy::SHRINK_WIDTH  ;
	if (it[StyleNames::SIZE_POLICY] ==   "shrink-height" ) size_policy = SizePolicy::SHRINK_HEIGHT ;
	if (it[StyleNames::SIZE_POLICY] ==   "shrink-all"    ) size_policy = SizePolicy::SHRINK_ALL    ;
}

/** Serialize the content of widget into json */
void serialize(JsonIterator & it, SizePolicy size_policy)
{
	switch(size_policy)
	{
	case SizePolicy::NORMAL_SIZE   : it[StyleNames::SIZE_POLICY] = "normal"        ; break;
	case SizePolicy::ENLARGE_WIDTH : it[StyleNames::SIZE_POLICY] = "enlarge-width" ; break;
	case SizePolicy::ENLARGE_HEIGHT: it[StyleNames::SIZE_POLICY] = "enlarge-height"; break;
	case SizePolicy::ENLARGE_ALL   : it[StyleNames::SIZE_POLICY] = "enlarge-all"   ; break;
	case SizePolicy::SHRINK_WIDTH  : it[StyleNames::SIZE_POLICY] = "shrink-width"  ; break;
	case SizePolicy::SHRINK_HEIGHT : it[StyleNames::SIZE_POLICY] = "shrink-height" ; break;
	case SizePolicy::SHRINK_ALL    : it[StyleNames::SIZE_POLICY] = "shrink-all"    ; break;
	}
}

}