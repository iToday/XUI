#include "tw_progress.h"

#define LOG_TAG "TWProgress"
TWProgress::TWProgress(HWND hWnd, int zorder, int id, RECT rect)
	: TWText(hWnd, zorder, id, rect, "", 0){
		
	mListener = NULL;
	mMax = 100;
    mProgress = 0;

    mLight = NULL;
    mDark = NULL;
    mThumb = NULL;

    mThumbFile.clear();
    mLightFile.clear();
    mDarkFile.clear();
}

TWProgress::~TWProgress(){	
#ifndef RESOURCE_MANAGER_SUPPORT
	 if (!mThumbFile.empty() && mThumb != NULL){
        UnloadBitmap(mThumb);

        delete mThumb;
	 }

     if (!mLightFile.empty() && mLight != NULL){
        UnloadBitmap(mLight);

        delete mLight;
     }

    if (!mDarkFile.empty() && mDark != NULL){
        UnloadBitmap(mDark);

        delete mDark;
    }
#endif
}


void TWProgress::onDraw(HDC dc){
	if (mVisable != VISABLE)
        return;

    if (mDark != NULL)
		FillBoxWithBitmap(dc, mRect.left, mRect.top, RECTW(mRect), RECTH(mRect), mDark);

    float progress = mProgress * 1.0f * RECTW(mRect) / mMax + 0.5f;
	if (mLight != NULL && mProgress > 0)
		FillBoxWithBitmap(dc, mRect.left, mRect.top, (int)progress, RECTH(mRect),mLight);

    if (mThumb != NULL){
        OffsetRect(&mThumbRect, progress - mThumbRect.left, 0);
        FillBoxWithBitmap(dc, mThumbRect.left, mThumbRect.top, RECTW(mThumbRect), RECTH(mThumbRect), mThumb);
    }

	TWText::onDraw(dc);
}

int TWProgress::onTouch(POINT pt, int action){

    if (PtInRect(&mRect, pt.x, pt.y)){
       
        float progress = (pt.x - mRect.left) * mMax / RECTW(mRect);
        LOGD("TWProgress::onTouch progress %d \n", progress);

        if (mProgress != progress){
            mProgress = progress;

            if (mListener != NULL)
                mListener->onProgressChange(mProgress, true);

            //InvalidateRect (mWnd, NULL, TRUE);;
        }
    }
}

void TWProgress::setListener(OnProgressChangeListener* listener){
    mListener = listener;
}

void TWProgress::setImages(PBITMAP light, PBITMAP dark){

    if (!mLightFile.empty() && mLight != NULL){
        UnloadBitmap(mLight);
        delete mLight;
    }

    if (!mDarkFile.empty() && mDark != NULL){
        UnloadBitmap(mDark);
        delete mDark;
    }
    
    mLight = light;
    mDark = dark;
}

void TWProgress::setThumb(PBITMAP thumb, RECT rect){

    if (!mThumbFile.empty() && mThumb != NULL){
        UnloadBitmap(mThumb);
        delete mThumb;
    }
    
    mThumb = thumb;
    mThumbRect = rect;
}

void TWProgress::setImages(string light, string dark){
    LOGD("setImages light %s, dark %s\n", light.c_str(), dark.c_str());
#ifdef RESOURCE_MANAGER_SUPPORT

    mLight = ResourceManager::loadImg(light);
    mDark = ResourceManager::loadImg(dark);
#else
    if (!mLightFile.empty() && mLight != NULL){
        UnloadBitmap(mLight);
        delete mLight;
    }

    if (!mDarkFile.empty() && mDark != NULL){
        UnloadBitmap(mDark);
        delete mDark;
    }

    mLightFile = light;
    mDarkFile = dark;

    mLight = new BITMAP();
    mDark = new BITMAP();

    LoadBitmap (HDC_SCREEN, mLight, mLightFile.c_str());
    LoadBitmap (HDC_SCREEN, mDark, mDarkFile.c_str());
#endif
}

void TWProgress::setThumb(string thumb, RECT rect){
    mThumbRect = rect;
    LOGD("setThumb thumb %s, \n", thumb.c_str());
    
#ifdef RESOURCE_MANAGER_SUPPORT
    mThumb = ResourceManager::loadImg(thumb);
#else
    if (!mThumbFile.empty() && mThumb != NULL){
        UnloadBitmap(mThumb);
        delete mThumb;
    }

    mThumbFile = thumb;
    mThumb = new BITMAP();
    LoadBitmap(HDC_SCREEN, mThumb, mThumbFile.c_str());
#endif
}

void TWProgress::setMax(int max){
    mMax = max;
}

void TWProgress::setProgress(int progress){

    if (mProgress != progress){
        mProgress = progress;

        if (mListener != NULL)
            mListener->onProgressChange(mProgress, false);
    }
}

int TWProgress::getProgress(){
    return mProgress;
}


