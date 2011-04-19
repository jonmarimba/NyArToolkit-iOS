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
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "INyARColorPatt.h"
#include "INyARMatchPatt.h"
#include "../utils/NyArray.h"
namespace NyARToolkitCPP
{
	class NyARMatchPatt_Color_WITHOUT_PCA :public INyARMatchPatt
	{
	private:
		NyArray<int> _input;
		double _datapow;
		int _width;
		int _height;
		double _cf;
		int _dir;
	public:
		NyARMatchPatt_Color_WITHOUT_PCA();
		double getConfidence()const;
		int getDirection()const;
	private:
		 void reallocInputArray(int i_width, int i_height);
	public:
		bool setPatt(const INyARColorPatt& i_target_patt);
		void evaluate(const NyARCode& i_code);
	};
}
