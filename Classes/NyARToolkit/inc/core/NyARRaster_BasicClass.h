#pragma once
#include "INyARRaster.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARRaster_BasicClass:public INyARRaster
	{
	protected:
		TNyARIntSize _size;
	protected:
		NyARRaster_BasicClass(const TNyARIntSize& i_size);
		NyARRaster_BasicClass(int i_width,int i_height);
	public:
		 int getWidth() const;
		 int getHeight() const;
		 const TNyARIntSize* getSize() const;
	};
}
