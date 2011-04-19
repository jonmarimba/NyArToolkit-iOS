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

#include "NyARSquareDetector.h"
#include "NyARLabeling_ARToolKit.h"
#include "NyARLabelingImage.h"
#include "NyARObserv2IdealMap.h"
#include "INyARSquareDetector.h"
#include "NyARVertexCounter.h"
#include "INyARPca2d.h"
#include "NyARPca2d_MatrixPCA_O2.h"
#include "../utils/NyArray.h"
#include "nyarcore.h"
#include <cstring>
namespace NyARToolkitCPP
{
	class OverlapChecker;
	class NyARSquareDetector : public INyARSquareDetector
	{
	private:
		int _width;
		int _height;
		NyARLabelingImage* _limage;
		NyARObserv2IdealMap* _dist_factor;
		NyARLabeling_ARToolKit* _labeling;
		INyARPca2d* _pca;

		double* _xpos;
		double* _ypos;
		int* _xcoord;
		int* _ycoord;
		OverlapChecker* _overlap_checker;
		NyARVertexCounter __getSquareVertex_wv1;
		NyARVertexCounter __getSquareVertex_wv2;

		/**
		* 最大i_squre_max個のマーカーを検出するクラスを作成する。
		* 
		* @param i_param
		*/
	public:
		NyARSquareDetector(const NyARCameraDistortionFactor& i_dist_factor_ref,const TNyARIntSize& i_size);
		~NyARSquareDetector();
	private:
		int _max_coord;

		void normalizeCoord(int i_coord_x[], int i_coord_y[], int i_index, int i_coord_num);
	public:
		void detectMarker(const NyARBinRaster& i_raster, NyARSquareStack& o_square_stack);
	private:
		int scanVertex(const int i_xcoord[], const int i_ycoord[], int i_coord_num);
		bool getSquareVertex(const int i_x_coord[], const int i_y_coord[], int i_vertex1_index, int i_coord_num, int i_area, int o_vertex[]);
	private:
		NyARDoubleMatrix22 __getSquareLine_evec;
	private:
		bool getSquareLine(const int i_mkvertex[], const int i_xcoord[],const int i_ycoord[], NyARSquare& o_square);
	};



}
