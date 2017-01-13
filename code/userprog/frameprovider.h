#ifdef CHANGED
#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "utility.h"
#include "bitmap.h"


class FrameProvider{
	public:
		FrameProvider();
		~FrameProvider();
		
		int GetEmptyFrame();
		void ReleaseFrame(int frame);
		unsigned int NumAvailFrame();
	
	private: 
		BitMap* FrameMap;
};
#endif	// FRAMEPROVIDER_H
#endif //CHANGED
