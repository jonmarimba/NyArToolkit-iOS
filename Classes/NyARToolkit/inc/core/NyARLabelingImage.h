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
#include "INyARBufferReader.h"
#include "NyARLabelingLabelStack.h"
#include "NyARRaster_BasicClass.h"
#include "NyARBufferReader.h"
#include "INyARLabelingImage.h"
#include "NyAR_types.h"
namespace NyARToolkitCPP
{
	class NyARLabelingImage:public INyARLabelingImage
	{
	/*INyARRaster*/
	protected:
		TNyARIntSize _size;
	public:
		int getWidth()const;
		int getHeight()const;
		const TNyARIntSize* getSize()const;
		const INyARBufferReader* getBufferReader()const;
	/*INyARLabelingImage*/
	protected:
		const int* _ref_buf;
	private:
		INyARBufferReader* _buffer_reader;
	protected:
		NyARLabelingLabelStack* _label_list;
		NyArray<int>* _index_table;
		bool _is_index_table_enable;
	public:
		const NyArray<int>* getIndexArray()const;
		NyARLabelingLabelStack* getLabelStack()const;
	/*NyARLabelingImage*/
	public:
		NyARLabelingImage(int i_width, int i_height);
		virtual ~NyARLabelingImage();
	protected:
		static int _getContour_xdir[];
		static int _getContour_ydir[];
	public:
		int getTopClipTangentX(const NyARLabelingLabel& i_label)const;
		int getContour(int i_index,int i_array_size,int o_coord_x[],int o_coord_y[])const;
		void reset(bool i_label_index_enable);
	};
}
