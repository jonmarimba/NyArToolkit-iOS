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
#include "INyARRgbPixelReader.h"
#include "NyARBufferReader.h"
#include "NyARRgbRaster_BasicClass.h"

namespace NyARToolkitCPP
{
	class NyARRgbRaster_BGRA:public NyARRgbRaster_BasicClass
	{
	private:
		class PixelReader : public INyARRgbPixelReader
		{
		private:
			const NyARRgbRaster_BGRA* _parent;

		public:
			PixelReader(const NyARRgbRaster_BGRA* i_parent)
			{
				this->_parent = i_parent;
			}
			 void getPixel(int i_x, int i_y, int* o_rgb)const
			{
				const NyAR_BYTE_t* ref_buf = this->_parent->_ref_buf;
				int bp = (i_x + i_y * this->_parent->_size.w) * 4;
				o_rgb[0] = (ref_buf[bp + 2] & 0xff);// R
				o_rgb[1] = (ref_buf[bp + 1] & 0xff);// G
				o_rgb[2] = (ref_buf[bp + 0] & 0xff);// B
				return;
			}
			 void getPixelSet(const int* i_x, const int* i_y, int i_num, int* o_rgb)const
			{
				int width = _parent->_size.w;
				const NyAR_BYTE_t* ref_buf = _parent->_ref_buf;
				int bp;
				for (int i = i_num - 1; i >= 0; i--) {
					bp = (i_x[i] + i_y[i] * width) * 4;
					o_rgb[i * 3 + 0] = (ref_buf[bp + 2] & 0xff);// R
					o_rgb[i * 3 + 1] = (ref_buf[bp + 1] & 0xff);// G
					o_rgb[i * 3 + 2] = (ref_buf[bp + 0] & 0xff);// B
				}
			}
		};
	private:
		INyARRgbPixelReader* _rgb_reader;
		NyARBufferReader* _buffer_reader;
		const NyAR_BYTE_t* _ref_buf;
	public:
		static NyARRgbRaster_BGRA* wrap(const NyAR_BYTE_t* i_buffer, int i_width, int i_height);
	private:
		NyARRgbRaster_BGRA(const NyAR_BYTE_t* i_buffer, int i_width, int i_height);
	public:
		NyARRgbRaster_BGRA(int i_width, int i_height);
		virtual ~NyARRgbRaster_BGRA();
	public:
		void setBuffer(const NyAR_BYTE_t* i_buffer);
		const INyARRgbPixelReader* getRgbPixelReader()const;
		const INyARBufferReader* getBufferReader()const;
	};
}
