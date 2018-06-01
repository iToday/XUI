
#include "ui_manager.h"

//#define DEBUG_TIME
#define LOG_TAG "UIManager"

UIManager::UIManager(const char* res_path){
	
	mMemDC = NULL;
    mThirdDC = NULL;

    mLockManager = LockManager::instance();   
}

UIManager::~UIManager(){
	
	unload();
}

void UIManager::onKey(int key, int state){
	
    LISTPAGES::iterator plist;

    for(plist = mPages.begin(); plist != mPages.end(); plist++){
		
		if ((*plist)->getVisable() == VISABLE
          && (*plist)->getId() != ID_PAGE_STATUS_BAR){
			(*plist)->onKey(key, state);
            break;
		}
	} 
}

int UIManager::onTouch(POINT pt, int action){
	
	LISTPAGES::reverse_iterator  plist; 

    static int last_action = TOUCH_UP;
    
    if (last_action == TOUCH_UP && action == TOUCH_MOVE){
        
        action = TOUCH_DOWN;
    }    
	
	for(plist = mPages.rbegin(); plist != mPages.rend(); plist++){
        if(((*plist)->getVisable() == VISABLE))
		    if ((*plist)->onTouch(pt, action) == SUCCESS)
              break;
	}

    last_action = action;    

	return SUCCESS;
}
		
int UIManager::onTimer(int id, int tick_count){
	
	int pageId = PAGE_ID_FROM_TIMER_ID(id);
	int timerId = SRC_TIMER_ID_FROM_TIMER_ID(id);

	
	LISTPAGES::iterator plist; 
	
	for(plist = mPages.begin(); plist != mPages.end(); plist++){
		
		if ((*plist)->getId() == pageId)
			(*plist)->onTimer(timerId, tick_count);
	}
		
	return SUCCESS;
}

void UIManager::pendingTransition(){
    int times = TRANSSITION_MAX_TIMES;
    int x = 0;
    int y = 0;
    int dis_x = RECTW(mRect)/(times);
    int dis_y = RECTH(mRect)/(times);

    LISTPAGES::iterator plist; 
    
    if (mMemDC == NULL)
		mMemDC = CreateCompatibleDC (HDC_SCREEN);	
	
	for(plist = mPages.begin(); plist != mPages.end(); plist++){
		(*plist)->onDraw(mMemDC);
	}

    while(times --){

        switch(TWPageBase::getTransition()){
        case RIGHT:
            BitBlt(mMemDC, RECTW(mRect)- x, mRect.top, x, RECTH(mRect), HDC_SCREEN, mRect.left, mRect.top, 0);
            break;
        case LEFT:
            BitBlt(mMemDC, mRect.left, mRect.top, x, RECTH(mRect), HDC_SCREEN,  RECTW(mRect)- x, mRect.top, 0);
            break;
        case UP:
            BitBlt(mMemDC, mRect.left, mRect.top, RECTW(mRect), y, HDC_SCREEN,  mRect.left, RECTH(mRect) - y, 0);
            break;
        case DOWN:
            BitBlt(mMemDC, mRect.left, RECTH(mRect) - y, RECTW(mRect), y, HDC_SCREEN, mRect.left, mRect.top, 0);
            break;
        default:
            break;
        }

        y += dis_y;    
        x += dis_x;
    }

}
void UIManager::setPageVisable(int pageID, int state){

    LISTPAGES::iterator plist; 
    bool bTransition = false;
	
	for(plist = mPages.begin(); plist != mPages.end(); plist++){
		
		if ((*plist)->getId() == pageID){
			if ((*plist)->setVisable(state)){
               bTransition = true;               
			}
		} else if ((*plist)->getId() != ID_PAGE_STATUS_BAR){
            (*plist)->setVisable(INVISABLE);
		}
	}

    if (bTransition)
        pendingTransition();
}

 void UIManager::transPageData(int pageId, MsgData* data){
     LISTPAGES::iterator plist; 
	
	for(plist = mPages.begin(); plist != mPages.end(); plist++){
		
		if ((*plist)->getId() == pageId){
			(*plist)->onNewData(data);
            break;
		}
	} 

    if (data != NULL)
        delete data;
 }

void UIManager::onDraw(HDC dc){
	
	LISTPAGES::iterator plist;

	if (mMemDC == NULL)
		mMemDC = CreateCompatibleDC (dc);

    if (mThirdDC == NULL)
        mThirdDC = CreateCompatibleDC(dc);
	
	for(plist = mPages.begin(); plist != mPages.end(); plist++){
		
        if(((*plist)->getVisable() == VISABLE)){
            (*plist)->onDraw(mThirdDC);
            RECT pageRect;
           
            (*plist)->getInvalidRect(&pageRect);        
            BitBlt(mThirdDC, pageRect.left, pageRect.top, RECTW(pageRect), RECTH(pageRect), mMemDC, pageRect.left, pageRect.top, 0);
        }
	}
    
    BitBlt(mMemDC, mRect.left, mRect.top, RECTW(mRect), RECTH(mRect), dc, mRect.left, mRect.top, 0);
    
}

int UIManager::load(const char* res_path){
	
	mPages.push_back(new TWPage(...));
	
	return SUCCESS;
}

int UIManager::unload(){
	
	LISTPAGES::iterator plist; 
	for(plist = mPages.begin(); plist != mPages.end(); plist++)
		delete *plist;

    ResourceManager::release();
	
	return SUCCESS;
}

int UIManager::onCreate(HWND hWnd){
	mWnd = hWnd;
	
	GetWindowRect(hWnd, &mRect);
	
	load(NULL);
		
	return SUCCESS;
}

int UIManager::onDestroy(){
	
	if (mMemDC != NULL)
		DeleteCompatibleDC (mMemDC);

    if (mThirdDC != NULL)
        DeleteCompatibleDC(mThirdDC);
	
	mMemDC = NULL;
    mThirdDC = NULL;
	return SUCCESS;
}