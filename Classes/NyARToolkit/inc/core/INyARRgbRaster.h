#pragma once
#include "INyARRaster.h"
#include "INyARRgbPixelReader.h"
namespace NyARToolkitCPP
{
	class INyARRgbRaster :public INyARRaster
	{
	public:
		virtual	const INyARRgbPixelReader* getRgbPixelReader()const=0;
	};
}
