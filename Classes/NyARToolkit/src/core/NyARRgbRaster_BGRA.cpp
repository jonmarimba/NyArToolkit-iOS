#include "nyarcore.h"
#include "NyARRgbRaster_BGRA.h"
#include "NyARBufferReader.h"
#include <cstddef>
namespace NyARToolkitCPP
{


	NyARRgbRaster_BGRA* NyARRgbRaster_BGRA::wrap(const NyAR_BYTE_t* i_buffer, int i_width, int i_height)
	{
		return new NyARRgbRaster_BGRA(i_buffer, i_width, i_height);
	}

	NyARRgbRaster_BGRA::NyARRgbRaster_BGRA(const NyAR_BYTE_t* i_buffer, int i_width, int i_height):NyARRgbRaster_BasicClass(i_width,i_height)
	{
		this->_ref_buf = i_buffer;
		this->_rgb_reader = new PixelReader(this);
		this->_buffer_reader=new NyARBufferReader(i_buffer,INyARBufferReader::BUFFERFORMAT_BYTE1D_B8G8R8X8_32);
		return;
	}
	NyARRgbRaster_BGRA::NyARRgbRaster_BGRA(int i_width, int i_height):NyARRgbRaster_BasicClass(i_width,i_height)
	{
		this->_ref_buf =NULL;
		this->_rgb_reader = new PixelReader(this);
		this->_buffer_reader=new NyARBufferReader(NULL,INyARBufferReader::BUFFERFORMAT_BYTE1D_B8G8R8X8_32);
		return;
	}

	NyARRgbRaster_BGRA::~NyARRgbRaster_BGRA(void)
	{
		NyAR_SAFE_DELETE(this->_rgb_reader);
		NyAR_SAFE_DELETE(this->_buffer_reader);
		return;
	}

	const INyARRgbPixelReader* NyARRgbRaster_BGRA::getRgbPixelReader() const
	{
		return this->_rgb_reader;
	}
	const INyARBufferReader* NyARRgbRaster_BGRA::getBufferReader() const
	{
		return this->_buffer_reader;
	}
	void NyARRgbRaster_BGRA::setBuffer(const NyAR_BYTE_t* i_buffer)
	{
		NyAR_ASSERT(i_buffer!=NULL);
		this->_ref_buf=i_buffer;
		this->_buffer_reader->setBuffer(i_buffer);
	}

}
