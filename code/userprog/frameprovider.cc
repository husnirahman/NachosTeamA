#ifdef CHANGED

#include "machine.h"
#include "frameprovider.h"

FrameProvider::FrameProvider(){
	FrameMap = new frame_info[NumPhysPages];
	int i;
	for(i = 0; i < NumPhysPages; i++){
		FrameMap[i].status = TRUE;
		FrameMap[i].id     = i;
	} 
	AvailFrame = NumPhysPages;
}

FrameProvider::~FrameProvider(){
	delete FrameMap;
}

unsigned int
FrameProvider::NumAvailFrame(){
	return AvailFrame;
}

int 
FrameProvider::GetEmptyFrame(){
	int j = 0;
	while(j < NumPhysPages){
		if(FrameMap[j].status == TRUE){
			FrameMap[j].status = FALSE;
			AvailFrame--;
			break;
		}
		j++;
	}
	return (FrameMap[j].id);
}

void FrameProvider::ReleaseFrame(int n){
	FrameMap[n].status = TRUE;
	AvailFrame++;
}
#endif //CHANGED1
