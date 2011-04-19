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
#include "NyARDoubleMatrix22.h"
namespace NyARToolkitCPP
{
	 void NyARDoubleMatrix22::setValue(const double i_value[])
	{
		this->m00=i_value[0];
		this->m01=i_value[1];
		this->m10=i_value[3];
		this->m11=i_value[4];
		return;
	}
	 void NyARDoubleMatrix22::getValue(double o_value[])const
	{
		o_value[0]=this->m00;
		o_value[1]=this->m01;
		o_value[3]=this->m10;
		o_value[4]=this->m11;
		return;
	}
}
