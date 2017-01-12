#ifdef CHANGED

#include "machine.h"
#include "frameprovider.h"

FrameProvider::FrameProvider(){
	FrameMap = new BitMap(NumPhysPages);
}

FrameProvider::~FrameProvider(){
	delete FrameMap;
}

unsigned int
FrameProvider::NumAvailFrame(){
	return FrameMap->NumClear ();
}

int 
FrameProvider::GetEmptyFrame(){
	return FrameMap->Find();
}

void FrameProvider::ReleaseFrame(int n){
	FrameMap->Clear(n);
}
#endif //CHANGED1
