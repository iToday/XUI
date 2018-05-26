
#include "tw_button.h"
#include <string.h>
#define LOG_TAG "TWButton"
TWButton::TWButton(HWND hWnd, int zorder, int id, RECT rect, const string* path, int size)
	: TWImage(hWnd, zorder, id, rect, path, size){
		
	mFormat = DT_CENTER | DT_WORDBREAK | DT_VCENTER;
	
	mTextRect = rect;
	
	mIndent = 0;
	
	mText = NULL;
	
	mFont = NULL;
	
	mColor = RGB2Pixel(HDC_SCREEN, 0xA4, 0xAD, 0xBA);
}

TWButton::TWButton(HWND hWnd, int zorder, int id, RECT rect, PBITMAP* imges, int size)
   : TWImage(hWnd, zorder, id, rect, imges, size){
	mFormat = DT_CENTER | DT_WORDBREAK | DT_VCENTER ;
	
	mTextRect = rect;
	
	mIndent = 0;
	
	mText = NULL;
	
	mFont = NULL;
	
	mColor = RGB2Pixel(HDC_SCREEN, 0xA4, 0xAD, 0xBA);

}

TWButton::TWButton(HWND hWnd, int zorder, int id, RECT rect, const string* path, int size, const char* text, int format)
	: TWImage(hWnd, zorder, id, rect, path, size){
	
	mFormat = format;
	
	mTextRect = rect;
	
	mIndent = 0;
	
	mText = NULL;
	
	mFont = NULL;
	
	mColor = RGB2Pixel(HDC_SCREEN, 0xA4, 0xAD, 0xBA);
	
	setText(text);
}

TWButton::TWButton(HWND hWnd, int zorder, int id, RECT rect, PBITMAP* imges, int size, const char* text, int format)
    : TWImage(hWnd, zorder, id, rect, imges, size){
    mFormat = format;
	
	mTextRect = rect;
	
	mIndent = 0;
	
	mText = NULL;
	
	mFont = NULL;
	
	mColor = RGB2Pixel(HDC_SCREEN, 0xA4, 0xAD, 0xBA);
	
	setText(text);
}

TWButton::~TWButton(){
	LOGD("\n TWButton::~TWButton() id %d\n ", mId);
	
	if (mText != NULL)
		delete mText;
}

void TWButton::onDraw(HDC dc){

     if (mVisable != VISABLE)
        return;
    
	TWImage::onDraw(dc);
	
	PLOGFONT lastFont = GetCurFont(dc);	
	
	if (mFont != NULL)
		SelectFont(dc, mFont);
	
	SetBkMode (dc, BM_TRANSPARENT);
    SetTextColor(dc, mColor);
	
	if (mText != NULL)
		DrawTextEx (dc, mText, -1, &mTextRect, mIndent, mFormat);
	
	if (mFont != NULL)
		SelectFont(dc, lastFont);
}

void TWButton::setState(TM_BUTTON_STATUS state){
	setShowImage(state);
}

TM_BUTTON_STATUS TWButton::getState(){
    return TM_BUTTON_STATUS(getShowImage());
}

int TWButton::onTouch(POINT pt, int action){
	
	if (mIndex != TW_BUTTON_DISABLE){
		
		if (isIn(pt) && action == TOUCH_DOWN){
			setState(TW_BUTTON_PRESSED);
		} else {
			setState(TW_BUTTON_NORMAL);
		}
		
	}
	
	return TWView::onTouch(pt, action);
}

void TWButton::setTextColor(int color){
	
	mColor = RGB2Pixel(HDC_SCREEN, color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF);
}

void TWButton::setTextRect(RECT rect){
	
	mTextRect = rect;
}

int TWButton::offset(int x, int y){
    TWView::offset(x, y);
    setTextOffset(x, y);

    return 0;
}

void TWButton::setTextOffset(int x, int y){
    OffsetRect(&mTextRect, x, y);
}

void TWButton::setFont(PLOGFONT font){
    mFont = font;
}

const char* TWButton::getText(){
    return mText;
}

void TWButton::setText(string text){
    setText(text.c_str());
}

void TWButton::setText(const char* text){
	
	if (mText != NULL)
		delete mText;
		
	if (text == NULL) return;
	
	mText = new char[strlen(text) + 1];
	
	memcpy(mText, text, strlen(text) + 1);
}

void TWButton::setTextFormat(int format){
	mFormat = format;
}

void TWButton::setIndent(int indent){
	mIndent = indent;
}