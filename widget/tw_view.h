#ifndef ITODAY_VIEW_H
#define ITODAY_VIEW_H

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <stdlib.h>
#include <string.h>

#include "ui/resource_manager.h"
#include <list>
#include <utils/log.h>


#define INVALID_ID -1

#define FAILED 		-1

#define SUCCESS		0

#define MAX_CLICK_DISTANCE 5

#define RESOURCE_MANAGER_SUPPORT 1

#define FONT_FAMILY_KAITI "kaiti"
#define FONT_FAMILY_CLOCK "Clockopia"


enum ACTION{
	TOUCH_DOWN,
	TOUCH_UP,
	TOUCH_MOVE,
};

enum {
	VISABLE,
	INVISABLE,
	GONE,
};

class TWView;
/**
* all view click listener
**/
class OnClickListener {
	public:
		OnClickListener(){
			
		}
		
		virtual ~OnClickListener(){
			
		}
		
		virtual int onClickListener(TWView* view) = 0;
};

/**
* base view ,all view extern from TWView
**/
class TWView {
	protected:

		//draw order
		int mZOrder;
		//id 
		int mId;
		
		string mName;
		
		OnClickListener* mClickListener;
		
		RECT mRect;
		
		HWND mWnd;

        POINT mLast;

        bool mDown;

        void* mDataExtra;

        int mVisable;
		
	public:
		TWView(HWND hWnd);
		TWView(HWND hWnd, int zorder, int id, RECT rect);
		virtual ~TWView();
		
		int getId();
		
		void setName(const char* name);
		
		bool is(const char* name);
		
		int setOnClickListener(OnClickListener* listener);
		
		bool isIn(POINT pt);
		
		virtual void onDraw(HDC dc) = 0;
		
		int compareZOrder(TWView* view);

        virtual int offset(int x, int y);
        
        void moveTo(int x, int y);

        RECT* getRect();

        void setRect(RECT rect);
		
		virtual int onTouch(POINT pt, int action);

        void setExtra(void* data);

        void* getExtra();

        void setVisable(int visable);
		
		int getVisable();

        bool isOverlap(RECT* rect);
		
};

typedef list<TWView*> LISTVIEWS;

#endif
