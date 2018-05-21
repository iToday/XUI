#ifndef ITODAY_PAGE_BASE_H
#define ITODAY_PAGE_BASE_H

#include <list>
#include <typeinfo>

#include "widget/tw_view.h"
#include "widget/tw_button.h"
#include "widget/tw_image.h"
#include "widget/tw_list.h"
#include "widget/tw_text.h"
#include "widget/tw_radio.h"
#include "widget/tw_video.h"
#include "widget/tw_progress.h"
#include "widget/tw_date_time.h"

#ifdef _LANG_ZHCN
#include "../res_cn.h"
#elif defined _LANG_ZHTW
#include "../res_tw.h"
#else
#include "../res_en.h"
#endif


#define ID_BASE					0xFF

using namespace std;

#define MSG_SET_PAGE_VISABLE MSG_USER + 1
#define MSG_SEND_DATA_TO_PAGE MSG_USER + 2

#define MSG_SEND_KEY        MSG_USER + 3

//TIMER_ID (PAGE_ID SRC_TIMER_ID)
#define MAKE_TIMER_ID(PAGE_ID, SRC_TIMER_ID)	(PAGE_ID << ((sizeof(int) - 1) * 8) | SRC_TIMER_ID)
#define PAGE_ID_FROM_TIMER_ID(TIMER_ID)			(TIMER_ID >> ((sizeof(int) - 1) * 8) & 0xFF)
#define SRC_TIMER_ID_FROM_TIMER_ID(TIMER_ID)	(TIMER_ID & (~(0xFF << ((sizeof(int) - 1) * 8))))


#define ID_PAGE_BASE 		0x0
 
#define ID_PAGE_STATUS_BAR 	    (ID_PAGE_BASE + 1)
#define ID_PAGE_MAIN 		    (ID_PAGE_BASE + 2)
#define ID_PAGE_SCREEN_SAVE	    (ID_PAGE_BASE + 3)
#define ID_PAGE_MANAGER		    (ID_PAGE_BASE + 4)
#define ID_PAGE_USER_PASSWORD   (ID_PAGE_BASE + 5)
#define ID_PAGE_USER_NAME       (ID_PAGE_BASE + 6)
#define ID_PAGE_BROWSER         (ID_PAGE_BASE + 7)
#define ID_PAGE_SOUND           (ID_PAGE_BASE + 8)
#define ID_PAGE_LOCK_MODE       (ID_PAGE_BASE + 9)
#define ID_PAGE_SETTINGS        (ID_PAGE_BASE + 10)
#define ID_PAGE_WIFI            (ID_PAGE_BASE + 11)
#define ID_PAGE_FACE            (ID_PAGE_BASE + 12)

enum DIRECTION{
        NONE,
        LEFT,
        RIGHT,
        UP,
        DOWN,
};

#define GESTURE_MAX_DISTANCE    20
#define GESTURE_SLOPE           0.3f

#define KEY_VOLUME_ADD      2
#define KEY_VOLUME_SUB      3
#define KEY_VOLUME_MUTE     4

/*enum KEY_STATE{
    KEY_DOWN,
    KEY_UP,
};*/

#define SUPPORT_GESTURE     0

enum PAGE_STYLE{
    NONE_S,
    SCROLL = 1,
};

#define TRANSSITION_MAX_TIMES 30

class MsgData{
    private:
        int mLength;
        char* mData;
        int mExtra;
    public:
        MsgData(int len, const char* data, int extra = 0);
        ~MsgData();

        int length();
        const char* getData();

        int getExtra();
};

class TWPageBase : public TWView, public OnClickListener{
	
	protected:	
		
        string mBkgPath;
        
		PBITMAP mBackground;
		
		LISTVIEWS mViews;
		
		TWView* mLastTouchView;

        POINT last;
        int last_action;

        PAGE_STYLE mStyle;

        static DIRECTION mTransition;
		
	public:
		TWPageBase(HWND hWnd, int id, RECT rect);
		TWPageBase(HWND hWnd, int id, RECT rect, const char* background, int visable);
        TWPageBase(HWND hWnd, int id, RECT rect, const char* background, int visable, PAGE_STYLE style);
		
		virtual ~TWPageBase();
		
		void init(const char* background, int visable);
		
		void addView(TWView* view);
				
		TWView* findViewbyId(int id);
		
		void setBackground(const char* background);

        void setPageVisable(int id, int state);

        void sendDataToPage(int targetPageId, const char* data, int len, int extra = 0);
		
		virtual void onDraw(HDC dc);
				
		virtual int onTouch(POINT pt, int action);
		
		virtual int onTimer(int id, int tick_count);

        virtual bool onGesture(DIRECTION direction );

        virtual void onKey(int key, int state);

        virtual int onPause();

        virtual int onResume();

        virtual int onNewData(MsgData* data);

        virtual void getInvalidRect(RECT* pRect);

        bool setVisable(int visable);

        static void setTransition(DIRECTION direction);
        static DIRECTION getTransition();

        virtual void overridePendingTransition();

    private:
        int transTouch(POINT pt, int action);
        bool canScroll(int offset);
        bool scroll(int offset);

        void resetScroll();

};

#endif