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

namespace NyARToolkitCPP
{
	/*	constant
		--íËêî
	*/
	const static double NyAR_PI=3.1415926535897932384626433832795;

	/*	basic type
		--äÓñ{å^
	*/
	typedef unsigned char NyAR_BYTE_t;

	/*	complex type
		--ï°çáå^
	*/
	typedef struct TNyARDoublePoint2d_t TNyARDoublePoint2d;

	struct TNyARDoublePoint2d_t{
		double x;
		double y;
	};

	typedef struct TNyARDoublePoint3d_t TNyARDoublePoint3d;

	struct TNyARDoublePoint3d_t{
		double x;
		double y;
		double z;
	};

	typedef struct TNyARIntPoint_t TNyARIntPoint;

	struct TNyARIntPoint_t
	{
		int x;
		int y;
	};

	typedef struct TNyARIntRect_t TNyARIntRect;

	struct TNyARIntRect_t
	{
		int l;
		int t;
		int w;
		int h;
	};

	typedef struct TNyARIntSize_t TNyARIntSize;
	struct TNyARIntSize_t
	{
		int h;
		int w;
		 static bool isEqualSize(const TNyARIntSize* a,const TNyARIntSize* b)
		{
			if (a->w == b->w && a->h == b->h){
				return true;
			}
			return false;
		}
	};
	typedef struct TNyARLinear_t TNyARLinear;
	struct TNyARLinear_t
	{
		double rise;//yé≤ÇÃëùâ¡ó 
		double run;//xé≤ÇÃëùâ¡ó 
		double intercept;//êÿï–
	};

	/*	configulation type
		--ÉRÉìÉtÉBÉOå^
	*/
	enum TNyAREndian{
		TNyAREndian_LITTLE,
		TNyAREndian_BIG
	};
}
