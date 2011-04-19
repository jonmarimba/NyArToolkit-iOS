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
#include "NyARBufferReader.h"
#include "nyarcore.h"
#include <cstdlib>
namespace NyARToolkitCPP
{
	NyARBufferReader::NyARBufferReader(const void* i_ref_buffer,int i_buffer_type)
	{
		this->_buffer=i_ref_buffer;
		this->_buffer_type=i_buffer_type;
	}
	NyARBufferReader::~NyARBufferReader(void)
	{
	}
	const void* NyARBufferReader::getBuffer(void) const
	{
		NyAR_ASSERT(this->_buffer!=NULL);
		return this->_buffer;
	}
	void NyARBufferReader::setBuffer(const void* i_ref_buffer)
	{
		this->_buffer=i_ref_buffer;
		return;
	}
	int NyARBufferReader::getBufferType(void) const
	{
		return this->_buffer_type;
	}
	bool NyARBufferReader::isEqualBufferType(int i_type_value) const
	{
		return this->_buffer_type==i_type_value;
	}

}
