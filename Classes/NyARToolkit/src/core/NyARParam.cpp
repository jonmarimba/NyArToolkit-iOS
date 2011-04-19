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
#include "NyARParam.h"
#include "../../inc/utils/NyStdLib.h"
#include "nyarcore.h"
#include <exception>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
namespace NyARToolkitCPP
{

	NyARParam::NyARParam()
	{
		this->_endian=TNyAREndian_LITTLE;
		return;
	}


	const TNyARIntSize* NyARParam::getScreenSize()const
	{
		return &this->_screen_size;
	}

	const NyARPerspectiveProjectionMatrix* NyARParam::getPerspectiveProjectionMatrix()const
	{
		return &this->_projection_matrix;
	}
	const NyARCameraDistortionFactor* NyARParam::getDistortionFactor()const
	{
		return &this->_dist;
	}

	/**
	* ARToolKit標準ファイルから1個目の設定をロードする。
	* 
	* @param i_filename
	* @throws NyARException
	*/
	void NyARParam::loadARParamFromFile(const char* i_filename)
	{
		ifstream inf;
		NyARParamFileStruct_t tmp;
		try {
			inf.open(i_filename);
			inf.read((char*)&tmp,sizeof(NyARParamFileStruct_t));
			if(inf.gcount()!=sizeof(NyARParamFileStruct_t))
			{
				throw NyARException();
			}
			/*エンディアン保障*/
			if(this->_endian==NyARToolkitCPP::TNyAREndian_LITTLE){
				tmp.x=NyStdLib::byteSwap(tmp.x);
				tmp.y=NyStdLib::byteSwap(tmp.y);
				for(int i=0;i<12;i++){
					tmp.projection[i]=NyStdLib::byteSwap(tmp.projection[i]);
				}
				for(int i=0;i<4;i++){
					tmp.distortion[i]=NyStdLib::byteSwap(tmp.distortion[i]);
				}
			}
			loadARParam(tmp);
			inf.close();
		} catch(NyARException e){
			throw;
		} catch (exception e) {
			throw NyARException(e);
		}
		return;
	}

	/**
	* int arParamChangeSize( ARParam *source, int xsize, int ysize, ARParam *newparam );
	* 関数の代替関数 サイズプロパティをi_xsize,i_ysizeに変更します。
	* @param i_xsize
	* @param i_ysize
	* @param newparam
	* @return
	* 
	*/
	void NyARParam::changeScreenSize(int i_xsize, int i_ysize)
	{
		const double scale = (double) i_xsize / (double) (this->_screen_size.w);// scale = (double)xsize / (double)(source->xsize);
		//スケールを変更
		this->_dist.changeScale(scale);
		this->_projection_matrix.changeScale(scale);

		this->_screen_size.w = i_xsize;// newparam->xsize = xsize;
		this->_screen_size.h = i_ysize;// newparam->ysize = ysize;
		return;
	}


	/**
	* int arParamLoad( const char *filename, int num, ARParam *param, ...);
	* i_streamの入力ストリームからi_num個の設定を読み込み、パラメタを配列にして返します。
	* 
	* @param i_stream
	* @throws Exception
	*/
	void NyARParam::loadARParam(const NyARParamFileStruct_t& i_stream)
	{
		/*必要に応じてbyteswapしながら値をコピー*/
		this->_screen_size.w = i_stream.x;
		this->_screen_size.h = i_stream.y;
		//Projectionオブジェクトにセット
		this->_projection_matrix.setValue(i_stream.projection);
		//Factorオブジェクトにセット
		this->_dist.setValue(i_stream.distortion);
		return;
	}

	void NyARParam::saveARParam()
	{
		NyARException::trap("未チェックのパス");
		/*
		byte[] buf = new byte[SIZE_OF_PARAM_SET];
		// バッファをラップ
		ByteBuffer bb = ByteBuffer.wrap(buf);
		bb.order(ByteOrder.BIG_ENDIAN);

		// 書き込み
		bb.putInt(this._screen_size.w);
		bb.putInt(this._screen_size.h);
		double[] tmp=new double[12];
		//Projectionを読み出し
		this._projection_matrix.getValue(tmp);
		//double値を12個書き込む
		for(int i=0;i<12;i++){
		tmp[i]=bb.getDouble();
		}
		//Factorを読み出し
		this._dist.getValue(tmp);
		//double値を4個書き込む
		for (int i = 0; i < 4; i++) {
		tmp[i]=bb.getDouble();
		}
		i_stream.write(buf);
		return;
		*/
	};

	void NyARParam::setEndian(TNyAREndian i_new_endian)
	{
		this->_endian=i_new_endian;
		return;
	}

}
