#pragma once
#include "NyARPerspectiveProjectionMatrix.h"
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
#include "NyARCameraDistortionFactor.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	typedef struct NyARParamFileStruct NyARParamFileStruct_t;
	struct NyARParamFileStruct{
		int x;
		int y;
		double projection[12];
		double distortion[4];
	};


	class NyARParam
	{
	protected:
		TNyARIntSize _screen_size;
		TNyAREndian _endian;
	private:
		static const int SIZE_OF_PARAM_SET = 4 + 4 + (3 * 4 * 8) + (4 * 8);
		NyARCameraDistortionFactor _dist;
		NyARPerspectiveProjectionMatrix _projection_matrix;
	public:
		NyARParam();
		virtual ~NyARParam(){};
	public:
		void setEndian(TNyAREndian i_new_endian);
	public:
		const TNyARIntSize* getScreenSize()const;
		const NyARPerspectiveProjectionMatrix* getPerspectiveProjectionMatrix()const;
		const NyARCameraDistortionFactor* getDistortionFactor()const;
		void loadARParamFromFile(const char* i_filename);
		void changeScreenSize(int i_xsize, int i_ysize);
		void loadARParam(const NyARParamFileStruct_t& i_stream);
		void saveARParam();
	};
}
