/* 
 * PROJECT: NyARToolkitCPP
 * --------------------------------------------------------------------------------
 *
 * The NyARToolkitCS is C++ version NyARToolkit class library.
 * 
 * Copyright (C)2008 R.Iizuka
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * For further information please contact.
 *	http://nyatla.jp/nyatoolkit/
 *	<airmail(at)ebony.plala.or.jp>
 * 
 */
#include "nyarcore.h"
#include "NyARRgbRaster_BasicClass.h"
namespace NyARToolkitCPP
{
	NyARRgbRaster_BasicClass::NyARRgbRaster_BasicClass(int i_width,int i_height)
	{
		this->_size.w=i_width;
		this->_size.h=i_height;
		return;
	}

	NyARRgbRaster_BasicClass::NyARRgbRaster_BasicClass(const TNyARIntSize& i_size)
	{
		this->_size=i_size;
	}

	 int NyARRgbRaster_BasicClass::getWidth()const
	{
		return this->_size.w;
	}

	 int NyARRgbRaster_BasicClass::getHeight()const
	{
		return this->_size.h;
	}

	 const TNyARIntSize* NyARRgbRaster_BasicClass::getSize()const
	{
		return &this->_size;
	}
}
