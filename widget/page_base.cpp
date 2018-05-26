
#include "page_base.h"
#include <unistd.h>

#define LOG_TAG "PageBase"

MsgData::MsgData(int len, const char* data, int extra ){

    mLength = len;
    mExtra = extra;

    if (data == NULL){
       mData = NULL;

       return;
    }

    mData = new char[len];

    memcpy(mData, data, len);
}
MsgData::~MsgData(){

    if (mData != NULL)
        delete mData;
}

int MsgData::length(){
    return mLength;
}

const char* MsgData::getData(){
    return mData;
}

int MsgData::getExtra(){

    return mExtra;
}

DIRECTION TWPageBase::mTransition = LEFT;

string TWPageBase::loadString(const char* name){
    return XmlLoader::loadString(name);
}

TWPageBase::TWPageBase(HWND hWnd, int id, RECT rect)
	: TWView(hWnd, 0, id, rect){
	mVisable = GONE;
	mBackground = NULL;
	mLastTouchView = NULL;
    last_action = TOUCH_UP;
    last.x = 0;
    last.y = 0;
    mStyle = SCROLL;
}

TWPageBase::TWPageBase(HWND hWnd, int id, RECT rect, const char* background, int visable)
	: TWView(hWnd, 0, id, rect){
		
	mBackground = NULL;
    mVisable = visable;
	setBackground(background);	
	mLastTouchView = NULL;
    last_action = TOUCH_UP;
    last.x = 0;
    last.y = 0;
    mStyle = SCROLL;
}

TWPageBase::TWPageBase(HWND hWnd, int id, RECT rect, const char* background, int visable, PAGE_STYLE style)
    : TWView(hWnd, 0, id, rect){

    mBackground = NULL;
    mVisable = visable;
	setBackground(background);	
	mLastTouchView = NULL;
    last_action = TOUCH_UP;
    last.x = 0;
    last.y = 0;

    mStyle = style;
}

void TWPageBase::setContentView(const char* xml){

    mxml_node_t* tree = XmlLoader::load(xml);

    if (tree != NULL){

        mxml_node_t *node = NULL;

        node = mxmlFindElement(tree, tree, CLS_PAGE , NULL, NULL, MXML_DESCEND);

        if (node != NULL){            

            setBackground(XmlLoader::getDrawableAttr(node));
            mRect = XmlLoader::getRectAttr(node);
            mVisable = XmlLoader::getVisableAttr(node);
            mStyle = PAGE_STYLE(XmlLoader::getStyleAttr(node));
            
            LISTVIEWS views = XmlLoader::createWidgets(node, mWnd);

            LISTVIEWS::iterator plist; 
	
        	for(plist = views.begin(); plist != views.end(); plist++)
        		addView(*plist);

            views.clear();
            
        }
        mxmlDelete(tree);
    }    
}
		
TWPageBase::~TWPageBase(){

	LISTVIEWS::iterator plist; 
	
	for(plist = mViews.begin(); plist != mViews.end(); plist++)
		delete *plist;
	
	mViews.clear();
	
	mLastTouchView = NULL;
}

void TWPageBase::init(const char* background, int visable){
	
	setBackground(background);
	
	mVisable = visable;

}

void TWPageBase::addView(TWView* view){
	
	if (view != NULL){
		mViews.push_back(view);
		view->setOnClickListener(this);
	}
		
}

TWView* TWPageBase::findViewbyId(int id){
	
	LISTVIEWS::iterator plist; 
	
	for(plist = mViews.begin(); plist != mViews.end(); plist++)
		if ((*plist)->getId() == id)
			return *plist;
		
	return NULL;
}

TWView* TWPageBase::findViewbyName(const char* name){
		
	return findViewbyName(mViews, name);
}

TWView* TWPageBase::findViewbyName(LISTVIEWS widgets, const char* name){
	
	LISTVIEWS::iterator plist; 
	
	for(plist = widgets.begin(); plist != widgets.end(); plist++)
		if ((*plist)->is(name))
			return *plist;
		
	return NULL;
}

bool TWPageBase::onGesture(DIRECTION direction){
    return false;
}

void TWPageBase::setTransition(DIRECTION direction){
    mTransition = direction;
}

DIRECTION TWPageBase::getTransition(){
    return mTransition;
}

void TWPageBase::overridePendingTransition(){
    return;
}

void TWPageBase::getInvalidRect(RECT* pRect){

    memcpy(pRect, &mRect, sizeof(RECT));
}


bool TWPageBase::setVisable(int visable){

    if (mVisable != visable){
	    mVisable = visable;

        if (mVisable == VISABLE){

            onResume();

            overridePendingTransition();
            
        }else if (mVisable == INVISABLE){
        
            onPause();

        }

        return true;
    }

    return false;
}

void TWPageBase::setBackground(const char* background){

    mBackground = ResourceManager::loadImg(background);
	
}

 void TWPageBase::setPageVisable(int id, int state){

    if (mWnd != NULL)
        PostMessage(mWnd, MSG_SET_PAGE_VISABLE, id, state);
    else
        LOGD("setPageVisable wnd is NULL \n");
 }


 void TWPageBase::sendDataToPage(int targetPageId, const char* data, int len, int extra){

    if (mWnd != NULL){
        MsgData* pData = new MsgData(len, data, extra);
        SendMessage(mWnd, MSG_SEND_DATA_TO_PAGE, targetPageId, (LPARAM)pData);
    }
 }


 int TWPageBase::onNewData(MsgData* data){

    return 0;
 }

 void TWPageBase::onKey(int key, int state){

 }

void TWPageBase::onDraw(HDC dc){
	LISTVIEWS::iterator plist; 
	
	if (mVisable != VISABLE) 
        return;
	
	if (mBackground != NULL)
		FillBoxWithBitmap (dc, mRect.left, mRect.top, RECTW(mRect), RECTH(mRect), mBackground);
	
	for(plist = mViews.begin(); plist != mViews.end(); plist++){
        if (!isOverlap((*plist)->getRect())){
            //printf("the view is out of page rect ,don't draw it\n");
            continue;
        }
        
		(*plist)->onDraw(dc);
	}
	
}

 bool TWPageBase::scroll(int offset){
 
     if (!canScroll(offset)){
         LOGD("can not Scroll offset %d \n", offset);
         return false;
     }
 
     LISTVIEWS::iterator plist; 
     
     for(plist = mViews.begin(); plist != mViews.end(); plist++){
         
         (*plist)->offset(0, offset);
     }   

     return true;
 }
 
 bool TWPageBase::canScroll(int offset){
     LISTVIEWS::iterator pBegin;
     LISTVIEWS::iterator pEnd; 

     if (mViews.size() == 0 || ((mStyle & SCROLL) != SCROLL))
        return false;

     pBegin = mViews.begin();
     pEnd = --mViews.end();

     if (mRect.top <= (*pBegin)->getRect()->top
        && mRect.bottom >= (*pEnd)->getRect()->bottom)
        return false;//no view out of page rect
 
     if (offset > 0){
         return mRect.top > (*pBegin)->getRect()->top - 30;
     } else if (offset < 0) {
         return mRect.bottom < (*pEnd)->getRect()->bottom + 30;
     }
 
     return false;
     
 }

 void TWPageBase::resetScroll(){
     LISTVIEWS::iterator pBegin;
     LISTVIEWS::iterator pEnd; 

     if (mViews.size() == 0)
        return;

     pBegin = mViews.begin();
     pEnd = --mViews.end();

     if (mRect.top <= (*pBegin)->getRect()->top
        && mRect.bottom >= (*pEnd)->getRect()->bottom)
        return ;//no view out of page rect

     int offset = 0;
 
     if (mRect.top < (*pBegin)->getRect()->top - 10){
         offset = mRect.top - (*pBegin)->getRect()->top + 10;
     } else if (mRect.bottom > (*pEnd)->getRect()->bottom + 10) {
         offset = mRect.bottom - (*pEnd)->getRect()->bottom - 10;
     }

     for(LISTVIEWS::iterator plist = mViews.begin(); plist != mViews.end(); plist++){
        
         (*plist)->offset(0, offset);
     }  
     
 }
 
 int TWPageBase::onTouch(POINT pt, int action){

    if (last.x == 0 && last.y == 0)
        last = pt;

    if (transTouch(pt, action) == FAILED){
        LOGD("id %d transTouch failed\n ", mId);
        return FAILED;
    }

    if (PtInRect(&mRect, pt.x, pt.y)){
         
         switch (action){
           
            case TOUCH_MOVE:{
                
                if (last_action != TOUCH_MOVE)
                    break;
                if (!scroll( pt.y - last.y)){
#if SUPPORT_GESTURE
                    
                    int x_offset = pt.x - last.x;
                    int y_offset = pt.y - last.y;

                    float slope = ABS(x_offset) * 1.0f / (ABS(y_offset) + 1);

                    LOGD("TWPageBase::onTouch x_offset%d , y_offset %d slope %f\n", x_offset, y_offset, slope);

                    if (ABS(x_offset) >= GESTURE_MAX_DISTANCE 
                        && slope > 1.0f / GESTURE_SLOPE){
                        onGesture(x_offset > 0 ? RIGHT : LEFT);
                    } else if (ABS(y_offset) >= GESTURE_MAX_DISTANCE
                        && slope < GESTURE_SLOPE){
                         onGesture(y_offset > 0 ? DOWN : UP);
                    }
#endif

                }
                    
            }break;
            case TOUCH_UP:
                if (last_action == TOUCH_MOVE)
                    resetScroll();
                break;
            default:
                break;
        }

        last = pt;
        last_action = action;
       
        return SUCCESS;
    } else {
        last_action = TOUCH_UP;
        
    }
    
    return SUCCESS;

}        
int TWPageBase::transTouch(POINT pt, int action){
	
	LISTVIEWS::iterator plist; 
    int res = FAILED;
	
	if (mVisable != VISABLE) return res;	
	
	for(plist = mViews.begin(); plist != mViews.end(); plist++){
		
		if ((*plist)->onTouch(pt, action) == SUCCESS){
			res = SUCCESS;
			break;
		}	
	}	
	
	if (action == TOUCH_UP ){
		
		if ( mLastTouchView != NULL && typeid(*mLastTouchView) == typeid(TWButton))
			((TWButton*)mLastTouchView)->setState(TW_BUTTON_NORMAL);
		
		mLastTouchView = NULL;
	}	
	
	if ((*plist) != NULL){
		mLastTouchView = *plist;
	}

    if (PtInRect(&mRect, pt.x, pt.y))
        res = SUCCESS;
        
	return res;
}

int TWPageBase::onTimer(int id, int tick_count){
	
	return FAILED;
}



 int TWPageBase::onPause(){
    return SUCCESS;
 }

 int TWPageBase::onResume(){
    return SUCCESS;
 }
