#include "berialdraw_imp.hpp"

namespace berialdraw
{

/** Unserialize the content of widget from json */
void unserialize(JsonIterator & it, SizePolicy & size_policy)
{
	if (it["size-policy"] ==   "normal"        ) size_policy = SizePolicy::NORMAL_SIZE  ;
	if (it["size-policy"] ==   "enlarge-width" ) size_policy = SizePolicy::ENLARGE_WIDTH ;
	if (it["size-policy"] ==   "enlarge-height") size_policy = SizePolicy::ENLARGE_HEIGHT;
	if (it["size-policy"] ==   "enlarge-all"   ) size_policy = SizePolicy::ENLARGE_ALL   ;
	if (it["size-policy"] ==   "shrink-width"  ) size_policy = SizePolicy::SHRINK_WIDTH  ;
	if (it["size-policy"] ==   "shrink-height" ) size_policy = SizePolicy::SHRINK_HEIGHT ;
	if (it["size-policy"] ==   "shrink-all"    ) size_policy = SizePolicy::SHRINK_ALL    ;
}

/** Serialize the content of widget into json */
void serialize(JsonIterator & it, SizePolicy size_policy)
{
	switch(size_policy)
	{
	case SizePolicy::NORMAL_SIZE   : it["size-policy"] = "normal"        ; break;
	case SizePolicy::ENLARGE_WIDTH : it["size-policy"] = "enlarge-width" ; break;
	case SizePolicy::ENLARGE_HEIGHT: it["size-policy"] = "enlarge-height"; break;
	case SizePolicy::ENLARGE_ALL   : it["size-policy"] = "enlarge-all"   ; break;
	case SizePolicy::SHRINK_WIDTH  : it["size-policy"] = "shrink-width"  ; break;
	case SizePolicy::SHRINK_HEIGHT : it["size-policy"] = "shrink-height" ; break;
	case SizePolicy::SHRINK_ALL    : it["size-policy"] = "shrink-all"    ; break;
	}
}

}