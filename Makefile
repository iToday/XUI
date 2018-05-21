#
#  Copyright (C) 2017, Wang Qiuwei <qiuwei.wang@ingenic.com / panddio@163.com>
#
#  Ingenic ilock Project
#
#  This program is free software; you can redistribute it and/or modify it
#  under  the terms of the GNU General  Public License as published by the
#  Free Software Foundation;  either version 2 of the License, or (at your
#  option) any later version.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  675 Mass Ave, Cambridge, MA 02139, USA.
#

CURDIR := $(shell pwd)
TOPDIR := $(CURDIR)/../..
TARGET_DEVICE ?= iToday
TARGET_APP_NAME := xui
include $(TOPDIR)/build/config.mk

CCP := mips-linux-gnu-g++
CC := mips-linux-gnu-gcc

STRIP = mips-linux-gnu-strip

OBJ_DIR := target
OUT_DIR := out
APP_SRC := $(wildcard ./ime/*.c)
 
APP_SRC_CPP += $(wildcard ./widget/*.cpp)

APP_SRC_CPP += $(wildcard ./ui/*.cpp)

APP_SRC_CPP += $(wildcard *.cpp)

APP_OBJ := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir ${APP_SRC}))

APP_OBJ_CPP += $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir ${APP_SRC_CPP}))

DYNAMIC_LIB := $(CURDIR)/lib/libiconv.a 

CFLAGS   := -Os -std=c++11 -g -mhard-float
INCFLAGS := -I$(CURDIR)/include -I$(SDK_BUILD_PATH)/include -I$(CURDIR)
LDFLAFS  := -L$(CURDIR)/lib -Wl,-Bdynamic                       \
            -lpthread -lrt -lm -lminigui_ths -ljpeg -lpng -ldl -lm -lz -lts -lfreetype  \	        
            -lcrypto -lssl                                     \
            -lswscale


.PHONY : all
all:$(OUT_DIR) $(OBJ_DIR) $(TARGET_APP_NAME)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	
$(OBJ_DIR)/%.o: ./widget/%.cpp 
	$(CCP) -c $(INCFLAGS) $(CFLAGS) $< -o $@
	
$(OBJ_DIR)/%.o: ./ui/%.cpp 
	$(CCP) -c $(INCFLAGS) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o: %.cpp 
	$(CCP) -c $(INCFLAGS) $(CFLAGS) $< -o $@

$(TARGET_APP_NAME):$(APP_OBJ_CPP) $(APP_OBJ)
	$(CCP) $(CFLAGS) $(LDFLAFS) $(APP_OBJ_CPP) $(APP_OBJ) $(DYNAMIC_LIB) -o $(OUT_DIR)/$@
	@$(STRIP) $(OUT_DIR)/$@
	$(shell if [ -e $(INSTALL_APP_EXEFILE_TO_FSDIR) ];then echo cp -af $(OUT_DIR)/$(TARGET_APP_NAME) $(INSTALL_APP_EXEFILE_TO_FSDIR); fi)


#
# For clean
.PHONY:clean distclean
clean:
	@rm -rf $(OUT_DIR)
	@rm -rf $(OBJ_DIR)

distclean:clean

