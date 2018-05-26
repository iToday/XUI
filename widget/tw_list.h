#ifndef ITODAY_LIST_H
#define ITODAY_LIST_H

#include "tw_view.h"
#include <list>
#include <iterator>
using namespace std;

typedef list<TWView*> LISTVIEWS;

class DataSetObservable{
    public:
        virtual int onDataChange() = 0;
};

class BaseAdapter{
    private:
        DataSetObservable* mNotify;
    public:
    	BaseAdapter(){
    		mNotify = NULL;
		}
		
        virtual int getCount() = 0;
        virtual TWView* getView(int pos, TWView* convertView) = 0;
        virtual bool isEmpty() = 0;

        void notifyDataSetChange(){

            if (mNotify != NULL)
                mNotify->onDataChange();
        }

        void setListener(DataSetObservable* listener){
            mNotify = listener;
        }
};

class OnItemClickListener{
    public:
        virtual int onItemClick(TWView* view, void* extra) = 0;;
};

enum LIST_STYLE {
    HORIZONTAL,
    VERTICAL,
};

class TWList : public TWView, public DataSetObservable, public OnClickListener{
	
	private:	
		
		PBITMAP mBackground;

        LISTVIEWS mChilds;

        LISTVIEWS mReservedChilds;

        int mFirstVisablePos;

        BaseAdapter* mAdapter;

        LIST_STYLE mStyle;

        OnItemClickListener* mOnItemClickListener;
	
	public:
		TWList(HWND hWnd, int zorder, int id, RECT rect, LIST_STYLE style);
		TWList(HWND hWnd, int zorder, int id, RECT rect, LIST_STYLE style, const char* background);
		~TWList();
		
		void setBackground(const char* background);

        void setAdapter(BaseAdapter* adapter);
		
		virtual void onDraw(HDC dc);

        virtual int onTouch(POINT pt, int action);

        virtual void setOnItemClickListener(OnItemClickListener* listener); 

        int getFirstVisiblePosition();

        int getLastVisiblePosition();

        int getChildCount();

        void setFirstVisiblePosition(int pos);

        virtual int onDataChange();

        virtual int onClickListener(TWView* view);

    private:

        void scroll(int x_offset, int y_offset);

        TWView* getChild(int index);

        bool canScroll(int x_offset, int y_offset);

        void resetScroll();

};

#endif