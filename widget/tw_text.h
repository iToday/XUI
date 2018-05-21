#ifndef ITODAY_TEXT_H
#define ITODAY_TEXT_H

#include "tw_view.h"

class TWText : public TWView{
	
	protected:	
		
		char* mText;
		
		int mFormat;

        string mBkgPath;
		PBITMAP mBackground;
		
		int mIndent;
		
		int mColor;
		
		PLOGFONT mFont;
	
	public:
		TWText(HWND hWnd, int zorder, int id, RECT rect, const char* text, int format);
		TWText(HWND hWnd, int zorder, int id, RECT rect, const char* text, int format, const char* background);
        TWText(HWND hWnd, int zorder, int id, RECT rect, const char* text, int format, PBITMAP background);
		~TWText();
		
		void setBackground(const char* background);
		void setBackground(PBITMAP background);
		
		void setTextFormat(int format);
		
		void setIndent(int indent);
		
		void setSize(int size);
		
		void setText(const char* text);
		
		void setTextColor(int color);

		void setFont(PLOGFONT font);
		
		virtual void onDraw(HDC dc);

};

#endif