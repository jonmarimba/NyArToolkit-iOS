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
#include "NyARCode.h"
#include "nyarcore.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
namespace NyARToolkitCPP
{
	/*	メモリレイアウト[4][height][width][3]
	*/
	const int* NyARCode::getPat()const
	{
		return pat;
	}
	const double* NyARCode::getPatPow()const
	{
		return patpow;
	}
	const short* NyARCode::getPatBW()const
	{
		return patBW;
	}

	const double* NyARCode::getPatPowBW()const
	{
		return patpowBW;
	}

	int NyARCode::getWidth()const
	{
		return width;
	}

	int NyARCode::getHeight()const
	{
		return height;
	}

	NyARCode::NyARCode(int i_width, int i_height)
	{
		width = i_width;
		height = i_height;
		pat = new int[4*height*width*3];// static int pat[AR_PATT_NUM_MAX][4][AR_PATT_SIZE_Y*AR_PATT_SIZE_X*3];
		patBW = new short[4*height*width];// static int patBW[AR_PATT_NUM_MAX][4][AR_PATT_SIZE_Y*AR_PATT_SIZE_X*3];
		return;
	}
	NyARCode::~NyARCode()
	{
		NyAR_SAFE_DELETE(pat);
		NyAR_SAFE_DELETE(patBW);
		return;
	}
	void NyARCode::loadARPattFromFile(const char* i_filename)
	{
		ifstream inf;

		int* data_array=NULL;
		try {
			inf.open(i_filename);
			data_array=new int[4*height*width*3];
			for(int i=0;i<4*height*width*3;i++){
				int r;
				inf>>r;
				data_array[i]=r;
			}
			loadARPatt(data_array,width,height);
			NyAR_SAFE_DELETE(data_array);
			data_array=NULL;
		} catch(NyARException e){
			NyAR_SAFE_DELETE(data_array);
			throw;
		} catch (exception e) {
			NyAR_SAFE_DELETE(data_array);
			throw NyARException(e);
		}
		return;
	}
	void NyARCode::loadARPatt(const int* i_data_array,int i_width,int i_height)
	{
		if(i_width!=width || i_height!=height)
		{
			throw NyARException();
		}
		try {
			int pt=0;
			//パターンデータはGBRAで並んでる。
			//データの並び順を変えるために力づくで変換
			for (int h = 0; h < 4; h++) {
				int l = 0;
				for (int i3 = 0; i3 < 3; i3++) {
					for (int i2 = 0; i2 < height; i2++) {
						for (int i1 = 0; i1 < width; i1++) {								
							short j = (short) (255 - i_data_array[pt]);// j = 255-j;
							int* pat_ptr=pat+(h*height*width*3+i2*width*3+i1*3);
							// 標準ファイルのパターンはBGRでならんでるからRGBに並べなおす
							switch (i3) {
								case 0:
									*(pat_ptr+2) = j;
									break;// pat[patno][h][(i2*Config.AR_PATT_SIZE_X+i1)*3+2]= j;break;
								case 1:
									*(pat_ptr+1) = j;
									break;// pat[patno][h][(i2*Config.AR_PATT_SIZE_X+i1)*3+1]= j;break;
								case 2:
									*(pat_ptr+0) = j;
									break;// pat[patno][h][(i2*Config.AR_PATT_SIZE_X+i1)*3+0]= j;break;
							}
							// pat[patno][h][(i2*Config.AR_PATT_SIZE_X+i1)*3+i3]= j;
							short* pat_BWptr=patBW+(h*height*width+i2*width+i1);
							if (i3 == 0) {
								*pat_BWptr = j;// patBW[patno][h][i2*Config.AR_PATT_SIZE_X+i1] = j;
							} else {
								*pat_BWptr += j;// patBW[patno][h][i2*Config.AR_PATT_SIZE_X+i1] += j;
							}
							if (i3 == 2) {
								*pat_BWptr /= 3;// patBW[patno][h][i2*Config.AR_PATT_SIZE_X+i1]/= 3;
							}
							l += j;
							pt++;
						}
					}
				}

				l /= (height * width * 3);

				int m = 0;
				for (int i = 0; i < height; i++) {// for( i = 0; i < AR_PATT_SIZE_Y*AR_PATT_SIZE_X*3;i++ ) {
					for (int i2 = 0; i2 < width; i2++) {
						for (int i3 = 0; i3 < 3; i3++) {
							int* pat_ptr=pat+(h*height*width*3+i*width*3+i2*3+i3);
							*pat_ptr -= l;
							m += (*pat_ptr * *pat_ptr);
						}
					}
				}
				patpow[h] = sqrt((double) m);
				if (patpow[h] == 0.0) {
					patpow[h] = 0.0000001;
				}

				m = 0;
				for (int i = 0; i < height; i++) {
					for (int i2 = 0; i2 < width; i2++) {
						short* patBW_ptr=patBW+(h*height*width+i*width+i2);
						*patBW_ptr -= (short)l;
						m += *patBW_ptr * *patBW_ptr;
					}
				}
				patpowBW[h] = sqrt((double) m);
				if (patpowBW[h] == 0.0) {
					patpowBW[h] = 0.0000001;
				}
			}
		} catch (exception e) {
			throw NyARException(e);
		}
	}
}
