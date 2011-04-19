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
#include "NyARRasterFilter_ARToolkitThreshold.h"
#include "INyARRgbRaster.h"
#include "NyARBinRaster.h"
#include "nyarcore.h"
namespace NyARToolkitCPP
{
	NyARRasterFilter_ARToolkitThreshold:: NyARRasterFilter_ARToolkitThreshold(int i_threshold)
	{
		this->_threshold = i_threshold;
	}
	void NyARRasterFilter_ARToolkitThreshold::setThreshold(int i_threshold)
	{
		this->_threshold = i_threshold;
	}

	void NyARRasterFilter_ARToolkitThreshold::doFilter(const INyARRgbRaster& i_input, NyARBinRaster& i_output)
	{
		const INyARBufferReader *in_buffer_reader=i_input.getBufferReader();	
		const INyARBufferReader *out_buffer_reader=i_output.getBufferReader();
		int in_buf_type=in_buffer_reader->getBufferType();

		NyAR_ASSERT(out_buffer_reader->isEqualBufferType(INyARBufferReader::BUFFERFORMAT_INT1D_BIN_8));
		NyAR_ASSERT(checkInputType(in_buf_type)==true);	
		NyAR_ASSERT(TNyARIntSize::isEqualSize(i_input.getSize(),i_output.getSize()));

		int* out_buf = (int*) out_buffer_reader->getBuffer();
		const NyAR_BYTE_t* in_buf = (const NyAR_BYTE_t*) in_buffer_reader->getBuffer();

		const TNyARIntSize* size = i_output.getSize();
		switch (in_buffer_reader->getBufferType()) {
		case INyARBufferReader::BUFFERFORMAT_BYTE1D_B8G8R8_24:
		case INyARBufferReader::BUFFERFORMAT_BYTE1D_R8G8B8_24:
			convert24BitRgb(in_buf, out_buf, size);
			break;
		case INyARBufferReader::BUFFERFORMAT_BYTE1D_B8G8R8X8_32:
			convert32BitRgbx(in_buf, out_buf, size);
			break;
		default:
			throw NyARException();
		}
		return;
	}

	void NyARRasterFilter_ARToolkitThreshold::convert24BitRgb(const NyAR_BYTE_t* i_in, int* i_out, const TNyARIntSize* i_size)const
	{
		int th=this->_threshold*3;
		int bp =(i_size->w*i_size->h-1)*3;
		int w;
		int xy;
		int pix_count   =i_size->h*i_size->w;
		int pix_mod_part=pix_count-(pix_count%8);
		for(xy=pix_count-1;xy>=pix_mod_part;xy--){
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
		}
		//タイリング
		for (;xy>=0;) {
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 3;
			xy--;
		}
		return;
	}
	void NyARRasterFilter_ARToolkitThreshold::convert32BitRgbx(const NyAR_BYTE_t* i_in, int* i_out, const TNyARIntSize* i_size)const
	{
		int th=this->_threshold*3;
		int bp =(i_size->w*i_size->h-1)*4;
		int w;
		int xy;
		int pix_count   =i_size->h*i_size->w;
		int pix_mod_part=pix_count-(pix_count%8);
		for(xy=pix_count-1;xy>=pix_mod_part;xy--){
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
		}
		//タイリング
		for (;xy>=0;) {
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
			w= ((i_in[bp] & 0xff) + (i_in[bp + 1] & 0xff) + (i_in[bp + 2] & 0xff));
			i_out[xy]=w<=th?0:1;
			bp -= 4;
			xy--;
		}
		return;
	}

	 bool NyARRasterFilter_ARToolkitThreshold::checkInputType(int i_input_type)const
	{
		switch(i_input_type){
		case INyARBufferReader::BUFFERFORMAT_BYTE1D_B8G8R8_24:
		case INyARBufferReader::BUFFERFORMAT_BYTE1D_R8G8B8_24:
		case INyARBufferReader::BUFFERFORMAT_BYTE1D_B8G8R8X8_32:
		case INyARBufferReader::BUFFERFORMAT_BYTE1D_R5G6B5_16LE:
			return true;
		default:
			return false;
		}
	}

}

#define COMMENT
#ifndef COMMENT
#include <cstdio>
#include <cstdlib>
#include <Winbase.h>
#include "NyARRgbRaster_BGRA.h"
#include "NyARGrayscaleRaster.h"
using namespace NyARToolkitCPP;
void test()
{
	FILE *fp;
	static unsigned char s[320*240*4];
	
	if ((fp = fopen("\\temp\\320x240ABGR.raw", "r")) == NULL) {
		printf("file open error!!\n");
		exit(-1);
	}
	fread(s,1,320*240*4,fp);
	fclose(fp);
	NyARRgbRaster_BGRA* in_raster=NyARRgbRaster_BGRA::wrap(s,320,240);
	NyARBinRaster gs(320,240);
	NyARRasterFilter_ARToolkitThreshold th(100);
	DWORD t=GetTickCount();
	for(int i=0;i<100;i++){
		th.doFilter(in_raster,&gs);
	}
	t=GetTickCount()-t;
	TCHAR w[256];
	wsprintf(w,L"%ums",t);
	OutputDebugString(w);
	const int* p;
	p=(const int*)gs.getBufferReader()->getBuffer();
	int sum=0;
	for(int i=0;i<320*240;i++){
		sum+=*(p+i);
	}
	delete in_raster;
	return;
}

#endif

