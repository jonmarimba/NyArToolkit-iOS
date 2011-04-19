#pragma once
#include "NyARRaster_BasicClass.h"
#include "INyARBufferReader.h"
#include "NyARBufferReader.h"
namespace NyARToolkitCPP
{
	class NyARGrayscaleRaster :public NyARRaster_BasicClass
	{
	protected:
		int* _ref_buf;
	private:
		INyARBufferReader* _buffer_reader;
	public:
		NyARGrayscaleRaster(int i_width, int i_height);
		virtual ~NyARGrayscaleRaster();
	public:
		 const INyARBufferReader* getBufferReader() const;
	};
}
