#ifndef ITODAY_DATE_TIME_H
#define ITODAY_DATE_TIME_H
#include <list>
#include "tw_view.h"
#include "tw_text.h"

class OnDateTimeChangeListener{
    public:
       
        virtual void onTimeChange(int year, int month, int day, int hour, int min) = 0;
};

typedef list<TWText*> LISTEXTS;

#define MAX_YEAR    2057
#define MIN_YEAR    2000

#define ID_YEAR     1
#define ID_MONTH    2
#define ID_DAY      3
#define ID_HOUR     4
#define ID_MIN      5

enum DATE_TIME_DIALOG_MODE{
    DATE,
    TIME,
    DATE_TIME,
};

class OnSelectChangeListener{
    public:
        virtual void onSelectChange(TWView* view, int cur) = 0;
};

class TWDigitalList : public TWView {

private:

    LISTEXTS mChilds;

    int mMin;
    int mMax;
    int mCur;

    OnSelectChangeListener* mListener;

    int last_action;
    bool haveDown;

    PLOGFONT mCommFont;
    
public:
    TWDigitalList(HWND hWnd, int zorder, int id, RECT rect, int items);
    ~TWDigitalList();

    void setListener(OnSelectChangeListener* listener);

    void setRegion(int min, int max, int cur);

    int getPrev(int cur);
    int getNext(int cur);

    int getSelected();
    void setSelected(int cur);

    virtual void onDraw(HDC dc);
    virtual int onTouch(POINT pt, int action);
    

private:
    void scroll(int y_offset);
    void resetScroll();
    
};

class TWDateTime : public TWView, public OnSelectChangeListener{
	
	protected:	
		
	
        string mBkgPath;
		PBITMAP mBackground;

        OnDateTimeChangeListener* mListener;

	    LISTVIEWS mChilds;

        int mYear;
        int mDay;
        int mMonth;
        int mMin;
        int mHour;

        DATE_TIME_DIALOG_MODE mMode;

	public:
		TWDateTime(HWND hWnd, int zorder, int id, RECT rect);
		TWDateTime(HWND hWnd, int zorder, int id, RECT rect, const char* background);
        TWDateTime(HWND hWnd, int zorder, int id, RECT rect, PBITMAP background);
		~TWDateTime();

        void setTime(int year, int month, int day,int hour, int min);
        void setMode(DATE_TIME_DIALOG_MODE mode);

        void setListener(OnDateTimeChangeListener* listener);

        
		void setBackground(const char* background);
		void setBackground(PBITMAP background);

		virtual void onDraw(HDC dc);
        virtual int onTouch(POINT pt, int action);
        virtual void onSelectChange(TWView* view, int cur);

     private:

        void initSub();

     public:

        int getMaxDay(int month);

        int getNextYear(int year);
        int getPrevYear(int year);
        int getYear();

        int getNextMonth(int month);
        int getPrevMonth(int month);
        int getMonth();

        int getNextDay(int month, int day);
        int getPrevDay(int month, int day);
        int getDay();

        int getNextHour(int hour);
        int getPrevHour(int hour);
        int getHour();

        int getNextMin(int min);
        int getPrevMin(int min);
        int getMin();

};

#endif