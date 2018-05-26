#ifndef ITODAY_RESOURCE_MANAGER_H
#define ITODAY_RESOURCE_MANAGER_H

#include   <map>
#include   <string>
extern "C"{
#include <minigui/common.h>
#include <minigui/gdi.h>

}
using namespace std;

typedef map<string, PBITMAP> IMGS_MAP;
typedef map<string, PLOGFONT> FONTS_MAP;

#define RESOURCE_ROOT "/data/res"

#define LANGUAGE_CH    "ch"
#define LANGUAGE_EN    "en"

class ResourceManager {
    
	static IMGS_MAP mImgs;
    static FONTS_MAP mFonts;

    public:
        static PBITMAP loadImg(string path);

        static PLOGFONT loadFont(string family, string charset, int size);

        static PLOGFONT loadFont(string family, int size);
        
        static void release();

    private:
        static const char* getAbsPath(const char* filp);

};

#endif