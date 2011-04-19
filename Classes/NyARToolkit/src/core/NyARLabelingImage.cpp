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
#include "nyarcore.h"
#include "NyARLabelingImage.h"
#include <cstdlib>
#define MAX_LABELS 1024*32

namespace NyARToolkitCPP
{
	void f(){
		NyARLabelingImage a(1,2);
	}
	 int NyARLabelingImage::getWidth()const
	{
		return this->_size.w;
	}

	 int NyARLabelingImage::getHeight()const
	{
		return this->_size.h;
	}

	 const TNyARIntSize* NyARLabelingImage::getSize()const
	{
		return &this->_size;
	}


	int NyARLabelingImage::_getContour_xdir[]={ 0, 1, 1, 1, 0,-1,-1,-1};
	int NyARLabelingImage::_getContour_ydir[]={-1,-1, 0, 1, 1, 1, 0,-1};

	NyARLabelingImage::NyARLabelingImage(int i_width, int i_height)
	{
		this->_size.w=i_width;
		this->_size.h=i_height;
		//
		this->_ref_buf =new int[i_height*i_width];
		this->_label_list = new NyARLabelingLabelStack(MAX_LABELS);
		this->_index_table=new NyArray<int>(MAX_LABELS);
		this->_is_index_table_enable=false;
		this->_buffer_reader=new NyARBufferReader(this->_ref_buf,INyARBufferReader::BUFFERFORMAT_INT1D);
		return;
	}
	NyARLabelingImage::~NyARLabelingImage()
	{
		NyAR_SAFE_DELETE(this->_buffer_reader);
		NyAR_SAFE_DELETE(this->_index_table);
		NyAR_SAFE_DELETE(this->_label_list);
		NyAR_SAFE_DELETE(this->_ref_buf);
		return;
	}

	 const INyARBufferReader* NyARLabelingImage::getBufferReader()const
	{
		return this->_buffer_reader;
	}

	 const NyArray<int>* NyARLabelingImage::getIndexArray()const
	{
		return this->_is_index_table_enable?this->_index_table:NULL;
	}

	 NyARLabelingLabelStack* NyARLabelingImage::getLabelStack()const
	{
		return this->_label_list;
	}
	 void NyARLabelingImage::reset(bool i_label_index_enable)
	{
		NyAR_ASSERT(i_label_index_enable==true);//非ラベルモードは未実装
		this->_label_list->clear();
		this->_is_index_table_enable=i_label_index_enable;
		return;
	}
	int NyARLabelingImage::getTopClipTangentX(const NyARLabelingLabel& i_label)const
	{
		int pix;
		int i_label_id=i_label.id;
		const int* index_table=this->_index_table->item;
		const int* limage=this->_ref_buf;
		int limage_ptr=i_label.clip_t*this->_size.w;
		int clip1 = i_label.clip_r;
		// p1=ShortPointer.wrap(limage,j*xsize+clip.get());//p1 =&(limage[j*xsize+clip[0]]);
		for (int i = i_label.clip_l; i <= clip1; i++) {// for( i = clip[0]; i <=clip[1]; i++, p1++ ) {
			pix = limage[limage_ptr+i];
			if (pix > 0 && index_table[pix-1] == i_label_id){
				return i;
			}
		}
		//あれ？見つからないよ？
		throw NyARException();
	}

	int NyARLabelingImage::getContour(int i_index,int i_array_size,int o_coord_x[],int o_coord_y[])const
	{
		const int width=this->_size.w;
		const int* xdir = this->_getContour_xdir;// static int xdir[8] = { 0,1, 1, 1, 0,-1,-1,-1};
		const int* ydir = this->_getContour_ydir;// static int ydir[8] = {-1,-1,0, 1, 1, 1, 0,-1};
		const NyARLabelingLabel* label=this->_label_list->getItem(i_index);		
		int i;
		//クリップ領域の上端に接しているポイントを得る。
		int sx=getTopClipTangentX(*label);
		int sy=label->clip_t;

		int coord_num = 1;
		o_coord_x[0] = sx;
		o_coord_y[0] = sy;
		int dir = 5;

		const int* limage=this->_ref_buf;
		int c = o_coord_x[0];
		int r = o_coord_y[0];
		for (;;) {
			dir = (dir + 5) % 8;
			for (i = 0; i < 8; i++) {
				if (limage[(r + ydir[dir])*width+(c + xdir[dir])] > 0) {
					break;
				}
				dir = (dir + 1) % 8;
			}
			if (i == 8) {
				//8方向全て調べたけどラベルが無いよ？
				throw NyARException();// return(-1);
			}
			// xcoordとycoordをc,rにも保存
			c = c + xdir[dir];
			r = r + ydir[dir];
			o_coord_x[coord_num] = c;
			o_coord_y[coord_num] = r;
			// 終了条件判定
			if (c == sx && r == sy){
				coord_num++;
				break;
			}
			coord_num++;
			if (coord_num == i_array_size) {
				//輪郭が末端に達した
				return coord_num;
			}
		}
		return coord_num;		

	}
}
