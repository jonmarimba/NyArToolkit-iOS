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
#pragma once
#include "NyAR_types.h"
#include "INyARRasterFilter_RgbToBin.h"
namespace NyARToolkitCPP
{
	class NyARRasterFilter_ARToolkitThreshold:public INyARRasterFilter_RgbToBin
	{
	private:
		int _threshold;
	public:
		NyARRasterFilter_ARToolkitThreshold(int i_threshold);
		void setThreshold(int i_threshold);
		void doFilter(const INyARRgbRaster& i_input, NyARBinRaster& i_output);
	private:
		void convert24BitRgb(const NyAR_BYTE_t* i_in, int* i_out, const TNyARIntSize* i_size) const;
		void convert32BitRgbx(const NyAR_BYTE_t* i_in, int* i_out, const TNyARIntSize* i_size) const;
		bool checkInputType(int i_input_type) const;
	};
}
