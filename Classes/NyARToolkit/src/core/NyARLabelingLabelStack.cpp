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
#include "NyARLabelingLabelStack.h"

namespace NyARToolkitCPP
{
	NyARLabelingLabelStack::NyARLabelingLabelStack(int i_length):NyObjectStack<NyARLabelingLabel>(i_length)
	{
	}
	NyARLabelingLabelStack::~NyARLabelingLabelStack(void)
	{
	}

	 void NyARLabelingLabelStack::sortByArea()
	{
		const int len=this->_length;
		int h = len *13/10;
		NyARLabelingLabel** item=this->_items->item;
		for(;;){
			int swaps = 0;
			for (int i = 0; i + h < len; i++) {
				if (item[i + h]->area > item[i]->area) {
					NyARLabelingLabel* temp = item[i + h];
					item[i + h] = item[i];
					item[i] = temp;
					swaps++;
				}
			}
			if (h == 1) {
				if (swaps == 0){
					break;
				}
			}else{
				h=h*10/13;
			}
		}
		return;
	} 
}
