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
#include "NyARFitVecCalculator.h"
#include "nyarcore.h"
#include <cstdlib>

namespace NyARToolkitCPP
{
	NyARFitVecCalculator::NyARFitVecCalculator(const NyARPerspectiveProjectionMatrix* i_projection_mat_ref,const NyARCameraDistortionFactor* i_distortion_ref)
	{
		this->_mat_a=new NyARMat(8,3);
		this->_mat_b=new NyARMat(3,8);
		this->_mat_d=new NyARMat(3,3);

		this->_mat_e = new NyARMat(3, 1);
		this->_mat_f = new NyARMat(3, 1);
		this->__calculateTransferVec_mat_c = new NyARMat(8, 1);//NUMBER_OF_VERTEX * 2, 1

		// 変換マトリクスdとbの準備(arGetTransMatSubの一部)
		double* a_array = this->_mat_a->getArray();
		double* b_array = this->_mat_b->getArray();

		//変換用行列のcpara固定値の部分を先に初期化してしまう。
		for (int i = 0; i < 4; i++){
			const int x2 = i * 2;
			a_array[x2*3+0] = b_array[0*8+x2] = i_projection_mat_ref->m00;// mat_a->m[j*6+0]=mat_b->m[num*0+j*2] =cpara[0][0];
			a_array[x2*3+1] = b_array[1*8+x2] = i_projection_mat_ref->m01;// mat_a->m[j*6+1]=mat_b->m[num*2+j*2]=cpara[0][1];
			a_array[(x2 + 1)*3+0] = b_array[0*8+x2 + 1] = 0.0;// mat_a->m[j*6+3] =mat_b->m[num*0+j*2+1]= 0.0;
			a_array[(x2 + 1)*3+1] = b_array[1*8+x2 + 1] = i_projection_mat_ref->m11;// mat_a->m[j*6+4] =mat_b->m[num*2+j*2+1]= cpara[1][1];
			//a_array[x2 + 1][2] = b_array[2][x2 + 1] = cpara[1 * 4 + 2] - o_marker_vertex_2d[i].y;// mat_a->m[j*6+5]=mat_b->m[num*4+j*2+1]=cpara[1][2]-pos2d[j][1];
		}
		this->_projection_mat=i_projection_mat_ref;
		this->_distortionfactor=i_distortion_ref;
		this->_offset_square=NULL;
		return;
	}
	NyARFitVecCalculator::~NyARFitVecCalculator()
	{
		NyAR_SAFE_DELETE(this->_mat_a);
		NyAR_SAFE_DELETE(this->_mat_b);
		NyAR_SAFE_DELETE(this->_mat_d);
		NyAR_SAFE_DELETE(this->_mat_e);
		NyAR_SAFE_DELETE(this->_mat_f);
		NyAR_SAFE_DELETE(this->__calculateTransferVec_mat_c);//NUMBER_OF_VERTEX * 2, 1
		return;
	}
	 void NyARFitVecCalculator::setOffsetSquare(const NyARTransOffset* i_offset)
	{
		this->_offset_square=i_offset;
		return;
	}
	 const TNyARDoublePoint2d* NyARFitVecCalculator::getFitSquare()const
	{
		return this->_fitsquare_vertex;
	}
	 const NyARTransOffset* NyARFitVecCalculator::getOffsetVertex()const
	{
		return this->_offset_square;
	}

	void NyARFitVecCalculator::setFittedSquare(const TNyARDoublePoint2d* i_square_vertex[])
	{
		TNyARDoublePoint2d* vertex=this->_fitsquare_vertex;
		this->_distortionfactor->ideal2ObservBatch(i_square_vertex, vertex,4);
		//		} else {
		//			for (i = 0; i < NUMBER_OF_VERTEX; i++) {
		//				o_marker_vertex_2d[i].x = i_square_vertex[i].x;
		//				o_marker_vertex_2d[i].y = i_square_vertex[i].y;
		//			}
		//		}		


		const double cpara02=this->_projection_mat->m02;
		const double cpara12=this->_projection_mat->m12;		
		NyARMat& mat_d=*this->_mat_d;
		NyARMat& mat_a=*this->_mat_a;
		NyARMat& mat_b=*this->_mat_b;
		double* a_array = mat_a.getArray();
		double* b_array = mat_b.getArray();
		for (int i = 0; i < 4; i++) {
			const int x2 = i * 2;	
			a_array[x2*3+2] = b_array[2*8+x2] = cpara02 - vertex[i].x;// mat_a->m[j*6+2]=mat_b->m[num*4+j*2]=cpara[0][2]-pos2d[j][0];
			a_array[(x2 + 1)*3+2] = b_array[2*8+x2 + 1] = cpara12 - vertex[i].y;// mat_a->m[j*6+5]=mat_b->m[num*4+j*2+1]=cpara[1][2]-pos2d[j][1];
		}
		// mat_d
		mat_d.matrixMul(mat_b, mat_a);
		mat_d.matrixSelfInv();		
		return;
	}

	void NyARFitVecCalculator::calculateTransfer(const NyARRotMatrix& i_rotation,TNyARDoublePoint3d& o_transfer)const
	{
		NyAR_ASSERT(this->_offset_square!=NULL);
		TNyARDoublePoint3d point3d[4];
		const double cpara00=this->_projection_mat->m00;
		const double cpara01=this->_projection_mat->m01;
		const double cpara02=this->_projection_mat->m02;
		const double cpara11=this->_projection_mat->m11;
		const double cpara12=this->_projection_mat->m12;
		const TNyARDoublePoint3d* vertex3d=this->_offset_square->vertex;		
		const TNyARDoublePoint2d* vertex2d=this->_fitsquare_vertex;
		NyARMat& mat_c = *this->__calculateTransferVec_mat_c;// 次処理で値をもらうので、初期化の必要は無い。

		double* f_array = this->_mat_f->getArray();
		double* c_array = mat_c.getArray();


		//（3D座標？）を一括請求
		i_rotation.getPoint3dBatch(vertex3d,point3d,4);
		for (int i = 0; i < 4; i++) {
			const int x2 = i+i;
			const TNyARDoublePoint3d& point3d_ptr=point3d[i];
			//			i_rotation.getPoint3d(vertex3d[i],point3d);
			//透視変換？
			c_array[x2*1+0] = point3d_ptr.z * vertex2d[i].x - cpara00 * point3d_ptr.x - cpara01 * point3d_ptr.y - cpara02 * point3d_ptr.z;// mat_c->m[j*2+0] = wz*pos2d[j][0]-cpara[0][0]*wx-cpara[0][1]*wy-cpara[0][2]*wz;
			c_array[(x2 + 1)*1+0] = point3d_ptr.z * vertex2d[i].y - cpara11 * point3d_ptr.y - cpara12 * point3d_ptr.z;// mat_c->m[j*2+1]= wz*pos2d[j][1]-cpara[1][1]*wy-cpara[1][2]*wz;
		}
		this->_mat_e->matrixMul(*this->_mat_b, mat_c);
		this->_mat_f->matrixMul(*this->_mat_d, *this->_mat_e);

		// double[] trans=wk_arGetTransMatSub_trans;//double trans[3];
		o_transfer.x= f_array[0*1+0];// trans[0] = mat_f->m[0];
		o_transfer.y= f_array[1*1+0];
		o_transfer.z= f_array[2*1+0];// trans[2] = mat_f->m[2];
		return;
	}
}
