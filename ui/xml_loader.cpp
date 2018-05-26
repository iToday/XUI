
#include "xml_loader.h"
#include "iconv.h"
#include <utils/log.h>
#include <unistd.h>
#include <stdio.h>
#include "page_base.h"
#include "resource_manager.h"

#define LOG_TAG "XmlLoader"

#define COVERT_SIZE 1024

mxml_node_t* XmlLoader::mLanguageTree = NULL;

int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen,char *outbuf,size_t outlen) 
{ 
    iconv_t cd; 
    int rc;

    char in[COVERT_SIZE] = {0};
    
    char **pout = &outbuf;    

    if (inlen < COVERT_SIZE)
        strcpy(in, inbuf);

    char **pin = (char **)&in; 
    
    cd = iconv_open(to_charset, from_charset); 
    
    if (cd == (iconv_t)(-1)){
        printf("iconv_open failed %d\n", errno);
        return -1; 
    }
    
    memset(outbuf,0,outlen); 

    
    if (iconv(cd, pin, &inlen, pout, &outlen)==-1){
        printf("iconv failed %d\n", errno);
        return -1; 
    }
    
    iconv_close(cd); 
    return 0; 
}

int u2g(const char *inbuf,int inlen,char *outbuf,int outlen) 
{ 
    return code_convert("utf-8","gbk",inbuf,inlen,outbuf,outlen); 
}

int g2u(const char *inbuf,size_t inlen,char *outbuf,size_t outlen)
{
    return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);
}



void XmlLoader::loadLanguage(const char* file){

    if (mLanguageTree != NULL)
        mxmlDelete(mLanguageTree);

    mLanguageTree = load(file);
}

string XmlLoader::loadString(const char* textId){

    string dest;
	
	if (*textId == '@' && mLanguageTree != NULL){
        
        mxml_node_t* strNode =  mxmlFindElement(mLanguageTree, mLanguageTree, "string", "name", textId + 1, MXML_DESCEND);

        if (strNode != NULL){

            const char* utf8Str = mxmlGetText(strNode, 0);
            char gbkStr[512] = {0};
            
            u2g(utf8Str, strlen(utf8Str), gbkStr, sizeof(gbkStr));

            dest = gbkStr;
		}
     }

    return dest;
}

mxml_node_t* XmlLoader::load(const char* file){

    FILE *fp;

    mxml_node_t *tree = NULL;

    fp = fopen(file, "r");

    if (fp != NULL){
        
        tree = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);

        fclose(fp);

    }
    
    return tree;
}

const char* XmlLoader::getStrAttr(mxml_node_t* node, const char* name){
    return mxmlElementGetAttr(node, name);
}

int XmlLoader::getIntAttr(mxml_node_t* node, const char* name){
    
    int value = 0;
    const char* str = mxmlElementGetAttr(node, name);

    if (str != NULL)
        sscanf(str, "%d", &value);

    return value;

}

RECT XmlLoader::getRectAttr(mxml_node_t* node, const char* name){
	
	 RECT rect = {0,0,0,0};
    const char* rect_str = mxmlElementGetAttr(node, name);    

    if (rect_str != NULL)
        sscanf(rect_str, "%d,%d,%d,%d", &rect.left, &rect.top, &rect.right, &rect.bottom);

    rect.bottom += rect.top;
    rect.right+= rect.left;
        
    return rect;
}

RECT XmlLoader::getRectAttr(mxml_node_t* node){
            
    return getRectAttr(node, ATTR_RECT);
}

const char* XmlLoader::getDrawableAttr(mxml_node_t* node){
    
  return mxmlElementGetAttr(node, ATTR_BKG);
}

int XmlLoader::getDrawablesAttr(mxml_node_t* node, const char* name, string* res, int size){
	
	int drawable_num = 0;
	const char* str = mxmlElementGetAttr(node, name);

    if (str == NULL)
        return drawable_num;

    if (*str == '@'){

        mxml_node_t* drawable = load(str + 1);

        if (drawable == NULL)
            return drawable_num;
            
        mxml_node_t* selector = mxmlFindElement(drawable, drawable, "selector" , NULL, NULL, MXML_DESCEND);
 	
    	mxml_node_t *subNode = NULL;
        
        for (subNode = mxmlWalkNext(selector, selector, MXML_DESCEND);
            subNode != NULL;
            subNode = mxmlWalkNext(subNode, selector, MXML_DESCEND)){

            if (subNode != NULL){

                const char* state = mxmlElementGetAttr(subNode, ATTR_STATE);
                
                if (state != NULL){
                	
                	 const char* drawable = mxmlElementGetAttr(subNode, ATTR_DRAWABLE);
                	 
                	if (strcmp(SELECTOR_NORMAL, state) == 0){                                       
		  
						  if (drawable != NULL)
						  		res[TW_BUTTON_NORMAL] = drawable;
								  		            
                    } else if (strcmp(SELECTOR_PRESSED, state) == 0){                      	
						  
						  if (drawable != NULL)
						  		res[TW_BUTTON_PRESSED] = drawable;
						  		
                    } else if (strcmp(SELECTOR_FOCUS, state) == 0){
						  
						  if (drawable != NULL)
						  		res[TW_BUTTON_FOCUS] = drawable;
						  		
                    } else if (strcmp(SELECTOR_DISABLE, state) == 0){
						  
						  if (drawable != NULL)
						  		res[TW_BUTTON_DISABLE] = drawable;
						  		
                    } else if (strcmp(SELECTOR_UNCHECK, state) == 0){
						  
						  if (drawable != NULL)
						  		res[TW_RADIO_UNCHECK] = drawable;
                    	
					} else if (strcmp(SELECTOR_CHECKED, state) == 0){
						  
						  if (drawable != NULL)
						  		res[TW_RADIO_CHECKED] = drawable;
						  		
					} else if (strcmp(SELECTOR_LIGHT, state) == 0){
						
						if (drawable != NULL)
						  		res[BKG_LIGHT] = drawable;
						
					} else if (strcmp(SELECTOR_DARK, state) == 0){
						
						if (drawable != NULL)
						  		res[BKG_DARK] = drawable;
					} else{
						
						if (drawable_num < size)
							res[drawable_num] = drawable;												
					}
					
					if (drawable_num < size)
						drawable_num ++;	
				} 
            }                    
        }
        	
        mxmlDelete(drawable);

    } else {

        res[drawable_num] = str;
        
        if (drawable_num < size)
			drawable_num ++;	
    }

    return drawable_num;
}

int XmlLoader::getDrawablesAttr(mxml_node_t* node, string* res, int size){

	return getDrawablesAttr(node, ATTR_BKG, res, size);        
}

unsigned int XmlLoader::getColorAttr(mxml_node_t* node){
    
    unsigned int color = 0;
    const char* str = mxmlElementGetAttr(node, ATTR_COLOR);

    if (str != NULL)
        sscanf(str, "%x", &color);

    return color;
}


const char* XmlLoader::getTextAttr(mxml_node_t* node){
    
    const char* str = mxmlElementGetAttr(node, ATTR_TEXT);

    if (str == NULL)
        return NULL;

    if (*str == '@' && mLanguageTree != NULL){
        
        mxml_node_t* strNode =  mxmlFindElement(mLanguageTree, mLanguageTree, "string", "name", str + 1, MXML_DESCEND);

        if (strNode != NULL)
            return mxmlGetText(strNode, 0);

    }

    return str;
}

unsigned int XmlLoader::getVisableAttr(mxml_node_t* node){
    
    unsigned int visable = VISABLE;
    const char* str = mxmlElementGetAttr(node, ATTR_VISABLE);

    if (str == NULL)
        return visable;

    
    if (strcmp(str, "INVISABLE") == 0)
        visable = INVISABLE;
    else if (strcmp(str, "GONE") == 0)
        visable = GONE;
    else if (strcmp(str, "VISABLE") == 0)
        visable = VISABLE;
    else
        visable = INVISABLE;

    return visable;
    
}

unsigned int XmlLoader::getStyleAttr(mxml_node_t* node){
    
    unsigned int style = 0;
    const char* str = mxmlElementGetAttr(node, ATTR_FORMAT);

    if (str == NULL)
        return style;

    if (strstr(str, "DT_CENTER"))
        style |= DT_CENTER;

    if (strstr(str, "DT_WORDBREAK"))
        style |= DT_WORDBREAK;
        
    if (strstr(str, "DT_TOP"))
        style |= DT_TOP;    
        
	if (strstr(str, "DT_LEFT"))
        style |= DT_LEFT;
        
    if (strstr(str, "DT_RIGHT"))
        style |= DT_RIGHT;
        
    if (strstr(str, "DT_VCENTER"))
        style |= DT_VCENTER;

	if (strstr(str, "DT_BOTTOM"))
        style |= DT_BOTTOM;    
        
	if (strstr(str, "DT_SINGLELINE"))
        style |= DT_SINGLELINE;
        
//    if (strstr(str, "DT_CHARBREAK"))
//        style |= DT_CHARBREAK;
        
    if (strstr(str, "DT_EXPANDTABS"))
        style |= DT_EXPANDTABS;
        
    if (strstr(str, "DT_TABSTOP"))
        style |= DT_TABSTOP;    
        
	if (strstr(str, "DT_CALCRECT"))
        style |= DT_CALCRECT;

    if (strstr(str, "NONE_S"))
        style |= NONE_S;

    if (strstr(str, "SCROLL"))
        style |= SCROLL;
        
    if (strstr(str, "VERTICAL"))
    	style |= VERTICAL;
    	
     if (strstr(str, "HORIZONTAL"))
    	style |= HORIZONTAL;   

    return style;
}

POINT XmlLoader::getTextOffset(mxml_node_t* node){
    
    POINT offset = {0,0};
    
    const char* str = mxmlElementGetAttr(node, ATTR_TEXT_OFFSET);    

    if (str != NULL)
        sscanf(str, "%d,%d", &offset.x, &offset.y);

    return offset;
}

TWButton* XmlLoader::createButton(mxml_node_t* subNode, HWND hWnd){
    
    RECT rect = XmlLoader::getRectAttr(subNode);
    const char* text = XmlLoader::getTextAttr(subNode);
    const char* name = XmlLoader::getStrAttr(subNode, ATTR_NAME);
    int id = XmlLoader::getIntAttr(subNode, ATTR_ID);
    unsigned int style = XmlLoader::getStyleAttr(subNode);
    unsigned int color = XmlLoader::getColorAttr(subNode);
    unsigned int visable = XmlLoader::getVisableAttr(subNode);
    
    int size = XmlLoader::getIntAttr(subNode, ATTR_TEXT_SIZE);
    const char* font = XmlLoader::getStrAttr(subNode, ATTR_FONT);

    POINT offset = XmlLoader::getTextOffset(subNode);

    string res[TW_BUTTON_STATUS_MAX]; 

    int imgSize = getDrawablesAttr(subNode, res, TW_BUTTON_STATUS_MAX);
    
    TWButton* button = new TWButton(hWnd, 0, id, rect, res, TW_BUTTON_STATUS_MAX, text, style);
    
    button->setName(name);
    
    if (font != NULL)
    	button->setFont(ResourceManager::loadFont(font, size));
    	
    button->setText(text);
    
    button->setTextOffset(offset.x, offset.y);
    
    button->setTextColor(color);
    button->setVisable(visable);

    return button;

}

TWText* XmlLoader::createTextView(mxml_node_t* subNode, HWND hWnd){
    
    RECT rect = getRectAttr(subNode);
    const char* text = getTextAttr(subNode);
    const char* name = getStrAttr(subNode, ATTR_NAME);
    int id = getIntAttr(subNode, ATTR_ID);
    unsigned int style = getStyleAttr(subNode);
    unsigned int color = getColorAttr(subNode);
    unsigned int visable = getVisableAttr(subNode);

    int size = getIntAttr(subNode, ATTR_TEXT_SIZE);
    const char* font = getStrAttr(subNode, ATTR_FONT);
    POINT offset = getTextOffset(subNode);    
    
    TWText* widget = new TWText(hWnd, 0, id, rect, text, style);
    widget->setName(name);
    widget->setTextColor(color);
    widget->setTextOffset(offset.x, offset.y);
    
    if (font != NULL)
    	widget->setFont(ResourceManager::loadFont(font, size));
    widget->setVisable(visable);

    return widget;

}

TWImage* XmlLoader::createImageView(mxml_node_t* subNode, HWND hWnd){
	
	RECT rect = getRectAttr(subNode);
	int id = getIntAttr(subNode, ATTR_ID);
	const char* name = XmlLoader::getStrAttr(subNode, ATTR_NAME);
	
	string res[10]; 
	int imgSize = getDrawablesAttr(subNode, res, 10);
	
	TWImage* img = new TWImage(hWnd, 0, 0, rect, res, imgSize);
	img->setName(name);
	
    return img;
}

TWList* XmlLoader::createListView(mxml_node_t* subNode, HWND hWnd){
	
	RECT rect = getRectAttr(subNode);
	int id = getIntAttr(subNode, ATTR_ID);
	unsigned int style = getStyleAttr(subNode);
	const char* name = XmlLoader::getStrAttr(subNode, ATTR_NAME);
	
	const char* background = getDrawableAttr(subNode);
	
	TWList* listView = new TWList(hWnd, 0, id, rect, LIST_STYLE(style), background);
	listView->setName(name);
    return listView;
}

TWProgress* XmlLoader::createProgressBar(mxml_node_t* subNode, HWND hWnd){
	
	RECT rect = getRectAttr(subNode);
	int id = getIntAttr(subNode, ATTR_ID);
	int max = getIntAttr(subNode, ATTR_MAX);
	const char* thumb = mxmlElementGetAttr(subNode, ATTR_THUMB);
	RECT thumbRect = getRectAttr(subNode, ATTR_THUMB_RECT);
	const char* name = XmlLoader::getStrAttr(subNode, ATTR_NAME);
	
	string res[PROGRESS_BACKGROUND_SIZE]; 

    int imgSize = getDrawablesAttr(subNode, res, PROGRESS_BACKGROUND_SIZE);
	
	TWProgress* progress = new TWProgress(hWnd, 0 ,id, rect);
	progress->setName(name);
    progress->setImages(res[BKG_LIGHT], res[BKG_DARK]);
    progress->setThumb(thumb, thumbRect);
    progress->setMax(max);
    
    return progress;
}

TWDateTime* XmlLoader::createDateTime(mxml_node_t* subNode, HWND hWnd){

	RECT rect = XmlLoader::getRectAttr(subNode);
    const char* name = XmlLoader::getStrAttr(subNode, ATTR_NAME);
    const char* background = getDrawableAttr(subNode);
    TWDateTime* dateTime = new TWDateTime(hWnd, 0,0, rect, background);
    
    dateTime->setName(name);
    return dateTime;
    
}


TWVideoView* XmlLoader::createVideoView(mxml_node_t* subNode, HWND hWnd){
    
    RECT rect = XmlLoader::getRectAttr(subNode);
    const char* name = XmlLoader::getStrAttr(subNode, ATTR_NAME);
    int id = XmlLoader::getIntAttr(subNode, ATTR_ID);
    unsigned int style = XmlLoader::getStyleAttr(subNode);

    TWVideoView* video = new TWVideoView(hWnd, 0, id, rect);
	video->setName(name);
    return video;
}

TWRadioButton* XmlLoader::createRadioButton(mxml_node_t* subNode, HWND hWnd){
    
	RECT rect = getRectAttr(subNode);
    const char* text = getTextAttr(subNode);
    const char* name = getStrAttr(subNode, ATTR_NAME);
    int id = getIntAttr(subNode, ATTR_ID);
    unsigned int style = getStyleAttr(subNode);
    unsigned int color = getColorAttr(subNode);
    unsigned int visable = getVisableAttr(subNode);
    
    int size = getIntAttr(subNode, ATTR_TEXT_SIZE);
    const char* font = getStrAttr(subNode, ATTR_FONT);

    POINT offset = getTextOffset(subNode);

    string res[TW_RADIO_STATUS_MAX]; 

    int imgSize = getDrawablesAttr(subNode, res, TW_RADIO_STATUS_MAX);
    
    TWRadioButton* button = new TWRadioButton(hWnd, 0, id, rect, res, TW_RADIO_STATUS_MAX);
    button->setName(name);
    
    if (font != NULL)
    	button->setFont(ResourceManager::loadFont(font, size));
    	
    button->setText(text);
    
    button->setTextOffset(offset.x, offset.y);
    
    button->setTextColor(color);
    button->setVisable(visable);
    return button;
}

LISTVIEWS XmlLoader::createWidgets(mxml_node_t* node, HWND hWnd){
    
    mxml_node_t *subNode = NULL;

    LISTVIEWS views;
                
    for (subNode = mxmlWalkNext(node, node, MXML_DESCEND);
        subNode != NULL;
        subNode = mxmlWalkNext(subNode, node, MXML_DESCEND)){

        if (subNode != NULL) {        	
            const char* name = mxmlGetElement(subNode);

            if (name != NULL){

                TWView* view = NULL;
                
                if (strcmp(CLS_BUTTON, name) == 0){                                       
                    view = XmlLoader::createButton(subNode, hWnd);                    
                } else if (strcmp(CLS_IMAGE, name) == 0){
                    view = XmlLoader::createImageView(subNode, hWnd);
                } else if (strcmp(CLS_LIST, name) == 0){
                    view = XmlLoader::createListView(subNode, hWnd);
                } else if (strcmp(CLS_PROGRESS, name) == 0){
                    view = XmlLoader::createProgressBar(subNode, hWnd);
                } else if (strcmp(CLS_RADIO, name) == 0){
                    view = XmlLoader::createRadioButton(subNode, hWnd);
                } else if (strcmp(CLS_TEXT, name) == 0){
                    view = XmlLoader::createTextView(subNode, hWnd);                    
                } else if (strcmp(CLS_VIDEO, name) == 0){                    
                    view = XmlLoader::createVideoView( subNode, hWnd);                    
                } else if (strcmp(CLS_DATE_TIME, name) == 0){
                    view = XmlLoader::createDateTime(subNode, hWnd);
                }

                if (view != NULL)
                    views.push_back(view);
            }
        }            
    }

    return views;

}

LISTVIEWS XmlLoader::loadXmlWidgets(const char* xml, HWND hWnd){
	
	LISTVIEWS widgets;
	mxml_node_t* tree = load(xml);
	
	if (tree == NULL)
		return widgets;
		
	widgets = createWidgets(tree, hWnd);
	
	mxmlDelete(tree);
	
	return widgets;
}

