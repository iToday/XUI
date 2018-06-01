#ifndef ITODAY_IMAGE_H
#define ITODAY_IMAGE_H

#include <string>

#include "tw_view.h"

using namespace std;

class TWImage : public TWView{
	
	protected:
	
		int mIndex;

        string* mPath;
		
		PBITMAP* mImages;
		
		int mSize;
	
	public:
        TWImage(TWImage* img);
		TWImage(HWND hWnd, int zorder, int id, RECT rect, const string* path, int size);
        TWImage(HWND hWnd, int zorder, int id, RECT rect, PBITMAP* images, int size);
		~TWImage();
		
		virtual void onDraw(HDC dc);

        virtual int onTouch(POINT pt, int action);

        int onParentTouch(POINT pt, int action);
		
		void setShowImage(int index);

        int getShowImage();
};

#endif