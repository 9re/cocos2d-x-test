LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := main.cpp \
                  ../../Classes/AppDelegate.cpp \
		  ../../Classes/BackKeyLayer.cpp \
	          ../../../scripting/lua/cocos2dx_support/CCLuaEngine.cpp \
                  ../../../scripting/lua/cocos2dx_support/Cocos2dxLuaLoader.cpp \
                  ../../../scripting/lua/cocos2dx_support/LuaCocos2d.cpp \
                  ../../../scripting/lua/cocos2dx_support/tolua_fix.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes                   

LOCAL_EXPORT_CFLAGS := -DCOCOS2D_DEBUG=2 -DCOCOS2D_JAVASCRIPT

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx)
$(call import-module,extensions)
$(call import-module,scripting/lua/proj.android/jni)
