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
#include "NyARTransMat.h"
#include "NyARRotTransOptimize_O2.h"
#include "NyARRotMatrix_ARToolKit.h"
#include "nyarcore.h"
#include <cstdlib>
namespace NyARToolkitCPP
{
#define AR_GET_TRANS_CONT_MAT_MAX_FIT_ERROR 1.0


	NyARTransMat::NyARTransMat()
	{
		this->_center.x=0;
		this->_center.y=0;
		//_calculator,_rotmatrix,_mat_optimizeをコンストラクタの終了後に
		//作成して割り当ててください。
		return;
	}
	NyARTransMat::NyARTransMat(const NyARParam* i_param)
	{
		this->_center.x=0;
		this->_center.y=0;
		const NyARCameraDistortionFactor* dist=i_param->getDistortionFactor();
		const NyARPerspectiveProjectionMatrix* pmat=i_param->getPerspectiveProjectionMatrix();
		this->_calculator=new NyARFitVecCalculator(pmat,dist);
		this->_rotmatrix = new NyARRotMatrix_ARToolKit(pmat);
		this->_mat_optimize=new NyARRotTransOptimize_O2(pmat);
	}
	NyARTransMat::~NyARTransMat()
	{
		NyAR_SAFE_DELETE(this->_calculator);
		NyAR_SAFE_DELETE(this->_rotmatrix);
		NyAR_SAFE_DELETE(this->_mat_optimize);
		return;
	}

	void NyARTransMat::setCenter(double i_x, double i_y)
	{
		this->_center.x= i_x;
		this->_center.y= i_y;
		return;
	}


	void NyARTransMat::initVertexOrder(const NyARSquare& i_square, int i_direction,const TNyARDoublePoint2d* o_sqvertex_ref[],const TNyARLinear* o_liner_ref[])const
	{
		//頂点順序を考慮した矩形の頂点情報
		o_sqvertex_ref[0]= &i_square.sqvertex[(4 - i_direction) % 4];
		o_sqvertex_ref[1]= &i_square.sqvertex[(5 - i_direction) % 4];
		o_sqvertex_ref[2]= &i_square.sqvertex[(6 - i_direction) % 4];
		o_sqvertex_ref[3]= &i_square.sqvertex[(7 - i_direction) % 4];	
		o_liner_ref[0]=&i_square.line[(4 - i_direction) % 4];
		o_liner_ref[1]=&i_square.line[(5 - i_direction) % 4];
		o_liner_ref[2]=&i_square.line[(6 - i_direction) % 4];
		o_liner_ref[3]=&i_square.line[(7 - i_direction) % 4];
		return;
	}


	void NyARTransMat::transMat(const NyARSquare& i_square, int i_direction, double i_width, NyARTransMatResult& o_result)
	{
		const TNyARDoublePoint2d* sqvertex_ref[4];
		const TNyARLinear* linear_ref[4];
		TNyARDoublePoint3d trans;

		//計算用に頂点情報を初期化（順番調整）
		initVertexOrder(i_square, i_direction, sqvertex_ref,linear_ref);

		//基準矩形を設定
		this->_offset.setSquare(i_width,this->_center);

		// rotationを矩形情報から計算
		this->_rotmatrix->initRotBySquare(linear_ref,sqvertex_ref);

		//平行移動量計算機にオフセット頂点をセット
		this->_calculator->setOffsetSquare(&this->_offset);

		//平行移動量計算機に適応先矩形の情報をセット
		this->_calculator->setFittedSquare(sqvertex_ref);	

		//回転行列の平行移動量の計算
		this->_calculator->calculateTransfer(*this->_rotmatrix,trans);

		//計算結果の最適化(this._rotmatrix,trans)
		this->_mat_optimize->optimize(*this->_rotmatrix,trans,*this->_calculator);

		// マトリクスの保存
		this->updateMatrixValue(*this->_rotmatrix,this->_offset.point,trans,o_result);
		return;
	}


	void NyARTransMat::transMatContinue(const NyARSquare& i_square, int i_direction, double i_width, NyARTransMatResult& io_result_conv)
	{
		const TNyARDoublePoint2d* sqvertex_ref[4];
		const TNyARLinear* linear_ref[4];
		TNyARDoublePoint3d trans;

		// io_result_convが初期値なら、transMatで計算する。
		if (!io_result_conv.has_value) {
			this->transMat(i_square, i_direction, i_width, io_result_conv);
			return;
		}

		//基準矩形を設定
		this->_offset.setSquare(i_width,this->_center);

		// rotationを矩形情報を一つ前の変換行列で初期化
		this->_rotmatrix->initRotByPrevResult(io_result_conv);

		//平行移動量計算機に、オフセット頂点をセット
		this->_calculator->setOffsetSquare(&this->_offset);

		//平行移動量計算機に、適応先矩形の情報をセット
		this->_calculator->setFittedSquare(sqvertex_ref);	

		//回転行列の平行移動量の計算
		this->_calculator->calculateTransfer(*this->_rotmatrix,trans);

		//計算結果の最適化(this._rotmatrix,trans)
		const double err=this->_mat_optimize->optimize(*this->_rotmatrix,trans,*this->_calculator);

		//計算結果を保存
		this->updateMatrixValue(*this->_rotmatrix,this->_offset.point, trans,io_result_conv);

		// エラー値が許容範囲でなければTransMatをやり直し
		if (err > AR_GET_TRANS_CONT_MAT_MAX_FIT_ERROR) {
			// rotationを矩形情報で初期化
			this->_rotmatrix->initRotBySquare(linear_ref,sqvertex_ref);
			//回転行列の平行移動量の計算
			this->_calculator->calculateTransfer(*this->_rotmatrix,trans);
			//計算結果の最適化(this._rotmatrix,trans)
			const double err2=this->_mat_optimize->optimize(*this->_rotmatrix,trans,*this->_calculator);
			//エラー値が低かったら値を差換え
			if (err2 < err) {
				// 良い値が取れたら、差換え
				this->updateMatrixValue(*this->_rotmatrix,this->_offset.point, trans,io_result_conv);
			}
		}
		return;
	}

	void NyARTransMat::updateMatrixValue(const NyARRotMatrix& i_rot,const TNyARDoublePoint3d& i_off, const TNyARDoublePoint3d& i_trans,NyARTransMatResult& o_result)
	{
		o_result.m00=i_rot.m00;
		o_result.m01=i_rot.m01;
		o_result.m02=i_rot.m02;
		o_result.m03=i_rot.m00 * i_off.x + i_rot.m01 * i_off.y + i_rot.m02 * i_off.z + i_trans.x;

		o_result.m10 = i_rot.m10;
		o_result.m11 = i_rot.m11;
		o_result.m12 = i_rot.m12;
		o_result.m13 = i_rot.m10 * i_off.x + i_rot.m11 * i_off.y + i_rot.m12 * i_off.z + i_trans.y;

		o_result.m20 = i_rot.m20;
		o_result.m21 = i_rot.m21;
		o_result.m22 = i_rot.m22;
		o_result.m23 = i_rot.m20 * i_off.x + i_rot.m21 * i_off.y + i_rot.m22 * i_off.z + i_trans.z;

		o_result.has_value = true;
		return;
	}
}

