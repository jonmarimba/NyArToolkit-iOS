#pragma once
#include "INyARRgbRaster.h"
#include "NyARSquare.h"

namespace NyARToolkitCPP
{
	class INyARColorPatt
	{
	public:
		virtual ~INyARColorPatt(){};
	public:
		virtual int getWidth()const=0;
		virtual int getHeight()const=0;
		/**
		* カメラパターンを格納した配列への参照値を返します。 配列は最低でも[height][wight][3]のサイズを持ちますが、
		* 配列のlengthとwidth,heightの数は一致しないことがあります。
		* 
		* @return
		*/
		virtual const int* getPatArray()const=0;
		virtual bool pickFromRaster(const INyARRgbRaster& image, const NyARSquare& i_square)=0;
	};
}
