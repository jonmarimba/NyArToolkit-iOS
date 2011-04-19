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
#include "NyARVertexCounter.h"
namespace NyARToolkitCPP
{
	/**
	* get_vertex関数を切り離すためのクラス
	* 
	*/
	bool NyARVertexCounter::getVertex(const int i_x_coord[], const int i_y_coord[], int st, int ed, double i_thresh)
	{
		this->number_of_vertex = 0;
		return get_vertex(i_x_coord,i_y_coord,st, ed,i_thresh);
	}
	bool NyARVertexCounter::get_vertex(const int lx_coord[],const int ly_coord[],int st, int ed,double i_thresh)
	{
		//メモ:座標値は65536を超えなければint32で扱って大丈夫なので変更。
		//dmaxは4乗なのでやるとしてもint64じゃないとマズイ
		int v1 = 0;
		int a = ly_coord[ed] - ly_coord[st];
		int b = lx_coord[st] - lx_coord[ed];
		int c = lx_coord[ed] * ly_coord[st] - ly_coord[ed] * lx_coord[st];
		double dmax = 0;
		for (int i = st + 1; i < ed; i++) {
			const double d = a * lx_coord[i] + b * ly_coord[i] + c;
			if (d * d > dmax) {
				dmax = d * d;
				v1 = i;
			}
		}
		if (dmax / (double)(a * a + b * b) > i_thresh) {
			if (!get_vertex(lx_coord,ly_coord,st, v1,i_thresh)) {
				return false;
			}
			if (this->number_of_vertex > 5) {
				return false;
			}
			this->vertex[number_of_vertex] = v1;// vertex[(*vnum)] = v1;
			this->number_of_vertex++;// (*vnum)++;

			if (!get_vertex(lx_coord,ly_coord,v1, ed,i_thresh)) {
				return false;
			}
		}
		return true;
	}
}
