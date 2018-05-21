#ifndef TOMWIN_BUTTON_H
#define TOMWIN_BUTTON_H

#include "tw_image.h"

enum TM_BUTTON_STATUS {
	TW_BUTTON_NORMAL, /**/
	TW_BUTTON_PRESSED,
	TW_BUTTON_DISABLE,
	TW_BUTTON_FOCUS,
	
	TW_BUTTON_STATUS_MAX,
};


class TWButton : public TWImage{
	
	private:
		char* mText;
		
		int mFormat;
		
		int mIndent;
		
		int mColor;
		
		PLOGFONT mFont;
		
		RECT mTextRect;
		
	public:
		TWButton(HWND hWnd, int zorder, int id, RECT rect, const string* path, int size);
        TWButton(HWND hWnd, int zorder, int id, RECT rect, PBITMAP* imges, int size);
		
		TWButton(HWND hWnd, int zorder, int id, RECT rect, const string* path, int size, const char* text, int format);

        TWButton(HWND hWnd, int zorder, int id, RECT rect, PBITMAP* imges, int size, const char* text, int format);
		~TWButton();		
		
		virtual int onTouch(POINT pt, int action);
		
		void setState(TM_BUTTON_STATUS state);

        TM_BUTTON_STATUS getState();
		
		void setTextFormat(int format);
		
		void setIndent(int indent);
		
		void setSize(int size);
		
		void setText(const char* text);

        const char* getText();
		
		void setTextColor(int color);
		
		void setTextRect(RECT rect);

        void setTextOffset(int x, int y);

        virtual int offset(int x, int y);
		
        void setFont(PLOGFONT font);
		
		virtual void onDraw(HDC dc);
};

#endif