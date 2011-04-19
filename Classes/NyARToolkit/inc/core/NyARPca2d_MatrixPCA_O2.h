#pragma once
#include "INyARPca2d.h"
namespace NyARToolkitCPP
{
	class NyARPca2d_MatrixPCA_O2 :public INyARPca2d
	{
	private:
		 static void PCA_QRM(NyARDoubleMatrix22* o_matrix, TNyARDoublePoint2d* dv);
		 void PCA_PCA(const double* i_x,const double* i_y,int i_number_of_data,NyARDoubleMatrix22* o_matrix, TNyARDoublePoint2d* o_ev,TNyARDoublePoint2d* o_mean)const;
	public:
		void pca(const double* i_x,const double* i_y,int i_number_of_point,NyARDoubleMatrix22* o_evec, TNyARDoublePoint2d* o_ev,TNyARDoublePoint2d* o_mean)const;
	};
}
