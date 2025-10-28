#ifndef __SSPLAYER_CELLMAP__
#define __SSPLAYER_CELLMAP__

#include <TPL/GenericLibrary/STL/TPLStlAllocate.h>

#include "../../SS6PlayerTypes.h"

namespace ss
{

/**
* SsCell
*/
struct SsCell
{
	float		pivot_X;		//原点補正
	float		pivot_Y;		//原点補正
	TextuerData texture;
	ss::SSRect	rect;
	TPL::StlString texname;
	int			cellIndex;
	TPL::StlString cellName;
	float		u1;
	float		v1;
	float		u2;
	float		v2;

	SsCell():
		  pivot_X(0)
		, pivot_Y(0)
		, cellIndex(-1)
		, u1(0)
		, v1(0)
		, u2(0)
		, v2(0)
	{}
};

///パーツが使用するセルの情報
struct SsCellValue
{
	SsRenderBlendType::_enum    blendType;
	SsCell						refCell;



	SsCellValue() :  
		blendType(SsRenderBlendType::_enum::Add)
		{}
};

};
#endif
