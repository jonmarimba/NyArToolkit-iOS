#include "NyStdLib.h"

namespace NyARToolkitCPP
{
	int NyStdLib::byteSwap(int i_src)
	{
		int result;
		const unsigned char* r=(const unsigned char*)&i_src;
		unsigned char* w=(unsigned char*)&result;
		for(int i = 0; i < 4; i++ ) {
			*(w+i)=*(r+3-i);
		}
		return result;
	}

	double NyStdLib::byteSwap(double i_src)
	{
		double result;
		const unsigned char* r=(const unsigned char*)&i_src;
		unsigned char* w=(unsigned char*)&result;
		for(int i = 0; i < 8; i++ ) {
			*(w+i)=*(r+7-i);
		}
		return result;
	}
}
