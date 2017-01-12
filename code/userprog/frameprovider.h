#ifdef CHANGED
#ifndef FRAMEPROVIDER_H
#define FRAMEPROVIDER_H

#include "copyright.h"
#include "utility.h"

struct frame_info{
	bool status;
	int id;
};

class FrameProvider{
	public:
		FrameProvider();
		~FrameProvider();
		
		int GetEmptyFrame();
		void ReleaseFrame(int frame);
		unsigned int NumAvailFrame();
	
	private: 
		frame_info* FrameMap;
		unsigned int AvailFrame;
};
#endif	// FRAMEPROVIDER_H
#endif //CHANGED
