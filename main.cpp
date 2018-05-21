/* 
** $Id: helloworld.c 793 2010-07-28 03:36:29Z dongjunjie $
**
** Listing 1.1
**
** helloworld.c: Sample program for MiniGUI Programming Guide
**      The first MiniGUI application.
**
** Copyright (C) 2004 ~ 2009 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <utils/log.h>

#include "iLock_resource.h"


#ifdef _LANG_ZHCN
#include "iLock_res_cn.h"
#elif defined _LANG_ZHTW
#include "iLock_res_tw.h"
#else
#include "iLock_res_en.h"
#endif


#include "ui/ui_manager.h"
#include "ui/check/check_common.h"

#define LOG_TAG "iLock"

static UIManager* g_UI;

static int HelloWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
	POINT pt;
	
    switch (message) {
		
        case MSG_CREATE:
			if (g_UI != NULL)
				g_UI->onCreate(hWnd);
            break;

        case MSG_TIMER:
			if (g_UI != NULL)
				g_UI->onTimer(wParam, lParam);
            break;
        case MSG_SET_PAGE_VISABLE:

            if (g_UI != NULL)
                g_UI->setPageVisable(wParam, lParam);

            InvalidateRect (hWnd, NULL, TRUE);
            break;
        case MSG_SEND_KEY:
            if (g_UI != NULL)
                g_UI->onKey(wParam, lParam);
            break;
        case MSG_SEND_DATA_TO_PAGE:
            if (g_UI != NULL)
                g_UI->transPageData(wParam, (MsgData*)lParam);
            break;
        case MSG_LBUTTONDBLCLK:
        case MSG_LBUTTONDOWN:            
           
			pt.x = LOSWORD(lParam);
            pt.y = HISWORD(lParam);
			if (g_UI != NULL)			
				g_UI->onTouch(pt, TOUCH_DOWN);
			InvalidateRect (hWnd, NULL, TRUE);
            break;
        case MSG_MOUSEMOVE:
			pt.x = LOSWORD(lParam);
            pt.y = HISWORD(lParam);
			if (g_UI != NULL)			
				g_UI->onTouch(pt, TOUCH_MOVE);
			InvalidateRect (hWnd, NULL, TRUE);

            break;

        case MSG_LBUTTONUP:
            			
			pt.x = LOSWORD(lParam);
            pt.y = HISWORD(lParam);
			
			if (g_UI != NULL)
				g_UI->onTouch(pt, TOUCH_UP);
			
			InvalidateRect (hWnd, NULL, TRUE);
            break;
		case MSG_ERASEBKGND:
			return 0;// must return ; don't use background fresh
        case MSG_PAINT:
            hdc = BeginPaint (hWnd);           
			if (g_UI != NULL)
				g_UI->onDraw(hdc);
            EndPaint (hWnd, hdc);
            return 0;       

        case MSG_CLOSE:
            LOGD("MSG_CLOSE \n");
			if (g_UI != NULL)
				 g_UI->onDestroy();
            DestroyMainWindow (hWnd);
            PostQuitMessage (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

	g_UI = new UIManager(NULL);

    CreateInfo.dwStyle = 
        WS_VISIBLE /*| WS_BORDER | WS_CAPTION*/;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = 0;//GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = HelloWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.bottom;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID){
		printf("MiniGUIMain hMainWnd HWND_INVALID \n");
		return -1;
	} 	

    ShowWindow(hMainWnd, SW_SHOW/*NORMAL*/);
	
	//ShowCursor(FALSE);

    while (GetMessage(&Msg, hMainWnd)) {
						
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

	if (g_UI != NULL)
		delete g_UI;
	
    MainWindowThreadCleanup (hMainWnd);

    return 0;
}
//((PMAINWIN)hWnd)->MainWindowProc
#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
