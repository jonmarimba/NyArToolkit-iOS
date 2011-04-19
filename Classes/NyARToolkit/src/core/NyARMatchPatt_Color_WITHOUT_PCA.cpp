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
#include "NyARMatchPatt_Color_WITHOUT_PCA.h"
#include "INyARColorPatt.h"
#include "INyARMatchPatt.h"
#include "../../inc/utils/NyArray.h"
#include "nyarcore.h"
#include <cmath>
namespace NyARToolkitCPP
{

	NyARMatchPatt_Color_WITHOUT_PCA::NyARMatchPatt_Color_WITHOUT_PCA()
	{
		this->_width=1;
		this->_height=1;
		this->_cf=0.0;
		this->_dir=0;
		return;

	}
	 double NyARMatchPatt_Color_WITHOUT_PCA::getConfidence()const
	{
		return this->_cf;
	}

	 int NyARMatchPatt_Color_WITHOUT_PCA::getDirection()const
	{
		return this->_dir;
	}

	 void NyARMatchPatt_Color_WITHOUT_PCA::reallocInputArray(int i_width, int i_height)
	{
		//値空間による制約
		NyAR_ASSERT(i_width<1024 && i_height<1024);
		if (this->_input.length<i_height*i_width*3) {
			// 配列が十分なサイズでなければ取り直す
			this->_input.renew(i_height*i_width*3);
		}
		this->_height = i_height;
		this->_width = i_width;
		return;
	}

	bool NyARMatchPatt_Color_WITHOUT_PCA::setPatt(const INyARColorPatt& i_target_patt)
	{
		int i;
		// input配列のサイズとwhも更新// input=new int[height][width][3];
		reallocInputArray(i_target_patt.getWidth(), i_target_patt.getHeight());
		const int pixels=this->_width*this->_height;

		const int *data = i_target_patt.getPatArray();

		int sum = 0, l_ave = 0;
		const int *data_ptr;
		data_ptr=data;
		for (i = pixels - 1; i >= 0; i--){
			l_ave +=-*(data_ptr++)-*(data_ptr++)-*(data_ptr++);
		}
		l_ave=255*3*pixels+l_ave;
		l_ave /= (pixels * 3);
		data_ptr=data;
		int *input_ptr = this->_input.item;
		for (i = pixels - 1; i >= 0; i--) {// for(i=0;i<height;i++){//for(int i=0;i<Config.AR_PATT_SIZE_Y;i++){
				int w_sum = (255 - *(data_ptr++)) - l_ave;
				*(input_ptr++) = w_sum;
				sum += w_sum * w_sum;

				w_sum = (255 - *(data_ptr++)) - l_ave;
				*(input_ptr++) = w_sum;
				sum += w_sum * w_sum;

				w_sum = (255 - *(data_ptr++)) - l_ave;
				*(input_ptr++) = w_sum;
				sum += w_sum * w_sum;
				// </Optimize>
		}
		this->_datapow = sqrt((double) sum);
		if (this->_datapow == 0.0) {
			return false;// throw NyARException();
		}
		return true;
	}

	void NyARMatchPatt_Color_WITHOUT_PCA::evaluate(const NyARCode& i_code)
	{
		const double *patpow = i_code.getPatPow();
		int res = -1;
		double max = 0.0;
		const int* pat_ptr=i_code.getPat();
		for (int j = 0; j < 4; j++) {
			int sum = 0;
			const int *input_ptr = this->_input.item;
			for (int i = this->_width*this->_height - 1; i >= 0; i--) {// for(int i=0;i<Config.AR_PATT_SIZE_Y;i++){
				sum += *(input_ptr++) * *(pat_ptr++);// sum +=input[i][i2][i3]*pat[k][j][i][i2][i3];
				sum += *(input_ptr++) * *(pat_ptr++);// sum +=input[i][i2][i3]*pat[k][j][i][i2][i3];
				sum += *(input_ptr++) * *(pat_ptr++);// sum +=input[i][i2][i3]*pat[k][j][i][i2][i3];
			}
			double sum2 = sum / patpow[j] / this->_datapow;// sum2 = sum / patpow[k][j]/ datapow;
			if (sum2 > max) {
				max = sum2;
				res = j;
			}
		}
		this->_dir = res;
		this->_cf = max;
	}

}
