#include "tw_text.h"

#define LOG_TAG "TWText"

TWText::TWText(HWND hWnd,int zorder, int id, RECT rect, const char* text, int format)
	: TWView(hWnd, zorder, id, rect){
	
	mFormat = format;
	
	mIndent = 0;
	
	mBackground = NULL;
	
	mText = NULL;
	
	mFont = NULL;
	
	mColor = RGB2Pixel(HDC_SCREEN, 0x0E, 0x86, 0xFF);
	
	setText(text);
	
}

TWText::TWText(HWND hWnd, int zorder, int id, RECT rect, const char* text, int format, const char* background)
	: TWView(hWnd, zorder, id, rect){
	
	mFormat = format;	
		
	mText = NULL;
	
	mFont = NULL;
	
	mColor = RGB2Pixel(HDC_SCREEN, 0x0E, 0x86, 0xFF);
	
	setText(text);
	
	if (background != NULL){

     #ifdef RESOURCE_MANAGER_SUPPORT
        mBackground = ResourceManager::loadImg(background);
     #else
		mBackground = new BITMAP();
		mBkgPath = string(background);
		LOGD("\n TWText load %s \n ", background);
		LoadBitmap (HDC_SCREEN, mBackground, background);
     #endif
		
	}
	
}

TWText::TWText(HWND hWnd, int zorder, int id, RECT rect, const char* text, int format, PBITMAP background)
    : TWView(hWnd, zorder, id, rect){
    mFormat = format;	
		
	mText = NULL;
	
	mFont = NULL;
	
	mColor = RGB2Pixel(HDC_SCREEN, 0x0E, 0x86, 0xFF);
	
	setText(text);

    mBkgPath.clear();

    mBackground = background;
}

TWText::~TWText(){

    LOGD("\n TWText::~TWText() id %d\n ", mId);
	
	if (mText != NULL)
		delete[] mText;

	if (mBackground != NULL && !mBkgPath.empty()){
#ifndef RESOURCE_MANAGER_SUPPORT
		UnloadBitmap(mBackground);
		delete mBackground;
#endif
	}

}

void TWText::onDraw(HDC dc){

    if (mVisable != VISABLE)
        return;
    
	if (mBackground != NULL)
		FillBoxWithBitmap (dc, mRect.left, mRect.top, RECTW(mRect), RECTH(mRect), mBackground);

	PLOGFONT lastFont = GetCurFont(dc);	
	
	if (mFont != NULL)
		SelectFont(dc, mFont);
	
	SetBkMode (dc, BM_TRANSPARENT);
    SetTextColor(dc, mColor);
	
	if (mText != NULL)
		DrawTextEx (dc, mText, -1, &mRect, mIndent, mFormat);
	
	if (mFont != NULL)
		SelectFont(dc, lastFont);
	
}

void TWText::setTextColor(int color){
	mColor = RGB2Pixel(HDC_SCREEN, color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
}

void TWText::setFont(PLOGFONT font){
    mFont = font;
}

void TWText::setText(const char* text){
	
	if (mText != NULL)
		delete[] mText;

    mText = NULL;

    if (text == NULL)
        return;
	
	mText = new char[strlen(text) + 1];
	
	memcpy(mText, text, strlen(text) + 1);
}

void TWText::setBackground(const char* background){
	
	if (mBackground != NULL && !mBkgPath.empty()){
#ifndef RESOURCE_MANAGER_SUPPORT
        UnloadBitmap(mBackground);
        delete mBackground;
#endif

        mBkgPath.clear();
	}
	
	if (background != NULL){
       
     #ifdef RESOURCE_MANAGER_SUPPORT
        mBackground = ResourceManager::loadImg(background);
     #else
		mBackground = new BITMAP();
		mBkgPath = string(background);
		LOGD("\n TWText load %s \n ", background);
		LoadBitmap (HDC_SCREEN, mBackground, background);
     #endif
	}
}

void TWText::setBackground(PBITMAP background){
	mBackground = background;
}

void TWText::setTextFormat(int format){
	mFormat = format;
}

void TWText::setIndent(int indent){
	mIndent = indent;
}
