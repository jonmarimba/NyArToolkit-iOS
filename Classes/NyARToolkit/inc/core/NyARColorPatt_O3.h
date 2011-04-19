#pragma once
#include "INyARColorPatt.h"
#include "NyARMat.h"
#include "NyARSquare.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	struct T__updateExtpat_struct_t;
	class NyARColorPatt_O3:public INyARColorPatt
	{
	private:
		int* _extpat;
		int _width;
		int _height;
	public:
		NyARColorPatt_O3(int i_width, int i_height);
		virtual ~NyARColorPatt_O3();
		 const int* getPatArray()const;
		 int getWidth()const;
		 int getHeight()const;
	private:
		NyARMat* __get_cpara_a;
		NyARMat* __get_cpara_b;
		NyARMat* __pickFromRaster_cpara;
		T__updateExtpat_struct_t* __updateExtpat;
	private:
		 bool get_cpara(const TNyARIntPoint i_vertex[], NyARMat& o_para);
	private:
		 static void initValue_wk_pickFromRaster_ext_pat2(int* i_ext_pat2, int i_width, int i_height);
	public:
		bool pickFromRaster(const INyARRgbRaster& image, const NyARSquare& i_square);
	private:
		 void updateExtpat(const INyARRgbRaster& image, const NyARMat& i_cpara, int i_xdiv2,int i_ydiv2);
	};
}
