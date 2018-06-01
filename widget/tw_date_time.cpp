#include "tw_date_time.h"

#define LOG_TAG "TWDateTime"

TWDigitalList::TWDigitalList(HWND hWnd, int zorder, int id, RECT rect, int items)
    : TWView(hWnd, zorder, id, rect){

    mListener = NULL;
    last_action = TOUCH_UP;
    haveDown = false;


    
    RECT item = mRect;
    item.bottom = item.top + RECTH(mRect) / items;

    TWText* txt = new TWText(mWnd, 0, 0, item, "", DT_CENTER | DT_SINGLELINE);    
    txt->setFont(mCommFont);
    mChilds.push_back(txt);

    for (int index = 0; index < items - 1; index ++){
        
        item.top = item.bottom;
        item.bottom = item.top + RECTH(mRect) / items;

        txt = new TWText(mWnd, 0, 0, item, "", DT_CENTER | DT_SINGLELINE);   
        txt->setFont(mCommFont);
        mChilds.push_back(txt);
    }
    setRegion(0, 100, 50);
    
}

TWDigitalList::~TWDigitalList(){
    
    LISTEXTS::iterator plist; 
    for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
        
        delete (*plist)->getExtra();
        delete *plist;
    }

    mChilds.clear();

}

void TWDigitalList::setListener(OnSelectChangeListener* listener){
    mListener = listener;
}

void TWDigitalList::setRegion(int min, int max, int cur){
    mMin = min;
    mMax = max;
    mCur = cur;

    LISTEXTS::iterator plist; 

    int size = mChilds.size();

    int first = cur - size / 2 - 1;

    if (first < mMin)
        first = mMax - (mMin - first);
    
    for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
        
        first = getNext(first);
        int* extra = new int(first);
        (*plist)->setExtra(extra);

        char str[16];
        sprintf(str, "%d", first); 
        (*plist)->setText(str);
    }

    if (mListener != NULL)
        mListener->onSelectChange(this, getSelected());
}

void TWDigitalList::onDraw(HDC dc){
    
    LISTEXTS::iterator plist; 
	
	if (mVisable != VISABLE) 
        return;	
	
	for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
        if (!isOverlap((*plist)->getRect())){
            LOGD("mChilds the view is out of page rect ,don't draw it\n");
            continue;
        }
        
		(*plist)->onDraw(dc);
	}
}

void TWDigitalList::resetScroll(){
    
    LISTEXTS::iterator pBegin;
    LISTEXTS::iterator pEnd; 
    
    if (mChilds.size() == 0)
       return;
    
    pBegin = mChilds.begin();
    pEnd = --mChilds.end();
    
    if (mRect.top <= (*pBegin)->getRect()->top
       && mRect.bottom >= (*pEnd)->getRect()->bottom)
       return ;//no view out of page rect
    
    int offset = mRect.top - (*pBegin)->getRect()->top ; 

    if (offset > RECTH(*(*pBegin)->getRect()) / 2)
        offset -= RECTH(*(*pBegin)->getRect());

    scroll(offset);
}

int TWDigitalList::onTouch(POINT pt, int action){
    
    static POINT last = pt;
        
    if (PtInRect(&mRect, pt.x, pt.y) && getVisable() == VISABLE){

        switch (action){
                            
            case TOUCH_UP:
				if (last_action == TOUCH_MOVE)
                    resetScroll();      
			 case TOUCH_DOWN: {
			 
                LISTEXTS::iterator plist;                
                
                for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
                    
                    if ((*plist)->onTouch(pt, action) == SUCCESS){
                        
                        break;
                    }   
                }  

                
                haveDown = action == TOUCH_DOWN;
                
            }break;
            case TOUCH_MOVE:{
                if (last_action != TOUCH_MOVE || !haveDown)
                    break;

                LISTEXTS::iterator plist; 
                
                for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
                    
                    if ((*plist)->onTouch(pt, action) == SUCCESS){
                        
                        break;
                    }   
                }   

                scroll( pt.y - last.y);
                }break;
            default:
                break;
        }

        last = pt;
        last_action = action;
        
        return SUCCESS;
    }else {
        
        if (last_action == TOUCH_MOVE)
            resetScroll();   

        last_action = TOUCH_UP;
         haveDown = false;
    }    

    return FAILED;
}

int TWDigitalList::getSelected(){
    LISTEXTS::iterator plist = mChilds.begin(); 

    int size = mChilds.size();     

    advance(plist, int(size / 2.0f - 0.5f));  

    return *((int*)(*plist)->getExtra());
}

void TWDigitalList::setSelected(int cur){
    
    LISTEXTS::iterator plist = mChilds.begin(); 

    int size = mChilds.size();

    int first = cur - size / 2 - 1;

    if (first < mMin)
        first = mMax - (mMin - first);

    
    for(plist = mChilds.begin(); plist != mChilds.end(); plist++){

        delete (*plist)->getExtra();
        
        first = getNext(first);
        int* extra = new int(first);
        (*plist)->setExtra(extra);

        char str[16];
        sprintf(str, "%d", first); 
        (*plist)->setText(str);
    }
}

int TWDigitalList::getPrev(int cur){

    if (cur <= mMin)
        cur = mMax;
    else
        cur --;

    return cur;
}
int TWDigitalList::getNext(int cur){
    if (cur >= mMax)
        cur = mMin;
    else
        cur ++;

    return cur;

}

void TWDigitalList::scroll(int y_offset){
    
    LISTEXTS::iterator plist;     

    bool bChanged = false;
     
    for(plist = mChilds.begin(); plist != mChilds.end(); ){
        (*plist)->offset(0, y_offset);

        if (!isOverlap((*plist)->getRect())){
            
            if ((*plist)->getExtra() != NULL)
                delete (*plist)->getExtra();
            
            delete *plist;
            LISTEXTS::iterator tmp = plist ++;
            mChilds.erase(tmp);

            bChanged = true;
        } else
            plist++;
    }
 

    //check head  is have space to mRect edge

    do {
        plist = mChilds.begin();

        RECT* begin = (*plist)->getRect();
        if (begin->top > mRect.top){

            int* extra = (int*)(*plist)->getExtra();
            int* cur = new int(getPrev(*extra));

            char str[16];

            sprintf(str, "%d", *cur);  
            
            TWText* txt = new TWText(mWnd, 0, 0, *begin, str, DT_CENTER | DT_SINGLELINE);
            txt->moveTo(begin->left, begin->top - RECTH(*begin));
            txt->setExtra(cur);
            txt->setFont(mCommFont);
            mChilds.push_front(txt);

            bChanged = true;
            
        } else
            break;
        
    } while(true);

   //check tail is have space to mRect edge
    do {
        plist = --mChilds.end();
        RECT* end = (*plist)->getRect();
        if ((*plist)->getRect()->bottom < mRect.bottom){

            int* extra = (int*)(*plist)->getExtra();

            int* cur = new int(getNext(*extra));

            char str[16];

            sprintf(str, "%d", *cur);  
            
            TWText* txt = new TWText(mWnd, 0, 0, *end, str, DT_CENTER | DT_SINGLELINE);
            txt->moveTo(end->left, end->bottom);
            txt->setExtra(cur);
            txt->setFont(mCommFont);

            mChilds.push_back(txt);

            bChanged = true;
            
        } else
            break;
        
    } while(true);

    if (bChanged && mListener != NULL){
        mListener->onSelectChange(this, getSelected());

    }

}


TWDateTime::TWDateTime(HWND hWnd,int zorder, int id, RECT rect)
	: TWView(hWnd, zorder, id, rect){

	mBackground = NULL;
    mListener = NULL;

    initSub();
    
}

TWDateTime::TWDateTime(HWND hWnd, int zorder, int id, RECT rect, const char* background)
    : TWView(hWnd, zorder, id, rect){
    mListener = NULL;
    initSub();
    
	if (background != NULL){
		mBackground = new BITMAP();
		mBkgPath = string(background);
		
		LoadBitmap (HDC_SCREEN, mBackground, background);
	}
	
}

TWDateTime::TWDateTime(HWND hWnd, int zorder, int id, RECT rect, PBITMAP background)
    : TWView(hWnd, zorder, id, rect){
    
    mListener = NULL;
    initSub();
    
    mBkgPath.clear();

    mBackground = background;
}

TWDateTime::~TWDateTime(){
#ifndef RESOURCE_MANAGER_SUPPORT
	if (mBackground != NULL && !mBkgPath.empty()){
		UnloadBitmap(mBackground);
		delete mBackground;
	}
#endif
}

void TWDateTime::initSub(){
    RECT tmp = mRect;

    mYear = 2017;
    mMonth = 11;
    mDay = 23;
    mHour = 12;
    mMin = 0;

    tmp.right = tmp.left + RECTW(mRect) / 3;

    TWDigitalList *list = new TWDigitalList(mWnd, 0, ID_YEAR,tmp, 3);
    list->setListener(this);
    list->setRegion(1990, 2050, mYear);
    mChilds.push_back(list);

    tmp.left = tmp.right;
    tmp.right = tmp.left + RECTW(mRect) / 3;

    list = new TWDigitalList(mWnd, 0, ID_MONTH,tmp, 3);
    list->setListener(this);
    list->setRegion(1, 12, mMonth);
    mChilds.push_back(list);

    tmp.left = tmp.right;
    tmp.right = tmp.left + RECTW(mRect) / 3;

    list = new TWDigitalList(mWnd, 0, ID_DAY,tmp, 3);
    list->setListener(this);
    list->setRegion(1, 30, mDay);
    mChilds.push_back(list);

    tmp = mRect;
    tmp.right = tmp.left + RECTW(mRect) / 2;
    list = new TWDigitalList(mWnd, 0, ID_HOUR,tmp, 3);
    list->setListener(this);
    list->setRegion(0, 23, mHour);
    list->setVisable(INVISABLE);
    mChilds.push_back(list);

    tmp.left = tmp.right;
    tmp.right = tmp.left + RECTW(mRect) / 2;

    list = new TWDigitalList(mWnd, 0, ID_MIN,tmp, 3);
    list->setListener(this);
    list->setRegion(0, 59, mMin);
    list->setVisable(INVISABLE);
    mChilds.push_back(list);
}

void TWDateTime::setListener(OnDateTimeChangeListener* listener){
    mListener = listener;
}

void TWDateTime::onDraw(HDC dc){
	
	LISTVIEWS::iterator plist; 
	
	if (mVisable != VISABLE) 
        return;
	
	if (mBackground != NULL)
		FillBoxWithBitmap (dc, mRect.left, mRect.top, RECTW(mRect), RECTH(mRect), mBackground);
	
	for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
        if (!isOverlap((*plist)->getRect())){
            LOGW("mFirst the view is out of page rect ,don't draw it\n");
            continue;
        }
        
		(*plist)->onDraw(dc);
	}
	
}

void TWDateTime::onSelectChange(TWView* view, int cur){

    LOGD("onSelectChange id %d, cur %d [%04d-%02d-%02d %02d:%02d]\n", view->getId(), cur, mYear, mMonth, mDay, mHour, mMin);
    switch(view->getId()){
    case ID_YEAR:
        mYear = cur;
        break;
    case ID_MONTH:
        mMonth = cur;
        break;
    case ID_DAY:
        mDay = cur;
        break;
    case ID_HOUR:
        mHour = cur;
        break;
    case ID_MIN:
        mMin = cur;
        break;
    default:
        break;
    }

    if (mListener != NULL)
        mListener->onTimeChange(mYear, mMonth, mDay, mHour, mMin);
}

int TWDateTime::onTouch(POINT pt, int action){
    
   
	LISTVIEWS::iterator plist; 
	
	if (mVisable != VISABLE) return FAILED;	


	
	for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
		
		if ((*plist)->onTouch(pt, action) == SUCCESS){			
			//break;
		}	
	}	

    if (!PtInRect(&mRect, pt.x, pt.y))
        return FAILED;
	
	return SUCCESS;
}

void TWDateTime::setMode(DATE_TIME_DIALOG_MODE mode){

    if (mMode == mode) return;

    LISTVIEWS::iterator plist; 

    mMode = mode;
    
    if (mMode == DATE){
        
        for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
            if ((*plist)->getId() <= ID_DAY)
                (*plist)->setVisable(VISABLE);
            else
                (*plist)->setVisable(INVISABLE);
        }
    } else if (mMode == TIME){
    
         for(plist = mChilds.begin(); plist != mChilds.end(); plist++){
            if ((*plist)->getId() > ID_DAY)
                (*plist)->setVisable(VISABLE);
            else
                (*plist)->setVisable(INVISABLE);
        }
    } 
}

void TWDateTime::setTime(int year, int month, int day,int hour, int min){

    LISTVIEWS::iterator plist; 

    mYear = year;
    mMonth = month;
    mDay = day;
    mHour = hour;
    mMin = min;

    for(plist = mChilds.begin(); plist != mChilds.end(); plist++){

        TWDigitalList *list = (TWDigitalList*)*plist;

        switch(list->getId()){
            case ID_YEAR:
                list->setSelected(year);
                break;
            case ID_MONTH:
                list->setSelected(month);
                break;
            case ID_DAY:
                list->setSelected(day);
                break;
            case ID_HOUR:
                list->setSelected(hour);
                break;
            case ID_MIN:
                list->setSelected(min);
                break;
            default:
                break;
        }

        if (mListener != NULL)
            mListener->onTimeChange(year, month, day, hour, min);
        
    }    
   
}

void TWDateTime::setBackground(const char* background){
#ifdef RESOURCE_MANAGER_SUPPORT
    mBackground = ResourceManager::loadImg(background);
#else
	if (mBackground != NULL && !mBkgPath.empty()){
		UnloadBitmap(mBackground);
		delete mBackground;
        mBkgPath.clear();
	}
	
	if (background != NULL){
        mBkgPath = string(background);
		mBackground = new BITMAP();
		
		LoadBitmap (HDC_SCREEN, mBackground, background);
	}
#endif
}

void TWDateTime::setBackground(PBITMAP background){
	mBackground = background;
}

int TWDateTime::getMaxDay(int month){

    switch (month){
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 2:
            return 28;
        default:
            return 30;
    }
}

int TWDateTime::getNextYear(int year){
    if (year == MAX_YEAR)
        return MIN_YEAR;

    year ++;

    return year;
}

int TWDateTime::getPrevYear(int year){
    if (year == MIN_YEAR)
        return MAX_YEAR;

    year --;

    return year;
}

int TWDateTime::getYear(){
    return mYear;
}

int TWDateTime::getNextMonth(int month){
    
    if (month == 12)
        month = 1;
    else
        month ++;

    return month;
}

int TWDateTime::getPrevMonth(int month){

    if (month == 1)
        month = 12;
    else
        month --;

    return month;

}

int TWDateTime::getMonth(){
    return mMonth;
}

int TWDateTime::getNextDay(int month, int day){

    if (getMaxDay(month) == day)
        day = 1;
    else 
        day ++;

    return day;
}

int TWDateTime::getPrevDay(int month, int day){

    if (day == 1)
        day = getMaxDay(month);

    day --;

    return day;
}

int TWDateTime::getDay(){
    return mDay;
}

int TWDateTime::getNextHour(int hour){

    if (hour == 23)
        hour = 0;
    else
        hour ++;

    return hour;
}

int TWDateTime::getPrevHour(int hour){

    if (hour == 0)
        hour = 23;
    else
        hour --;

    return hour;
    
}

int TWDateTime::getHour(){
    return mHour;
}

int TWDateTime::getNextMin(int min){

    if (min == 59)
        min = 0;
    else
        min ++;

    return min;
}

int TWDateTime::getPrevMin(int min){

    if (min == 1)
        min = 59;
    else
        min --;

    return min;
}

int TWDateTime::getMin(){
    return mMin;
}


