
#include "resource_manager.h"
#include "iLock_resource.h"
#include <utils/log.h>
#include <unistd.h>


#define LOG_TAG "ResourceManager"

#define FONT_FAMILY_KAITI "kaiti"
#define FONT_FAMILY_CLOCK "Clockopia"

#define FONT_CHARSET_GB2312_0       "GB2312-0"
#define FONT_CHARSET_ISO8859_1      "ISO8859-1"

IMGS_MAP ResourceManager::mImgs;
FONTS_MAP ResourceManager::mFonts;

#define FONTS_MAP_SIZE 2

static string FONTS_FIAMILY_CHARSET_MAP[FONTS_MAP_SIZE][2] = {
    {FONT_FAMILY_KAITI, FONT_CHARSET_GB2312_0},
    {FONT_FAMILY_CLOCK, FONT_CHARSET_ISO8859_1},
};


PBITMAP ResourceManager::loadImg(string path){

    string absPath = getAbsPath(path.c_str());

    map<string, PBITMAP>::iterator i = mImgs.find(absPath);

    if (i != mImgs.end()){
        LOGE("ResourceManager::loadImg %s already loaded \n", absPath.c_str());
        return i->second;
    }
    LOGD("ResourceManager::loadImg %s \n", absPath.c_str());

    PBITMAP bmp = new BITMAP();

    if (bmp == NULL)
        LOGE("\n load failed %s \n ", absPath.c_str());
    
    int res = LoadBitmap (HDC_SCREEN, bmp, absPath.c_str());

    if (res == 0)
        mImgs.insert(make_pair(absPath, bmp));
    else
        LOGE("\n load failed %s , res %d\n ", absPath.c_str(), res);    

    return bmp;
}

const char* ResourceManager::getAbsPath(const char* filp){
    
    static char tmp_path[256] = {0};

    sprintf(tmp_path, "%s/%s/%s", RESOURCE_ROOT, LANGUAGE_CH, filp);

    if (access(tmp_path, F_OK) == 0)
        return tmp_path;

    return filp;
}

PLOGFONT ResourceManager::loadFont(string family, int size){

    string charset;

    for(int index = 0; index < FONTS_MAP_SIZE; index ++)
        if (FONTS_FIAMILY_CHARSET_MAP[index][0] == family)
            charset = FONTS_FIAMILY_CHARSET_MAP[index][1];

    return loadFont(family, charset, size);;
        
}


PLOGFONT ResourceManager::loadFont(string family, string charset, int size){
    
    string key = family + charset + char(size);

    map<string, PLOGFONT>::iterator i = mFonts.find(key);

    if (i != mFonts.end())
        return i->second;

    PLOGFONT font = CreateLogFont(FONT_TYPE_NAME_SCALE_TTF, family.c_str(), charset.c_str(),
               FONT_WEIGHT_SUBPIXEL, FONT_SLANT_ROMAN, FONT_FLIP_NIL,
               FONT_OTHER_NIL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
               size, 0);

    mFonts.insert(make_pair(key, font));

    return font;
}

void ResourceManager::release(){

    for(map<string, PBITMAP>::iterator i = mImgs.begin();  i != mImgs.end(); i++){ 
        UnloadBitmap(i->second);
    }

    mImgs.clear();

    for(map<string, PLOGFONT>::iterator i = mFonts.begin();  i != mFonts.end(); i++){ 
        DestroyLogFont(i->second);
    }

    mFonts.clear();
}

