#ifndef ITODAY_UI_MANAGER_H
#define ITODAY_UI_MANAGER_H

#include "page_base.h"

using namespace std;

typedef list<TWPageBase*> LISTPAGES;


class UIManager {
	
	private:
		
		LISTPAGES mPages;
		
		HDC mMemDC;

        HDC mThirdDC;
		
		RECT mRect;
		
		HWND mWnd;

        LockManager* mLockManager;

	public:
		UIManager(const char* res_path);
		
		~UIManager();		
		
		int onCreate(HWND hWnd);
		
		int onDestroy();
		
		int onTouch(POINT pt, int action);
		
		int onTimer(int id, int tick_count);

        void onKey(int key, int state);
		
		void onDraw(HDC dc);

        void setPageVisable(int pageID, int state);

        void transPageData(int pageId, MsgData* data);
		
	private:
		int load(const char* res_path);
		int unload();

        void pendingTransition();

};

#endif