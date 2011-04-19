#include "nyarcore.h"
#include "NyARRaster_BasicClass.h"
namespace NyARToolkitCPP
{
	NyARRaster_BasicClass::NyARRaster_BasicClass(int i_width,int i_height)
	{
		this->_size.w=i_width;
		this->_size.h=i_height;
		return;
	}

	NyARRaster_BasicClass::NyARRaster_BasicClass(const TNyARIntSize& i_size)
	{
		this->_size=i_size;
		return;
	}

	 int NyARRaster_BasicClass::getWidth() const
	{
		return this->_size.w;
	}

	 int NyARRaster_BasicClass::getHeight() const
	{
		return this->_size.h;
	}

	 const TNyARIntSize* NyARRaster_BasicClass::getSize() const
	{
		return &this->_size;
	}
}
