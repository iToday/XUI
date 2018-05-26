#include "tw_view.h"

TWView::TWView(HWND hWnd){
	
	mClickListener = NULL;
    mDataExtra = NULL;
	mId = INVALID_ID;
	mZOrder = 0;
    mDown = false;
    mVisable = VISABLE;
	memset(&mRect, 0, sizeof(RECT));
	mWnd = hWnd;
}

TWView::TWView(HWND hWnd, int zorder, int id, RECT rect){
	mClickListener = NULL;
    mDataExtra = NULL;
	mId = id;
	mZOrder = zorder;
	memcpy(&mRect, &rect, sizeof(RECT));
    mVisable = VISABLE;
	mWnd = hWnd;
    mDown = false;
}

TWView::~TWView(){

    printf("\n TWView::~TWView() id %d\n ", mId);
	mClickListener = NULL;
}

int TWView::getId(){
	return mId;
}

void TWView::setName(const char* name){
	
	if (name != NULL)
		mName = name;
}

bool TWView::is(const char* name){
	
	if (name != NULL)
		return mName == name;
		
	return false;
}

int TWView::setOnClickListener(OnClickListener* listener){
	
	mClickListener = listener;
	
	return SUCCESS;
}

bool TWView::isIn(POINT pt){
	
	return pt.x > mRect.left && pt.x < mRect.right 
		&& pt.y > mRect.top && pt.y < mRect.bottom;
}

int TWView::offset(int x, int y){
    OffsetRect(&mRect, x, y);
    return 0;
}

int TWView::compareZOrder(TWView* view){
	
	if (view == NULL)
		return 1;
	
	return this->mZOrder - view->mZOrder;
}

RECT* TWView::getRect(){
    return &mRect;
}

void TWView::setRect(RECT rect){
    memcpy(&mRect, &rect, sizeof(RECT));
}

int TWView::onTouch(POINT pt, int action){
    
	if (isIn(pt) && mVisable == VISABLE){
		
		if (action == TOUCH_UP){
			
			if (mClickListener != NULL && mDown){
                mDown = false;
				return mClickListener->onClickListener(this);
			}
            
		} else if (action == TOUCH_DOWN){
            mDown = true;
            mLast = pt;

            return SUCCESS;
		} else if (action == TOUCH_MOVE){

            if (mDown == false)
                return SUCCESS;

            if (pt.x - mLast.x > MAX_CLICK_DISTANCE || pt.x - mLast.x < -MAX_CLICK_DISTANCE
                || pt.y - mLast.y > MAX_CLICK_DISTANCE || pt.y - mLast.y < -MAX_CLICK_DISTANCE)
                mDown = false;

            return SUCCESS;
		}       
        
		
	}

    mDown = false;
	
	return FAILED;
}

 void TWView::setExtra(void* data){
    mDataExtra = data;
 }

 void* TWView::getExtra(){
    return mDataExtra;
 }

 void TWView::moveTo(int x, int y){

    OffsetRect(&mRect, x - mRect.left, y - mRect.top);  
 }

 int TWView::getVisable(){
	return mVisable;
 }

void TWView::setVisable(int visable){
    mVisable = visable;
}

bool TWView::isOverlap(RECT* rect){

    if (rect == NULL) return false;
    
    if (rect->top > mRect.bottom
        || rect->bottom < mRect.top
        || rect->left > mRect.right
        || rect->right < mRect.left){
        
        return false;
    }

    return true;
}