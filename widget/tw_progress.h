#ifndef ITODAY_PROGRESS_H
#define ITODAY_PROGRESS_H

#include <string>

#include "tw_text.h"

using namespace std;

enum PROGRESS_BACKGROUND{
	BKG_LIGHT,
	BKG_DARK,
	
	PROGRESS_BACKGROUND_SIZE,
};

class OnProgressChangeListener{
    public:
        virtual void onProgressChange(int progress, bool byUser) = 0;
};

class TWProgress : public TWText{
	
	protected:
		
		PBITMAP mLight;
        PBITMAP mDark;
        PBITMAP mThumb;

        string mLightFile;
        string mDarkFile;
        string mThumbFile;

        RECT mThumbRect;

        int mMax;
        int mProgress;

        OnProgressChangeListener* mListener;
	
	public:		
        TWProgress(HWND hWnd, int zorder, int id, RECT rect);
		~TWProgress();
		
		virtual void onDraw(HDC dc);

        virtual int onTouch(POINT pt, int action);

        void setListener(OnProgressChangeListener* listener);

        void setImages(PBITMAP light, PBITMAP dark);
		
		void setThumb(PBITMAP thumb, RECT rect);

        void setImages(string light, string dark);
		
		void setThumb(string thumb, RECT rect);

        void setMax(int max);

        void setProgress(int progress);

        int getProgress();
};

#endif