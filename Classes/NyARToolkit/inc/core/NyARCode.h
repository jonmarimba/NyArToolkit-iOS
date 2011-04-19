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
	class NyARCode
	{
	private:
		int* pat;
		double patpow[4];// static double patpow[AR_PATT_NUM_MAX][4];
		short* patBW;// static int patBW[AR_PATT_NUM_MAX][4][AR_PATT_SIZE_Y*AR_PATT_SIZE_X*3];
		double patpowBW[4];// static double patpowBW[AR_PATT_NUM_MAX][4];
		int width, height;
	public:
		const int* getPat()const;
		const double* getPatPow()const;
		const short* getPatBW()const;
		const double* getPatPowBW()const;
		int getWidth()const;
		int getHeight()const;
		NyARCode(int i_width, int i_height);
		virtual ~NyARCode();
		void loadARPattFromFile(const char* i_filename);
		void loadARPatt(const int* i_data_array,int i_width,int i_height);
	};

}
