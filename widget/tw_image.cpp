#include "tw_image.h"

#define LOG_TAG "TWImage"
TWImage::TWImage(HWND hWnd, int zorder, int id, RECT rect, const string* path, int size)
	: TWView(hWnd, zorder, id, rect){
	
	mSize = size;
	mIndex = 0;

    mPath  = new string[size];
    mImages = new PBITMAP[size];
	
#ifdef RESOURCE_MANAGER_SUPPORT

    for (int index = 0; index < size; index ++)
        mImages[index] = ResourceManager::loadImg(path[index]);
#else	
	for (int index = 0; index < size; index ++){
        mPath[index] = path[index];
		mImages[index] = new BITMAP();
		LOGD("\n TWImage load %s \n ", path[index].c_str());
		LoadBitmap (HDC_SCREEN, mImages[index], path[index].c_str());
	}
#endif
}

//load bmp outside, don't need release
TWImage::TWImage(HWND hWnd, int zorder, int id, RECT rect, PBITMAP* images, int size)
    :TWView(hWnd, zorder, id, rect){
    mSize = size;
	mIndex = 0;
	mPath  = NULL;

    mImages = images;
}


TWImage::~TWImage(){
	LOGD("\n TWImage::~TWImage() id %d\n ", mId);	

    if (mPath != NULL){ //load bmp by self, so release by self 
        
        delete[] mPath;
        
#ifndef RESOURCE_MANAGER_SUPPORT
        for (int index = 0; index < mSize; index ++){
     
    		if (mImages[index] != NULL){
    			UnloadBitmap(mImages[index]);
    			delete mImages[index];           
    		}
	    }
#endif
                
        if (mImages != NULL)
            delete[] mImages;
    }

    mPath = NULL;		
	
}


void TWImage::onDraw(HDC dc){
	if (mVisable != VISABLE)
        return;
    
	if (mImages != NULL && mImages[mIndex] != NULL)
		FillBoxWithBitmap (dc, mRect.left, mRect.top, RECTW(mRect), RECTH(mRect), mImages[mIndex]);
	
}

void TWImage::setShowImage(int index){
	
	if (index < mSize && index >= 0)
		mIndex = index;
	
	InvalidateRect (mWnd, &mRect, TRUE);
}

int TWImage::getShowImage(){
    return mIndex;
}

int TWImage::onTouch(POINT pt, int action){
    return FAILED;
}

int TWImage::onParentTouch(POINT pt, int action){
    return TWView::onTouch(pt, action);
}


