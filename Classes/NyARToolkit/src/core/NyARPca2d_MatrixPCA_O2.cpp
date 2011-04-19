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
#include "NyARPca2d_MatrixPCA_O2.h"
#include <cmath>

#define PCA_EPS 1e-6 // #define EPS 1e-6
#define PCA_MAX_ITER 100 // #define MAX_ITER 100
#define PCA_VZERO 1e-16 // #define VZERO 1e-16

using namespace std;
namespace NyARToolkitCPP
{
	void NyARPca2d_MatrixPCA_O2::PCA_QRM(NyARDoubleMatrix22* o_matrix, TNyARDoublePoint2d* dv)
	{
		double w, t, s, x, y, c;
		double ev1;
		double dv_x,dv_y;
		double mat00,mat01,mat10,mat11;
		// <this.vecTridiagonalize2d(i_mat, dv, ev);>
		dv_x = o_matrix->m00;// this.m[dim - 2][dim - 2];// d.v[dim-2]=a.m[dim-2][dim-2];//d->v[dim-2]=a->m[(dim-2)*dim+(dim-2)];
		ev1 = o_matrix->m01;// this.m[dim - 2][dim - 1];// e.v[dim-2+i_e_start]=a.m[dim-2][dim-1];//e->v[dim-2] = a->m[(dim-2)*dim+(dim-1)];
		dv_y = o_matrix->m11;// this.m[dim - 1][dim - 1];// d.v[dim-1]=a_array[dim-1][dim-1];//d->v[dim-1] =a->m[(dim-1)*dim+(dim-1)];
		// 単位行列にする。
		mat00 = mat11 = 1;
		mat01 = mat10 = 0;
		// </this.vecTridiagonalize2d(i_mat, dv, ev);>

		// int j = 1;
		// // while(j>0 && fabs(ev->v[j])>EPS*(fabs(dv->v[j-1])+fabs(dv->v[j])))
		// while (j > 0 && Math.abs(ev1) > PCA_EPS * (Math.abs(dv.x) + Math.abs(dv.y))) {
		// j--;
		// }
		// if (j == 0) {
		int iter = 0;
		do {
			iter++;
			if (iter > PCA_MAX_ITER) {
				break;
			}
			w = (dv_x - dv_y) / 2;// w = (dv->v[h-1] -dv->v[h]) / 2;//ここ？
			t = ev1 * ev1;// t = ev->v[h] * ev->v[h];
			s = sqrt(w * w + t);
			if (w < 0) {
				s = -s;
			}
			x = dv_x - dv_y + t / (w + s);// x = dv->v[j] -dv->v[h] +t/(w+s);
			y = ev1;// y = ev->v[j+1];

			if (abs(x) >= abs(y)) {
				if (abs(x) > PCA_VZERO) {
					t = -y / x;
					c = 1 / sqrt(t * t + 1);
					s = t * c;
				} else {
					c = 1.0;
					s = 0.0;
				}
			} else {
				t = -x / y;
				s = 1.0 / sqrt(t * t + 1);
				c = t * s;
			}
			w = dv_x - dv_y;// w = dv->v[k] -dv->v[k+1];
			t = (w * s + 2 * c * ev1) * s;// t = (w * s +2 * c *ev->v[k+1]) *s;
			dv_x -= t;// dv->v[k] -= t;
			dv_y += t;// dv->v[k+1] += t;
			ev1 += s * (c * w - 2 * s * ev1);// ev->v[k+1]+= s * (c* w- 2* s *ev->v[k+1]);

			x = mat00;// x = a->m[k*dim+i];
			y = mat10;// y = a->m[(k+1)*dim+i];
			mat00 = c * x - s * y;// a->m[k*dim+i] = c * x - s* y;
			mat10 = s * x + c * y;// a->m[(k+1)*dim+i] = s* x + c * y;

			x = mat01;// x = a->m[k*dim+i];
			y = mat11;// y = a->m[(k+1)*dim+i];
			mat01 = c * x - s * y;// a->m[k*dim+i] = c * x - s* y;
			mat11 = s * x + c * y;// a->m[(k+1)*dim+i] = s* x + c * y;
		} while (abs(ev1) > PCA_EPS * (abs(dv_x) + abs(dv_y)));
		// }

		t = dv_x;// t = dv->v[h];
		if (dv_y > t) {// if( dv->v[i] > t ) {
			t = dv_y;// t = dv->v[h];
			dv_y = dv_x;// dv->v[h] = dv->v[k];
			dv_x = t;// dv->v[k] = t;
			// 行の入れ替え
			o_matrix->m00 = mat10;
			o_matrix->m01 = mat11;
			o_matrix->m10 = mat00;		
			o_matrix->m11 = mat01;

		} else {
			// 行の入れ替えはなし
			o_matrix->m00 = mat00;
			o_matrix->m01 = mat01;
			o_matrix->m10 = mat10;		
			o_matrix->m11 = mat11;
		}
		dv->x=dv_x;
		dv->y=dv_y;
		return;
	}

	/**
	* static int PCA( ARMat *input, ARMat *output, ARVec *ev )
	* 
	* @param output
	* @param o_ev
	* @throws NyARException
	*/
	void NyARPca2d_MatrixPCA_O2::PCA_PCA(const double* i_x,const double* i_y,int i_number_of_data,NyARDoubleMatrix22* o_matrix, TNyARDoublePoint2d* o_ev,TNyARDoublePoint2d* o_mean)const
	{
		// double[] mean_array=mean.getArray();
		// mean.zeroClear();

		//PCA_EXの処理
		double sx = 0;
		double sy = 0;
		for (int i = 0; i < i_number_of_data; i++) {
			sx += i_x[i];
			sy += i_y[i];
		}
		sx = sx / i_number_of_data;
		sy = sy / i_number_of_data;

		//PCA_CENTERとPCA_xt_by_xを一緒に処理
		const double srow = sqrt((double) i_number_of_data);
		double w00, w11, w10;
		w00 = w11 = w10 = 0.0;// *out = 0.0;
		for (int i = 0; i < i_number_of_data; i++) {
			const double x = (i_x[i] - sx) / srow;
			const double y = (i_y[i] - sy) / srow;
			w00 += (x * x);// *out += *in1 * *in2;
			w10 += (x * y);// *out += *in1 * *in2;
			w11 += (y * y);// *out += *in1 * *in2;
		}
		o_matrix->m00=w00;
		o_matrix->m01=o_matrix->m10=w10;
		o_matrix->m11=w11;

		//PCA_PCAの処理
		PCA_QRM(o_matrix, o_ev);
		// m2 = o_output.m;// m2 = output->m;
		if (o_ev->x < PCA_VZERO) {// if( ev->v[i] < VZERO ){
			o_ev->x = 0.0;// ev->v[i] = 0.0;
			o_matrix->m00 = 0.0;// *(m2++) = 0.0;
			o_matrix->m01 = 0.0;// *(m2++) = 0.0;
		}

		if (o_ev->y < PCA_VZERO) {// if( ev->v[i] < VZERO ){
			o_ev->y = 0.0;// ev->v[i] = 0.0;
			o_matrix->m10 = 0.0;// *(m2++) = 0.0;
			o_matrix->m11 = 0.0;// *(m2++) = 0.0;
		}
		o_mean->x=sx;
		o_mean->y=sy;
		// }
		return;
	}
	void NyARPca2d_MatrixPCA_O2::pca(const double* i_x,const double* i_y,int i_number_of_point,NyARDoubleMatrix22* o_evec, TNyARDoublePoint2d* o_ev,TNyARDoublePoint2d* o_mean)const
	{
		PCA_PCA(i_x,i_y,i_number_of_point,o_evec, o_ev,o_mean);

		const double sum = o_ev->x + o_ev->y;
		// For順変更禁止
		o_ev->x /= sum;// ev->v[i] /= sum;
		o_ev->y /= sum;// ev->v[i] /= sum;
		return;	
	}
}
