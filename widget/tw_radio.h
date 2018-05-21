#ifndef ITODAY_RADIO_BUTTON_H
#define ITODAY_RADIO_BUTTON_H

#include "tw_button.h"

enum TW_RADIO_BUTTON_STATUS {
	TW_RADIO_UNCHECK, /**/
	TW_RADIO_CHECKED
	
};


class TWRadioButton : public TWButton, public OnClickListener{

		
	public:
		TWRadioButton(HWND hWnd, int zorder, int id, RECT rect, const string* path, int size);
        TWRadioButton(HWND hWnd, int zorder, int id, RECT rect, PBITMAP* imges, int size);		
		
		~TWRadioButton();		


        void setChecked(bool check);		
        bool isChecked();

        virtual int onTouch(POINT pt, int action);
        virtual int onClickListener(TWView* view);
};

#endif