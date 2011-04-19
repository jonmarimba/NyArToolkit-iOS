#include "NyAR_core.h"

#define CODE_FILE "../../data/patt.hiro"
#define DATA_FILE "../../data/320x240ABGR.raw"
#define CAMERA_FILE "../../data/camera_para.dat"
#include <cstdio>
#include <iostream>
#include <fstream>
//#include <windows.h>
using namespace NyARToolkitCPP;
using namespace std;

unsigned char* __load_file_image(const char* i_filename)
{
	ifstream inf;
	unsigned char* result=NULL;
	try {
		
		inf.open(i_filename,ios::binary);
		int size=320*240*4;
		result=new unsigned char[size];
		inf.read((char*)result,size);
		if(size!=inf.gcount()){
			throw exception();
		}
		inf.close();
	} catch(exception e){
		if(result!=NULL){
			delete result;
		}
		throw;
	}
	return result;
}

int __win_main(int argc,char* argv[])
{
	{
	// ARópÉJÉÅÉâÉpÉâÉÅÉ^ÉtÉ@ÉCÉãÇÉçÅ[Éh
	NyARParam ap;
	//CPUÇÃÉGÉìÉfÉBÉAÉìÇéwíËÇµÇƒâ∫Ç≥Ç¢ÅB
	ap.setEndian(TNyAREndian_LITTLE);
	ap.loadARParamFromFile(CAMERA_FILE);
	ap.changeScreenSize(320, 240);

	// ARópÇÃÉpÉ^Å[ÉìÉRÅ[ÉhÇì«Ç›èoÇµ
	NyARCode* code=NULL;
	unsigned char* buf;
	NyARRgbRaster_BGRA* ra;
	NyARSingleDetectMarker* ar;


	code= new NyARCode(16, 16);
	code->loadARPattFromFile(CODE_FILE);
	buf=__load_file_image(DATA_FILE);
	ra = new NyARRgbRaster_BGRA(320, 240);
	//ÉâÉbÉvÇ∑ÇÈÉoÉbÉtÉ@ÇÉZÉbÉgÇ∑ÇÈÅB
	ra->setBuffer(buf);

	// ÇPÉpÉ^Å[ÉìÇÃÇ›Çí«ê’Ç∑ÇÈÉNÉâÉXÇçÏê¨
	ar = new NyARSingleDetectMarker(ap, code, 80.0);
	code=NULL;/*codeÇÃèäóLå†ÇÕNyARSingleDetectMarkerÇ÷ìnà⁄ìÆ*/
	ar->setContinueMode(false);

	NyARTransMatResult result_mat;
	ar->detectMarkerLite(*ra, 100);
	ar->getTransmationMatrix(result_mat);
	printf("%s",
		"performance test of NyARToolkit.\n"
		"This test measures processing time of marker detection and transform matrix calculation 1000 times.\n");
	printf("Marker confidence\n cf=%f,direction=%d\n",ar->getConfidence(),ar->getDirection());
	printf("Transform Matrix\n");
	printf(
		"% 4.8f,% 4.8f,% 4.8f,% 4.8f\n"
		"% 4.8f,% 4.8f,% 4.8f,% 4.8f\n"
		"% 4.8f,% 4.8f,% 4.8f,% 4.8f\n",
		result_mat.m00,result_mat.m01,result_mat.m02,result_mat.m03,
		result_mat.m10,result_mat.m11,result_mat.m12,result_mat.m13,
		result_mat.m20,result_mat.m21,result_mat.m22,result_mat.m23);

	//DWORD st=GetTickCount();

	// É}Å[ÉJÅ[Çåüèo1000âÒï™ÇÃèàóùéûä‘Çåvë™
	for (int i = 0; i < 1000; i++) {
		// ïœä∑çsóÒÇéÊìæ
		ar->detectMarkerLite(*ra, 100);
		ar->getTransmationMatrix(result_mat);
	}
	//printf("done.\ntotal=%u[ms]\n",GetTickCount()-st);
	
	delete ra;
	delete ar;
	delete buf;
	}
	//_CrtDumpMemoryLeaks();
	return 0;
}