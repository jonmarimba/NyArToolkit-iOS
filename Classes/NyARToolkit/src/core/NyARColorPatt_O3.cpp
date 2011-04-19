#include "NyARColorPatt_O3.h"
#include "nyarcore.h"
#include <cstring>
#include <cstdio>
#define AR_PATT_SAMPLE_NUM 64
namespace NyARToolkitCPP
{
	struct T__updateExtpat_struct_t{
		double* para00_xw;
		double* para10_xw;
		double* para20_xw;
		int* rgb_buf;
		int* x_rgb_index;
		int* y_rgb_index;
		int* i_rgb_index;
		//
		int* buf_int;
		double* buf_double;
		int length;
		T__updateExtpat_struct_t()
		{
			this->buf_double=NULL;
			this->buf_int=NULL;
			this->length=0;
		}
		~T__updateExtpat_struct_t()
		{
			NyAR_SAFE_DELETE(this->buf_double);
			NyAR_SAFE_DELETE(this->buf_int);
		}
		void reservWorkBuffers(int i_xdiv2)
		{
			int* new_int;
			double* new_double;
			if (this->length < i_xdiv2) {
				new_double=new double[i_xdiv2*3];
				new_int=new int[i_xdiv2*(3+1+1+1)];
				NyAR_SAFE_DELETE(this->buf_double);
				NyAR_SAFE_DELETE(this->buf_int);
				this->buf_double=new_double;
				this->buf_int=new_int;
			}
			this->para00_xw=this->buf_double;
			this->para10_xw=this->para00_xw+i_xdiv2;
			this->para20_xw=this->para10_xw+i_xdiv2;

			this->x_rgb_index=this->buf_int;
			this->y_rgb_index=this->x_rgb_index+i_xdiv2;
			this->i_rgb_index=this->y_rgb_index+i_xdiv2;
			this->rgb_buf=this->i_rgb_index+i_xdiv2;

			this->length=i_xdiv2;
			return;
		}
	};


	/*	extpatのメモリマップは、[W][H][3]

	*/
	NyARColorPatt_O3::NyARColorPatt_O3(int i_width, int i_height)
	{
		this->_width = i_width;
		this->_height = i_height;
		this->_extpat = new int[i_height*i_width*3];
		////////////////////
		this->__get_cpara_a=new NyARMat(8, 8);
		this->__get_cpara_b=new NyARMat(8, 1);
		this->__pickFromRaster_cpara = new NyARMat(8, 1);
		this->__updateExtpat=new T__updateExtpat_struct_t();
	}
	NyARColorPatt_O3::~NyARColorPatt_O3()
	{
		NyAR_SAFE_DELETE(this->_extpat);
		////////////////////
		NyAR_SAFE_DELETE(this->__get_cpara_a);
		NyAR_SAFE_DELETE(this->__get_cpara_b);
		NyAR_SAFE_DELETE(this->__pickFromRaster_cpara);
		NyAR_SAFE_DELETE(this->__updateExtpat);
		return;
	}
	const int* NyARColorPatt_O3::getPatArray()const
	{
		return this->_extpat;
	}
	int NyARColorPatt_O3::getWidth()const
	{
		return this->_width;
	}
	int NyARColorPatt_O3::getHeight()const
	{
		return this->_height;
	}
	bool NyARColorPatt_O3::get_cpara(const TNyARIntPoint i_vertex[], NyARMat& o_para)
	{
		const static int world[4][2]={// double world[4][2];
			{ 100, 100 }, { 100 + 10, 100 }, { 100 + 10, 100 + 10 }, { 100, 100 + 10 }};
			NyARMat *a =this->__get_cpara_a;// 次処理で値を設定するので、初期化不要// new NyARMat( 8, 8 );
			double* a_array = a->getArray();
			NyARMat *b =this->__get_cpara_b;// 次処理で値を設定するので、初期化不要// new NyARMat( 8, 1 );
			double *b_array = b->getArray();
			double *a_pt0, *a_pt1;
			const int* world_pti;

			for (int i = 0; i < 4; i++) {
				a_pt0 = a_array+((i * 2)*8);
				a_pt1 = a_array+((i * 2 + 1)*8);
				world_pti = world[i];

				a_pt0[0] = (double) world_pti[0];// a->m[i*16+0] = world[i][0];
				a_pt0[1] = (double) world_pti[1];// a->m[i*16+1] = world[i][1];
				a_pt0[2] = 1.0;// a->m[i*16+2] = 1.0;
				a_pt0[3] = 0.0;// a->m[i*16+3] = 0.0;
				a_pt0[4] = 0.0;// a->m[i*16+4] = 0.0;
				a_pt0[5] = 0.0;// a->m[i*16+5] = 0.0;
				a_pt0[6] = (double) (-world_pti[0] * i_vertex[i].x);// a->m[i*16+6]= -world[i][0]*vertex[i][0];
				a_pt0[7] = (double) (-world_pti[1] * i_vertex[i].x);// a->m[i*16+7]=-world[i][1]*vertex[i][0];
				a_pt1[0] = 0.0;// a->m[i*16+8] = 0.0;
				a_pt1[1] = 0.0;// a->m[i*16+9] = 0.0;
				a_pt1[2] = 0.0;// a->m[i*16+10] = 0.0;
				a_pt1[3] = (double) world_pti[0];// a->m[i*16+11] = world[i][0];
				a_pt1[4] = (double) world_pti[1];// a->m[i*16+12] = world[i][1];
				a_pt1[5] = 1.0;// a->m[i*16+13] = 1.0;
				a_pt1[6] = (double) (-world_pti[0] * i_vertex[i].y);// a->m[i*16+14]=-world[i][0]*vertex[i][1];
				a_pt1[7] = (double) (-world_pti[1] * i_vertex[i].y);// a->m[i*16+15]=-world[i][1]*vertex[i][1];
				b_array[(i * 2 + 0)*1+0] = (double) i_vertex[i].x;// b->m[i*2+0] =vertex[i][0];
				b_array[(i * 2 + 1)*1+0] = (double) i_vertex[i].y;// b->m[i*2+1] =vertex[i][1];
			}
			if (!a->matrixSelfInv()) {
				return false;
			}

			o_para.matrixMul(*a,*b);
			return true;
	}
	void NyARColorPatt_O3::initValue_wk_pickFromRaster_ext_pat2(int* i_ext_pat2, int i_width, int i_height)
	{
		//ゼロクリア
		/*
		int* pt2;
		int* pt1;
		for (int i = i_height - 1; i >= 0; i--) {
		pt2 = i_ext_pat2+(i*i_width*3);
		for (int i2 = i_width - 1; i2 >= 0; i2--) {
		pt1 = pt2+(i2*3);
		pt1[0] = 0;
		pt1[1] = 0;
		pt1[2] = 0;
		}
		}
		*/
		memset(i_ext_pat2,0,i_height*i_width*3*sizeof(int));
		return;
	}
	bool NyARColorPatt_O3::pickFromRaster(const INyARRgbRaster& image, const NyARSquare& i_square) 
	{
		NyARMat& cpara = *this->__pickFromRaster_cpara;
		const TNyARIntPoint* local = i_square.imvertex;
		// //localの計算
		// int[] local_0=wk_pickFromRaster_local[0];//double local[4][2];
		// int[] local_1=wk_pickFromRaster_local[1];//double local[4][2];
		// //
		// for(int i = 0; i < 4; i++ ) {
		// local_0[i] = i_square.imvertex[i][0];
		// local_1[i] = i_square.imvertex[i][1];
		// }
		// xdiv2,ydiv2の計算
		int xdiv2, ydiv2;
		int l1, l2;
		int w1, w2;

		// x計算
		w1 = local[0].x - local[1].x;
		w2 = local[0].y - local[1].y;
		l1 = (w1 * w1 + w2 * w2);
		w1 = local[2].x - local[3].x;
		w2 = local[2].y - local[3].y;
		l2 = (w1 * w1 + w2 * w2);
		if (l2 > l1) {
			l1 = l2;
		}
		l1 = l1 / 4;
		xdiv2 = this->_width;
		while (xdiv2 * xdiv2 < l1) {
			xdiv2 *= 2;
		}
		if (xdiv2 > AR_PATT_SAMPLE_NUM) {
			xdiv2 = AR_PATT_SAMPLE_NUM;
		}

		// y計算
		w1 = local[1].x - local[2].x;
		w2 = local[1].y - local[2].y;
		l1 = (w1 * w1 + w2 * w2);
		w1 = local[3].x - local[0].x;
		w2 = local[3].y - local[0].y;
		l2 = (w1 * w1 + w2 * w2);
		if (l2 > l1) {
			l1 = l2;
		}
		ydiv2 = this->_height;
		l1 = l1 / 4;
		while (ydiv2 * ydiv2 < l1) {
			ydiv2 *= 2;
		}
		if (ydiv2 > AR_PATT_SAMPLE_NUM) {
			ydiv2 = AR_PATT_SAMPLE_NUM;
		}

		// cparaの計算
		if (!get_cpara(local, cpara)) {
			return false;
		}
		updateExtpat(image, cpara, xdiv2, ydiv2);
		return true;
	}
	void NyARColorPatt_O3::updateExtpat(const INyARRgbRaster& image, const NyARMat& i_cpara, int i_xdiv2,int i_ydiv2)
	{
		const int img_x = image.getWidth();
		const int img_y = image.getHeight();
		int* L_extpat = this->_extpat;
		const int L_WIDTH = this->_width;
		const int L_HEIGHT = this->_height;
		/* wk_pickFromRaster_ext_pat2ワーク変数を初期化する。 */
		// int[][][] ext_pat2=wk_pickFromRaster_ext_pat2;//ARUint32
		// ext_pat2[AR_PATT_SIZE_Y][AR_PATT_SIZE_X][3];
		// int ext_pat2_j[][],ext_pat2_j_i[];

		initValue_wk_pickFromRaster_ext_pat2(L_extpat, L_WIDTH, L_HEIGHT);

		const double* cpara_array = i_cpara.getArray();
		double para21_x_yw, para01_x_yw, para11_x_yw;
		double para00, para01, para02, para10, para11, para12, para20, para21;
		para00 = cpara_array[(0 * 3 + 0)*1+0];// para[i][0] = c->m[i*3+0];
		para01 = cpara_array[(0 * 3 + 1)*1+0];// para[i][1] = c->m[i*3+1];
		para02 = cpara_array[(0 * 3 + 2)*1+0];// para[i][2] = c->m[i*3+2];
		para10 = cpara_array[(1 * 3 + 0)*1+0];// para[i][0] = c->m[i*3+0];
		para11 = cpara_array[(1 * 3 + 1)*1+0];// para[i][1] = c->m[i*3+1];
		para12 = cpara_array[(1 * 3 + 2)*1+0];// para[i][2] = c->m[i*3+2];
		para20 = cpara_array[(2 * 3 + 0)*1+0];// para[2][0] = c->m[2*3+0];
		para21 = cpara_array[(2 * 3 + 1)*1+0];// para[2][1] = c->m[2*3+1];

		double d, yw;
		int xc, yc;
		int i, j;
		// arGetCode_put_zero(ext_pat2);//put_zero( (ARUint8 *)ext_pat2,
		// AR_PATT_SIZE_Y*AR_PATT_SIZE_X*3*sizeof(ARUint32) );
		int xdiv = i_xdiv2 / L_WIDTH;// xdiv = xdiv2/Config.AR_PATT_SIZE_X;
		int ydiv = i_ydiv2 / L_HEIGHT;// ydiv = ydiv2/Config.AR_PATT_SIZE_Y;

		// 計算バッファを予約する
		double *para00_xw,*para10_xw,*para20_xw;
		int *x_rgb_index,*y_rgb_index,*i_rgb_index,*rgb_buf;
		{
			T__updateExtpat_struct_t& up_ptr=*this->__updateExtpat;
			up_ptr.reservWorkBuffers(i_xdiv2);
			para00_xw = up_ptr.para00_xw;
			para10_xw = up_ptr.para10_xw;
			para20_xw = up_ptr.para20_xw;
			x_rgb_index = up_ptr.x_rgb_index;
			y_rgb_index = up_ptr.y_rgb_index;
			i_rgb_index = up_ptr.i_rgb_index;
			rgb_buf = up_ptr.rgb_buf;
			for (i = 0; i < i_xdiv2; i++) {
				const double xw = 102.5 + 5.0 * ((double) i + 0.5) / i_xdiv2;
				para20_xw[i] = para20 * xw;
				para00_xw[i] = para00 * xw;
				para10_xw[i] = para10 * xw;
			}
		}

		int *extpat_ptr;
		int index_num;
		int *extpat_j;
		//ピクセルリーダーを取得
		const INyARRgbPixelReader* reader=image.getRgbPixelReader();

		for (j = 0; j < i_ydiv2; j++) {
			yw = 102.5 + 5.0 * ((double) j + 0.5) / i_ydiv2;
			para21_x_yw = para21 * yw + 1.0;
			para11_x_yw = para11 * yw + para12;
			para01_x_yw = para01 * yw + para02;
			extpat_j = L_extpat+((j / ydiv)*this->_width*3);
			index_num = 0;
			// ステップ１．RGB取得用のマップを作成
			for (i = 0; i < i_xdiv2; i++) {
				d = para20_xw[i] + para21_x_yw;
				if (d == 0) {
					throw NyARException();
				}
				xc = (int) ((para00_xw[i] + para01_x_yw) / d);
				yc = (int) ((para10_xw[i] + para11_x_yw) / d);
				// 範囲外は無視
				if (xc < 0 || xc >= img_x || yc < 0 || yc >= img_y) {
					continue;
				}
				// ピクセル値の計算
				// image.getPixel(xc,yc,rgb_buf);
				// ext_pat2_j_i=ext_pat2_j[i/xdiv];
				// ext_pat2_j_i[0] += rgb_buf[0];//R
				// ext_pat2_j_i[1] += rgb_buf[1];//G
				// ext_pat2_j_i[2] += rgb_buf[2];//B

				x_rgb_index[index_num] = xc;
				y_rgb_index[index_num] = yc;
				i_rgb_index[index_num] = i / xdiv;
				index_num++;
			}
			// //ステップ２．ピクセル配列を取得
			reader->getPixelSet(x_rgb_index, y_rgb_index, index_num, rgb_buf);

			//ピクセル値の計算
			for (i = index_num - 1; i >= 0; i--) {
				extpat_ptr = extpat_j+(i_rgb_index[i]*3);
				extpat_ptr[0] += rgb_buf[i * 3 + 0];// R
				extpat_ptr[1] += rgb_buf[i * 3 + 1];// G
				extpat_ptr[2] += rgb_buf[i * 3 + 2];// B
			}
		}
		const int xdiv_x_ydiv = xdiv * ydiv;
		extpat_ptr = L_extpat;//+(j*this->_width*3);
		for (j = L_HEIGHT*L_WIDTH-1; j >= 0; j--) {
			*(extpat_ptr++) /= (xdiv_x_ydiv);
			*(extpat_ptr++) /= (xdiv_x_ydiv);
			*(extpat_ptr++) /= (xdiv_x_ydiv);
		}
		return;
	}
}
