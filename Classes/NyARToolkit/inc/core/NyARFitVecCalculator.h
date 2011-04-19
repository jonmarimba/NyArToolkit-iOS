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
#include "NyARPerspectiveProjectionMatrix.h"
#include "NyARCameraDistortionFactor.h"
#include "NyARTransOffset.h"
#include "NyARMat.h"
#include "NyARRotMatrix.h"
#include "NyAR_types.h"

namespace NyARToolkitCPP
{
	class NyARFitVecCalculator
	{
	private:
		NyARMat* _mat_b;//3,NUMBER_OF_VERTEX*2
		NyARMat* _mat_a;/*NUMBER_OF_VERTEX,3*/
		NyARMat* _mat_d;
		const NyARPerspectiveProjectionMatrix* _projection_mat;
		const NyARCameraDistortionFactor* _distortionfactor;
	public:
		NyARFitVecCalculator(const NyARPerspectiveProjectionMatrix* i_projection_mat_ref,const NyARCameraDistortionFactor* i_distortion_ref);
		~NyARFitVecCalculator();
	private:
		TNyARDoublePoint2d _fitsquare_vertex[4];
		const NyARTransOffset* _offset_square;
		NyARMat* _mat_e;
		NyARMat* _mat_f;
		NyARMat* __calculateTransferVec_mat_c;
	public:
		void setOffsetSquare(const NyARTransOffset* i_offset);
		const TNyARDoublePoint2d* getFitSquare()const;
		const NyARTransOffset* getOffsetVertex()const;
		void setFittedSquare(const TNyARDoublePoint2d* i_square_vertex[]);
	public:
		void calculateTransfer(const NyARRotMatrix& i_rotation,TNyARDoublePoint3d& o_transfer)const;
	};
}
