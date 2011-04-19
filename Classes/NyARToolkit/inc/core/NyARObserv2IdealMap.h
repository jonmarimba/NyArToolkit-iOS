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

#include "NyARDoubleMatrix34.h"
#include "NyARCameraDistortionFactor.h"
#include "NyAR_types.h"

namespace NyARToolkitCPP
{
	/**
	* 歪み成分マップを使用するINyARCameraDistortionFactor
	*/
	class NyARObserv2IdealMap
	{
	private:
		int _stride;
		double* _mapx;
		double* _mapy;
	public:
		NyARObserv2IdealMap(const NyARCameraDistortionFactor& i_distfactor,const TNyARIntSize& i_screen_size);
		virtual ~NyARObserv2IdealMap();
	public:
		void observ2Ideal(double ix, double iy, TNyARDoublePoint2d& o_point)const;
		void observ2IdealBatch(const int i_x_coord[], const int i_y_coord[],int i_start, int i_num, double o_x_coord[],double o_y_coord[])const;
	};

}
