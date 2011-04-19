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
#include "NyARDoubleMatrix33.h"
#include "NyARTransMatResult.h"
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
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARRotMatrix :public NyARDoubleMatrix33
	{	
		/**
		* NyARTransMatResultÇÃì‡óeÇ©ÇÁNyARRotMatrixÇïúå≥ÇµÇ‹Ç∑ÅB
		* @param i_prev_result
		*/
	public:
		virtual void initRotByPrevResult(const NyARTransMatResult& i_prev_result)=0;
		virtual void initRotBySquare(const TNyARLinear* i_linear[],const TNyARDoublePoint2d* i_sqvertex[])=0;
		virtual void getAngle(TNyARDoublePoint3d& o_angle)const=0;
		virtual void setAngle(double i_x,double i_y,double i_z)=0;
		virtual void getPoint3d(const TNyARDoublePoint3d& i_in_point,TNyARDoublePoint3d& i_out_point)const=0;
		virtual void getPoint3dBatch(const TNyARDoublePoint3d i_in_point[],TNyARDoublePoint3d i_out_point[],int i_number_of_vertex)const=0;
	};
}
