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

#include "NyARRotTransOptimize_O2.h"
#include "INyARRotTransOptimize.h"
#include "NyARPerspectiveProjectionMatrix.h"
#include "NyARRotMatrix.h"
#include "NyARFitVecCalculator.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARRotTransOptimize_O2 : public INyARRotTransOptimize
	{
	private:
		const NyARPerspectiveProjectionMatrix* _projection_mat_ref;
	public:
		NyARRotTransOptimize_O2(const NyARPerspectiveProjectionMatrix* i_projection_mat_ref);
		double optimize(NyARRotMatrix& io_rotmat,TNyARDoublePoint3d& io_transvec,const NyARFitVecCalculator& i_calculator)const;
	private:
		double modifyMatrix(NyARRotMatrix& io_rot,TNyARDoublePoint3d& trans,const TNyARDoublePoint3d i_vertex3d[], const TNyARDoublePoint2d i_vertex2d[])const;
	};
}
