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
#include "NyARVec.h"
namespace NyARToolkitCPP
{


	class NyARMat
	{
	public:
		NyARMat(void);
		NyARMat(int i_row, int i_clm);
		virtual ~NyARMat(void);
	private:
		double* m;
		int m_length;
		int clm,row;
		NyARVec* wk_PCA_QRM_ev;// = new NyARVec(1);
//		NyARVec* wk_vecTridiagonalize_vec;// = new NyARVec(0);
//		NyARVec* wk_vecTridiagonalize_vec2;// = new NyARVec(0);
		NyARMat* wk_work_matrixPCA;// = null;
		NyARMat* wk_PCA_PCA_u;// = null;
	private:
/*<not_implement>
		static void PCA_CENTER(NyARMat inout, NyARVec mean);
		static void PCA_xt_by_x(NyARMat input, NyARMat i_output);
		static void PCA_x_by_xt(NyARMat input, NyARMat output);
		static void PCA_EV_create(NyARMat input, NyARMat u, NyARMat output,NyARVec ev);
		static void arMatrixPCA2(NyARMat input, NyARMat evec, NyARVec ev);
		static double Det_mdet(double* ap, int dimen, int rowa);
		static NyARMat matrixAllocMul(NyARMat a, NyARMat b);
		static double arMatrixDet(NyARMat m);

		void PCA_EX(NyARVec mean);
		void vecTridiagonalize(NyARVec d, NyARVec e, int i_e_start);
		void PCA_QRM(NyARVec dv);
		void flipRow(int i_row_1, int i_row_2);
		void PCA_PCA(NyARMat o_output, NyARVec o_ev);
</not_implement>*/
	public:
		void realloc(int i_row, int i_clm);
		int getClm()const;
		int getRow()const;
		void zeroClear();
		void copyFrom(const NyARMat& i_copy_from);
		double* getArray()const;
		void matrixMul(const NyARMat& a,const NyARMat& b);
		bool matrixSelfInv();
		void dump();
/*<not_implement>
		static void matrixTrans(const NyARMat* dest,const NyARMat* source);
		static void matrixUnit(const NyARMat* unit);
		void matrixDup(const NyARMat* i_source);
		NyARMat* matrixAllocDup();
		static void matrixInv(NyARMat dest, NyARMat source);
		NyARMat* matrixAllocInv();
		static NyARMat matrixAllocUnit(int dim);
		int matrixDisp();
		void matrixPCA(NyARMat o_evec, NyARVec o_ev, NyARVec mean);
</not_implement>*/
	};
}
