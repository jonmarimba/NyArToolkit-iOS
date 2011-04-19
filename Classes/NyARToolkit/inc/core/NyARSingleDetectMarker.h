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
#include "NyARSingleDetectMarker.h"
#include "NyARParam.h"
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "INyARSquareDetector.h"
#include "INyARTransMat.h"
#include "NyARTransMat.h"
#include "NyARRasterFilter_ARToolkitThreshold.h"
#include "NyARTransMatResult.h"
#include "NyARColorPatt_O3.h"
#include "NyARSquareDetector.h"
#include "NyARSquareStack.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARSingleDetectMarker
	{
	public:
		NyARSingleDetectMarker(const NyARParam& i_param, NyARCode* i_code, double i_marker_width);
		virtual ~NyARSingleDetectMarker();
	private:
		bool _is_continue;
		NyARMatchPatt_Color_WITHOUT_PCA* _match_patt;
		INyARSquareDetector* _square_detect;
		NyARSquareStack* _square_list;// = new NyARSquareStack(AR_SQUARE_MAX);
		NyARCode* _code;
	protected:
		INyARTransMat* _transmat;
	private:
		double _marker_width;
		// åüèoåãâ ÇÃï€ë∂óp
		int _detected_direction;
		double _detected_confidence;
		NyARSquare* _detected_square;
		INyARColorPatt* _patt;
		NyARBinRaster* _bin_raster;
		NyARRasterFilter_ARToolkitThreshold* _tobin_filter;//=new NyARRasterFilter_ARToolkitThreshold(100);
	public:
		bool detectMarkerLite(const INyARRgbRaster& i_raster,int i_threshold);
		void getTransmationMatrix(NyARTransMatResult& o_result)const;
		double getConfidence()const;
		int getDirection()const;
		void setContinueMode(bool i_is_continue);
	};
}
