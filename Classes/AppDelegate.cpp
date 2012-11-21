#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "BackKeyLayer.h"

#include "tolua++.h"
#include "tolua_fix.h"
#include <platform/CCFileUtils.h>
#include "cocos2d.h"
#include <string.h>
#include <errno.h>

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

static int tolua_BackKeyLayer_create(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
    tolua_Error tolua_err;
    if (
        !tolua_isusertable(tolua_S,1,"BackKeyLayer",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
	)
      goto tolua_lerror;
    else
#endif
    {
        CCLayer* tolua_ret = (CCLayer*)  BackKeyLayer::create();
        int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"BackKeyLayer");
    }

    return 1;
#ifndef TOLUA_RELEASE
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

static void toluaRegisterTypes(lua_State *tolua_S)
{
    tolua_usertype(tolua_S, "BackKeyLayer");
}

static void registerCustomCppClasses(lua_State *tolua_S)
{
    tolua_open(tolua_S);
    toluaRegisterTypes(tolua_S);
    tolua_module(tolua_S,NULL,0);
    tolua_beginmodule(tolua_S,NULL);
    tolua_cclass(tolua_S, "BackKeyLayer", "BackKeyLayer", "CCLayer", NULL);
    /* BackKeyLayer */
    tolua_beginmodule(tolua_S, "BackKeyLayer");
    tolua_function(tolua_S,"create", tolua_BackKeyLayer_create);
    tolua_endmodule(tolua_S);
    /* BackKeyLayer */
    tolua_endmodule(tolua_S);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCLog("AppDelegate::applicationDidFinishLaunching");

    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    

    // going to test lua bindings
    CCLuaEngine *pEngine = CCLuaEngine::defaultEngine();
    registerCustomCppClasses(pEngine->getLuaState());
    
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    const char *pLuaFile = "/sdcard/test.lua";
    CCString *pstrFileContent = CCString::createWithContentsOfFile(pLuaFile);
    if (pstrFileContent)
    {
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("hello.lua");
    pEngine->addSearchPath(path.substr(0, path.find_last_of("/")).c_str());
    pEngine->executeScriptFile(path.c_str());
#endif

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->resume();
    
    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
