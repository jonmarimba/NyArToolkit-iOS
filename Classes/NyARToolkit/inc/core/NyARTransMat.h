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
#include "INyARTransMat.h"
#include "NyARTransOffset.h"
#include "NyARFitVecCalculator.h"
#include "INyARRotTransOptimize.h"
#include "NyARParam.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARTransMat:public INyARTransMat
	{
	private:
		NyARTransOffset _offset;
		TNyARDoublePoint2d _center;
	protected:
		NyARRotMatrix* _rotmatrix;
		NyARFitVecCalculator* _calculator;
		INyARRotTransOptimize* _mat_optimize;
	protected:
		NyARTransMat();
	public:
		NyARTransMat(const NyARParam* i_param);
		virtual ~NyARTransMat();
		void setCenter(double i_x, double i_y);
		void transMat(const NyARSquare& i_square, int i_direction, double i_width, NyARTransMatResult& o_result);
		void transMatContinue(const NyARSquare& i_square, int i_direction, double i_width, NyARTransMatResult& io_result_conv);
	private:
		void initVertexOrder(const NyARSquare& i_square, int i_direction,const TNyARDoublePoint2d* o_sqvertex_ref[],const TNyARLinear* o_liner_ref[])const;
	private:
		void updateMatrixValue(const NyARRotMatrix& i_rot,const TNyARDoublePoint3d& i_off, const TNyARDoublePoint3d& i_trans,NyARTransMatResult& o_result);
	};

}
