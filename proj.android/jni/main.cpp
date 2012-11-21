#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "cocos2d.h"


#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

/* android.os.Environment */
typedef struct {
    jclass klass;
    jmethodID get_external_storage_directory;
} android_os_environment_class;

/* java.io.File */
typedef struct {
    jclass klass;
    jmethodID get_path;
} java_io_file_class;

static void get_sdcard_path(JNIEnv* env, char* dst)
{
    android_os_environment_class environment_class;
    java_io_file_class f_class;
    jobject f;
    jstring j_path;
    const char* n_path;
    environment_class.klass = env->FindClass("android.os.Environment");
    environment_class.get_external_storage_directory
        = env->GetStaticMethodID(environment_class.klass,
                                 "getExternalStorageDirectory",
                                 "()Ljava/io/File;");

    f_class.klass = env->FindClass("java.io.File");
    f_class.get_path = env->GetMethodID(f_class.klass,
                                        "getPath",
                                        "()Ljava/lang/String;");
    
    f = env->CallStaticObjectMethod(environment_class.klass,
                                    environment_class.get_external_storage_directory);

    j_path = (jstring) env->CallObjectMethod(f, f_class.get_path);
    if(j_path != NULL){
        n_path = env->GetStringUTFChars(j_path, NULL);
        strcpy(dst, n_path);
    }

    env->ReleaseStringUTFChars(j_path, n_path);
    env->DeleteLocalRef(j_path);
    env->DeleteLocalRef(f_class.klass);
    env->DeleteLocalRef(environment_class.klass);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        char path[256];
        get_sdcard_path(env, path);
        pAppDelegate->setScriptRoot(path);
        CCApplication::sharedApplication()->run();
    }
    else
    {
        ccDrawInit();
        ccGLInvalidateStateCache();
        
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVNET_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues(); 
    }
}

}    /* extern "C" */
