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
#include "NyARObserv2IdealMap.h"
#include "nyarcore.h"
#include <cmath>
#include <cstdlib>

namespace NyARToolkitCPP
{

	NyARObserv2IdealMap::NyARObserv2IdealMap(const NyARCameraDistortionFactor& i_distfactor,const TNyARIntSize& i_screen_size)
	{
		TNyARDoublePoint2d opoint;
		this->_mapx=new double[i_screen_size.w*i_screen_size.h];
		this->_mapy=new double[i_screen_size.w*i_screen_size.h];
		this->_stride=i_screen_size.w;
		int ptr=i_screen_size.h*i_screen_size.w-1;
		//˜c‚Ýƒ}ƒbƒv‚ð\’z
		for(int i=i_screen_size.h-1;i>=0;i--)
		{
			for(int i2=i_screen_size.w-1;i2>=0;i2--)
			{
				i_distfactor.observ2Ideal(i2,i,opoint);
				this->_mapx[ptr]=opoint.x;
				this->_mapy[ptr]=opoint.y;
				ptr--;
			}
		}
		return;
	}
	NyARObserv2IdealMap::~NyARObserv2IdealMap()
	{
		NyAR_SAFE_DELETE(this->_mapx);
		NyAR_SAFE_DELETE(this->_mapy);
	}

	 void NyARObserv2IdealMap::observ2Ideal(double ix, double iy, TNyARDoublePoint2d& o_point)const
	{
		int idx=(int)ix+(int)iy*this->_stride;
		o_point.x=this->_mapx[idx];
		o_point.y=this->_mapy[idx];
		return;
	}
	 void NyARObserv2IdealMap::observ2IdealBatch(const int i_x_coord[], const int i_y_coord[],int i_start, int i_num, double o_x_coord[],double o_y_coord[])const
	{
		int idx;
		int ptr=0;
		for (int j = 0; j < i_num; j++) {
			idx=i_x_coord[i_start + j]+i_y_coord[i_start + j]*this->_stride;
			o_x_coord[ptr]=this->_mapx[idx];
			o_y_coord[ptr]=this->_mapy[idx];
			ptr++;
		}
		return;
	}	


}
