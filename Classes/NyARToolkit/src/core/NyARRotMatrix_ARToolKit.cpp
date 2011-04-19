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
#include "NyARRotMatrix_ARToolKit.h"
#include "NyARRotMatrix.h"
#include "NyARRotVector.h"
#include "nyarcore.h"
#include <cmath>
#include <cstdlib>
namespace NyARToolkitCPP
{
	NyARRotMatrix_ARToolKit::NyARRotMatrix_ARToolKit(const NyARPerspectiveProjectionMatrix* i_matrix)
	{
		this->__initRot_vec1=new NyARRotVector(i_matrix);
		this->__initRot_vec2=new NyARRotVector(i_matrix);
		return;
	}
	NyARRotMatrix_ARToolKit::~NyARRotMatrix_ARToolKit()
	{
		NyAR_SAFE_DELETE(this->__initRot_vec1);
		NyAR_SAFE_DELETE(this->__initRot_vec2);
	}
	void NyARRotMatrix_ARToolKit::initRotByPrevResult(const NyARTransMatResult& i_prev_result)
	{

		this->m00=i_prev_result.m00;
		this->m01=i_prev_result.m01;
		this->m02=i_prev_result.m02;

		this->m10=i_prev_result.m10;
		this->m11=i_prev_result.m11;
		this->m12=i_prev_result.m12;

		this->m20=i_prev_result.m20;
		this->m21=i_prev_result.m21;
		this->m22=i_prev_result.m22;
		return;
	}
	void NyARRotMatrix_ARToolKit::initRotBySquare(const TNyARLinear* i_linear[],const TNyARDoublePoint2d* i_sqvertex[])
	{
		NyARRotVector& vec1=*this->__initRot_vec1;
		NyARRotVector& vec2=*this->__initRot_vec2;

		//向かい合った辺から、２本のベクトルを計算

		//軸１
		vec1.exteriorProductFromLinear(*i_linear[0],*i_linear[2]);
		vec1.checkVectorByVertex(*i_sqvertex[0],*i_sqvertex[1]);

		//軸２
		vec2.exteriorProductFromLinear(*i_linear[1],*i_linear[3]);
		vec2.checkVectorByVertex(*i_sqvertex[3],*i_sqvertex[0]);

		//回転の最適化？
		NyARRotVector::checkRotation(vec1,vec2);

		this->m00 =vec1.v1;
		this->m10 =vec1.v2;
		this->m20 =vec1.v3;
		this->m01 =vec2.v1;
		this->m11 =vec2.v2;
		this->m21 =vec2.v3;

		//最後の軸を計算
		const double w02 = vec1.v2 * vec2.v3 - vec1.v3 * vec2.v2;
		const double w12 = vec1.v3 * vec2.v1 - vec1.v1 * vec2.v3;
		const double w22 = vec1.v1 * vec2.v2 - vec1.v2 * vec2.v1;
		const double w = sqrt(w02 * w02 + w12 * w12 + w22 * w22);
		this->m02 = w02/w;
		this->m12 = w12/w;
		this->m22 = w22/w;
		return;
	}



	/*	バグ：m22はセット時に確定できるんじゃないの？
	
	*/
	void NyARRotMatrix_ARToolKit::getAngle(TNyARDoublePoint3d& o_angle)const
	{
		double a,b,c;
		double sina, cosa, sinb,cosb,sinc, cosc;

		if (this->m22 > 1.0) {// <Optimize/>if( rot[2][2] > 1.0 ) {
			cosb = 1.0;// <Optimize/>rot[2][2] = 1.0;
		} else if (this->m22 < -1.0) {// <Optimize/>}else if( rot[2][2] < -1.0 ) {
			cosb = -1.0;// <Optimize/>rot[2][2] = -1.0;
		}else{
			cosb =this->m22;// <Optimize/>cosb = rot[2][2];
		}
		b = acos(cosb);
		sinb =sin(b);
		const double rot02=this->m02;
		const double rot12=this->m12;
		if (b >= 0.000001 || b <= -0.000001) {
			cosa = rot02 / sinb;// <Optimize/>cosa = rot[0][2] / sinb;
			sina = rot12 / sinb;// <Optimize/>sina = rot[1][2] / sinb;
			if (cosa > 1.0) {
				/* printf("cos(alph) = %f\n", cosa); */
				cosa = 1.0;
				sina = 0.0;
			}
			if (cosa < -1.0) {
				/* printf("cos(alph) = %f\n", cosa); */
				cosa = -1.0;
				sina = 0.0;
			}
			if (sina > 1.0) {
				/* printf("sin(alph) = %f\n", sina); */
				sina = 1.0;
				cosa = 0.0;
			}
			if (sina < -1.0) {
				/* printf("sin(alph) = %f\n", sina); */
				sina = -1.0;
				cosa = 0.0;
			}
			a = acos(cosa);
			if (sina < 0) {
				a = -a;
			}
			// <Optimize>
			// sinc = (rot[2][1]*rot[0][2]-rot[2][0]*rot[1][2])/(rot[0][2]*rot[0][2]+rot[1][2]*rot[1][2]);
			// cosc = -(rot[0][2]*rot[2][0]+rot[1][2]*rot[2][1])/(rot[0][2]*rot[0][2]+rot[1][2]*rot[1][2]);
			const double tmp = (rot02 * rot02 + rot12 * rot12);
			sinc = (this->m21 * rot02 - this->m20 * rot12) / tmp;
			cosc = -(rot02 * this->m20 + rot12 * this->m21) / tmp;
			// </Optimize>

			if (cosc > 1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = 1.0;
				sinc = 0.0;
			}
			if (cosc < -1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = -1.0;
				sinc = 0.0;
			}
			if (sinc > 1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = 1.0;
				cosc = 0.0;
			}
			if (sinc < -1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = -1.0;
				cosc = 0.0;
			}
			c = acos(cosc);
			if (sinc < 0) {
				c = -c;
			}
		} else {
			a = b = 0.0;
			cosa = cosb = 1.0;
			sina = sinb = 0.0;
			cosc=this->m00;//cosc = rot[0];// <Optimize/>cosc = rot[0][0];
			sinc=this->m01;//sinc = rot[1];// <Optimize/>sinc = rot[1][0];
			if (cosc > 1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = 1.0;
				sinc = 0.0;
			}
			if (cosc < -1.0) {
				/* printf("cos(r) = %f\n", cosc); */
				cosc = -1.0;
				sinc = 0.0;
			}
			if (sinc > 1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = 1.0;
				cosc = 0.0;
			}
			if (sinc < -1.0) {
				/* printf("sin(r) = %f\n", sinc); */
				sinc = -1.0;
				cosc = 0.0;
			}
			c = acos(cosc);
			if (sinc < 0) {
				c = -c;
			}
		}
		o_angle.x = a;// wa.value=a;//*wa = a;
		o_angle.y = b;// wb.value=b;//*wb = b;
		o_angle.z = c;// wc.value=c;//*wc = c;
		return;
	}
	/**
	* 回転角から回転行列を計算してセットします。
	* @param i_x
	* @param i_y
	* @param i_z
	*/
	void NyARRotMatrix_ARToolKit::setAngle(double i_x,double i_y,double i_z)
	{
		const double sina = sin(i_x);
		const double cosa = cos(i_x);
		const double sinb = sin(i_y);
		const double cosb = cos(i_y);
		const double sinc = sin(i_z);
		const double cosc = cos(i_z);
		// Optimize
		const double CACA = cosa * cosa;
		const double SASA = sina * sina;
		const double SACA = sina * cosa;
		const double SASB = sina * sinb;
		const double CASB = cosa * sinb;
		const double SACACB = SACA * cosb;

		this->m00 = CACA * cosb * cosc + SASA * cosc + SACACB * sinc - SACA * sinc;
		this->m01 = -CACA * cosb * sinc - SASA * sinc + SACACB * cosc - SACA * cosc;
		this->m02 = CASB;
		this->m10 = SACACB * cosc - SACA * cosc + SASA * cosb * sinc + CACA * sinc;
		this->m11 = -SACACB * sinc + SACA * sinc + SASA * cosb * cosc + CACA * cosc;
		this->m12 = SASB;
		this->m20 = -CASB * cosc - SASB * sinc;
		this->m21 = CASB * sinc - SASB * cosc;
		this->m22 = cosb;
		return;
	}
	/**
	* i_in_pointを変換行列で座標変換する。
	* @param i_in_point
	* @param i_out_point
	*/
	void NyARRotMatrix_ARToolKit::getPoint3d(const TNyARDoublePoint3d& i_in_point,TNyARDoublePoint3d& i_out_point)const
	{
		const double x=i_in_point.x;
		const double y=i_in_point.y;
		const double z=i_in_point.z;
		i_out_point.x=this->m00 * x + this->m01 * y + this->m02 * z;
		i_out_point.y=this->m10 * x + this->m11 * y + this->m12 * z;
		i_out_point.z=this->m20 * x + this->m21 * y + this->m22 * z;
		return;
	}
	/**
	* 複数の頂点を一括して変換する
	* @param i_in_point
	* @param i_out_point
	* @param i_number_of_vertex
	*/
	void NyARRotMatrix_ARToolKit::getPoint3dBatch(const TNyARDoublePoint3d i_in_point[],TNyARDoublePoint3d i_out_point[],int i_number_of_vertex)const
	{
		for(int i=i_number_of_vertex-1;i>=0;i--){
			TNyARDoublePoint3d& out_ptr=i_out_point[i];
			const TNyARDoublePoint3d& in_ptr=i_in_point[i];
			out_ptr.x=this->m00 * in_ptr.x + this->m01 * in_ptr.y + this->m02 * in_ptr.z;
			out_ptr.y=this->m10 * in_ptr.x + this->m11 * in_ptr.y + this->m12 * in_ptr.z;
			out_ptr.z=this->m20 * in_ptr.x + this->m21 * in_ptr.y + this->m22 * in_ptr.z;
		}
		return;
	};
}
