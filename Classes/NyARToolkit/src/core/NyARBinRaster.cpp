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
#include "NyARBinRaster.h"
#include "NyARBufferReader.h"
#include "nyarcore.h"
#include <cstdlib>
namespace NyARToolkitCPP
{
	NyARBinRaster::NyARBinRaster(int i_width, int i_height):NyARRaster_BasicClass(i_width,i_height)
	{
		this->_ref_buf = new int[i_height*i_width];
		this->_buffer_reader=new NyARBufferReader(this->_ref_buf,INyARBufferReader::BUFFERFORMAT_INT1D_BIN_8);
	}
	NyARBinRaster::~NyARBinRaster()
	{
		NyAR_SAFE_DELETE(this->_ref_buf);
		NyAR_SAFE_DELETE(this->_buffer_reader);
		return;
	}
	const INyARBufferReader* NyARBinRaster::getBufferReader()const
	{
		return this->_buffer_reader;
	}
}
