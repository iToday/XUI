#ifndef ITODAY_XML_LOADER_H
#define ITODAY_XML_LOADER_H

#include   <map>
#include   <string>
#include   <lib/mxml/mxml.h>

#include "widget/tw_view.h"
#include "widget/tw_button.h"
#include "widget/tw_image.h"
#include "widget/tw_list.h"
#include "widget/tw_text.h"
#include "widget/tw_radio.h"
#include "widget/tw_video.h"
#include "widget/tw_progress.h"
#include "widget/tw_date_time.h"

using namespace std;

#define ATTR_NAME    "name"
#define ATTR_ID      "id"
#define ATTR_RECT    "rect"
#define ATTR_TEXT    "text"
#define ATTR_BKG     "background"
#define ATTR_VISABLE "visable"
#define ATTR_FONT    "font"
#define ATTR_TEXT_SIZE "size"
#define ATTR_COLOR    "color"
#define ATTR_INDENT   "indent"
#define ATTR_FORMAT   "style"
#define ATTR_TEXT_OFFSET   "textOffset"
#define ATTR_THUMB		"thumb"
#define ATTR_THUMB_RECT 	"thumbRect"
#define ATTR_MAX			"max"
#define ATTR_STATE			"state"
#define ATTR_DRAWABLE		"drawable"

//button state
#define SELECTOR_NORMAL "normal"
#define SELECTOR_PRESSED "pressed"
#define SELECTOR_FOCUS  "focus"
#define SELECTOR_DISABLE "disable"
//radio button state
#define SELECTOR_UNCHECK "uncheck"
#define SELECTOR_CHECKED "checked"
// progress bar background
#define SELECTOR_LIGHT   "light"
#define SELECTOR_DARK	 "dark"

#define CLS_PAGE    "Page"
#define CLS_BUTTON  "Button"
#define CLS_TEXT    "Text"
#define CLS_IMAGE   "Image"
#define CLS_RADIO   "Radio"
#define CLS_PROGRESS "Progress"
#define CLS_LIST     "List"
#define CLS_VIDEO   "Video"
#define CLS_DATE_TIME    "DateTime"

int u2g(const char *inbuf,int inlen,char *outbuf,int outlen);
int g2u(const char *inbuf,size_t inlen,char *outbuf,size_t outlen);

class XmlLoader {

    static mxml_node_t* mLanguageTree;
    
public:

    static void loadLanguage(const char* file);
    
    static string loadString(const char* textId);
    
    static mxml_node_t* load(const char* file);

    static const char* getStrAttr(mxml_node_t* node, const char* name);
    
    static int getIntAttr(mxml_node_t* node, const char* name);
    
    static RECT getRectAttr(mxml_node_t* node);
    
    static RECT getRectAttr(mxml_node_t* node, const char* name);

    static const char* getDrawableAttr(mxml_node_t* node);
    
    static int getDrawablesAttr(mxml_node_t* node, const char* name, string* res, int size);
    static int getDrawablesAttr(mxml_node_t* node, string* res, int size);
    
    static unsigned int getColorAttr(mxml_node_t* node);
    
    static const char* getTextAttr(mxml_node_t* node);

    static unsigned int getVisableAttr(mxml_node_t* node);

    static unsigned int getStyleAttr(mxml_node_t* node);

    static POINT getTextOffset(mxml_node_t* node);

    static TWButton* createButton(mxml_node_t* subNode, HWND hWnd);
    static TWText* createTextView(mxml_node_t* subNode, HWND hWnd);
    static TWImage* createImageView(mxml_node_t* subNode, HWND hWnd);
    static TWList* createListView(mxml_node_t* subNode, HWND hWnd);
    static TWProgress* createProgressBar(mxml_node_t* subNode, HWND hWnd);
    static TWDateTime* createDateTime(mxml_node_t* subNode, HWND hWnd);
    static TWVideoView* createVideoView(mxml_node_t* subNode, HWND hWnd);
    static TWRadioButton* createRadioButton(mxml_node_t* subNode, HWND hWnd);


    static LISTVIEWS createWidgets(mxml_node_t* node, HWND hWnd);
    
    static LISTVIEWS loadXmlWidgets(const char* xml, HWND hWnd);
    
};

#endif