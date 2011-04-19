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
#include "NyARMat.h"
#include "nyarcore.h"
#include <cstddef>
#include <cstring>
#include <cmath>
#include <cstdio>
#define PCA_EPS = 1e-6; // #define EPS 1e-6
#define PCA_MAX_ITER = 100; // #define MAX_ITER 100
#define PCA_VZERO = 1e-16; // #define VZERO 1e-16
namespace NyARToolkitCPP
{
	NyARMat::NyARMat(int i_row, int i_clm)
	{
		this->wk_PCA_QRM_ev=new NyARVec(1);// = new NyARVec(1);
//		this->wk_vecTridiagonalize_vec=new NyARVec(0);// = new NyARVec(0);
//		this->wk_vecTridiagonalize_vec2=new NyARVec(0);// = new NyARVec(0);
		this->wk_work_matrixPCA=NULL;// = null;
		this->wk_PCA_PCA_u=NULL;// = null;
		
		this->m = new double[i_row*i_clm];
		this->clm = i_clm;
		this->row = i_row;
		return;
	}
	NyARMat::~NyARMat(void)
	{
		NyAR_SAFE_DELETE(this->wk_PCA_QRM_ev);
//		NyAR_SAFE_DELETE(this->wk_vecTridiagonalize_vec);
//		NyAR_SAFE_DELETE(this->wk_vecTridiagonalize_vec2);
		NyAR_SAFE_DELETE(this->wk_work_matrixPCA);
		NyAR_SAFE_DELETE(this->wk_PCA_PCA_u);
		NyAR_SAFE_DELETE(this->m);
		return;
	}
	 void NyARMat::realloc(int i_row, int i_clm)
	{
		if(i_row*i_clm>this->m_length){
			// 十分な配列があれば何もしない。
		} else {
			// 不十分なら取り直す。
			NyAR_SAFE_DELETE(this->m);
			this->m = new double[i_row*i_clm];
			this->m_length=i_row*i_clm;
		}
		this->clm = i_clm;
		this->row = i_row;
		return;
	}
	 int NyARMat::getClm()const
	{
		return this->clm;
	}
	 int NyARMat::getRow()const
	{
		return this->row;
	}
	 void NyARMat::zeroClear()
	{
		double* m_ptr=this->m;
		for(int i=0;i<this->m_length;i++){
			*(m_ptr+i)=0;
		}
		return;
	}
	 void NyARMat::copyFrom(const NyARMat& i_copy_from)
	{
		// サイズ確認
		if (this->row != i_copy_from.row || this->clm != i_copy_from.clm){
			throw NyARException();
		}
		// 値コピー
		memcpy(this->m,i_copy_from.m,sizeof(double)*this->row*this->clm);
		return;
	}
	 double* NyARMat::getArray()const
	{
		return this->m;
	}
	 void NyARMat::matrixMul(const NyARMat& a,const NyARMat& b)
	{
		const int d_row=this->row;
		const int d_clm=this->clm;
		if (a.clm != b.row || d_row != a.row || d_clm != b.clm) {
			throw NyARException();
		}
		double w;
		int r, c, i;
		double *dm = this->m;
		const double *am_ptr,*bm_ptr;
		const int a_clm=a.clm;

		// For順変更禁止
		am_ptr=a.m;
		for (r = 0; r < d_row; r++) {
			for (c = 0; c < d_clm; c++) {
				bm_ptr=b.m+c;
				w = 0.0;// dest.setARELEM0(r, c,0.0);
				for (i = 0; i < a.clm; i++) {
					w += *(am_ptr+i) * *bm_ptr;// ARELEM0(dest, r, c) +=ARELEM0(a, r, i) * ARELEM0(b,i, c);
					bm_ptr+=d_clm;
				}
				*dm=w;
				dm++;
			}
			am_ptr+=a_clm;
		}
		return;
	}
	bool NyARMat::matrixSelfInv()
	{
		int nos[50];
		double* ap = this->m;
		const int dimen = this->row;
		int dimen_1 = dimen - 1;
		double *ap_n, *ap_ip, *ap_i;// wap;
		int j, ip, nwork;
		// double epsl;
		double p, pbuf, work;

		/* check size */
		switch (dimen) {
		case 0:
			throw NyARException();
		case 1:
			ap[0*dimen+0] = 1.0 / ap[0*dimen+0];// *ap = 1.0 / (*ap);
			return true;/* 1 dimension */
		}

		for (int n = 0; n < dimen; n++) {
			*(nos+n) = n;
		}

		/*
		 * nyatla memo ipが定まらないで計算が行われる場合があるので挿入。 ループ内で0初期化していいかが判らない。
		 */
		ip = 0;
		// For順変更禁止
		for (int n = 0; n < dimen; n++) {
			ap_n = ap+n*dimen;// wcp = ap + n * rowa;
			p = 0.0;
			for (int i = n; i < dimen; i++) {// for(i = n, wap = wcp, p =
												// 0.0; i < dimen ; i++, wap +=
												// rowa)
				if (p < (pbuf = fabs(ap[i*dimen+0]))) {
					p = pbuf;
					ip = i;
				}
			}
			// if (p <= matrixSelfInv_epsl){
			if (p == 0.0) {
				return false;
				// throw NyARException();
			}

			nwork = nos[ip];
			nos[ip] = nos[n];
			nos[n] = nwork;

			ap_ip = ap+ip*dimen;
			for (j = 0; j < dimen; j++) {// for(j = 0, wap = ap + ip * rowa,
											// wbp = wcp; j < dimen ; j++) {
				work = ap_ip[j]; // work = *wap;
				ap_ip[j] = ap_n[j];
				ap_n[j] = work;
			}

			work = ap_n[0];
			for (j = 0; j < dimen_1; j++) {// for(j = 1, wap = wcp, work =
											// *wcp; j < dimen ; j++, wap++)
				ap_n[j] = ap_n[j + 1] / work;// *wap = *(wap + 1) / work;
			}
			ap_n[j] = 1.0 / work;// *wap = 1.0 / work;
			for (int i = 0; i < dimen; i++) {
				if (i != n) {
					ap_i = ap+i*dimen;// wap = ap + i * rowa;

					work = ap_i[0];
					for (j = 0; j < dimen_1; j++) {// for(j = 1, wbp = wcp,work = *wap;j < dimen ;j++, wap++, wbp++)
						ap_i[j] = ap_i[j + 1] - work * ap_n[j];// wap = *(wap +1) - work *(*wbp);
					}
					ap_i[j] = -work * ap_n[j];// *wap = -work * (*wbp);
				}
			}
		}

		for (int n = 0; n < dimen; n++) {
			for (j = n; j < dimen; j++) {
				if (nos[j] == n) {
					break;
				}
			}
			nos[j] = nos[n];
			for (int i = 0; i < dimen; i++) {// for(i = 0, wap = ap + j, wbp
												// = ap + n; i < dimen ;i++, wap
												// += rowa, wbp += rowa) {
				ap_i = ap+i*dimen;
				work = ap_i[j];// work = *wap;
				ap_i[j] = ap_i[n];// *wap = *wbp;
				ap_i[n] = work;// *wbp = work;
			}
		}
		return true;
	}
	void NyARMat::dump()
	{
		const double* m_ptr=this->m;
		for(int i=0;i<this->clm;i++){
			for(int i2=0;i2<this->row;i2++){
				printf("%f ",*(m_ptr++));
			}
			printf("\n");
		}
	}

/*
	void NyARMat::matrixTrans(NyARMat* dest, NyARMat* source)
	{
		NyARException::trap("未チェックのパス");
		return;
	}
	void NyARMat::matrixUnit(NyARMat* unit)
	{
		NyARException::trap("未チェックのパス");
		return;
	}

	void NyARMat::PCA_CENTER(NyARMat inout, NyARVec mean);
	void NyARMat::PCA_xt_by_x(NyARMat input, NyARMat i_output);
	void NyARMat::PCA_x_by_xt(NyARMat input, NyARMat output);
	void NyARMat::PCA_EV_create(NyARMat input, NyARMat u, NyARMat output,NyARVec ev);
	void NyARMat::arMatrixPCA2(NyARMat input, NyARMat evec, NyARVec ev);
	double NyARMat::Det_mdet(double[][] ap, int dimen, int rowa);
	NyARMat NyARMat::matrixAllocMul(NyARMat a, NyARMat b);
	double NyARMat::arMatrixDet(NyARMat m);

	void NyARMat::PCA_EX(NyARVec mean);
	void NyARMat::vecTridiagonalize(NyARVec d, NyARVec e, int i_e_start);
	void NyARMat::PCA_QRM(NyARVec dv);
	void NyARMat::flipRow(int i_row_1, int i_row_2);
	void NyARMat::PCA_PCA(NyARMat o_output, NyARVec o_ev);
	static void NyARMat::matrixUnit(NyARMat unit);
	void NyARMat::matrixDup(NyARMat i_source);
	NyARMat NyARMat::matrixAllocDup();
	void NyARMat::matrixInv(NyARMat dest, NyARMat source)
		NyARMat NyARMat::matrixAllocInv();
	static NyARMat NyARMat::matrixAllocUnit(int dim);
	int NyARMat::matrixDisp();
	void NyARMat::matrixPCA(NyARMat o_evec, NyARVec o_ev, NyARVec mean);*/
}
