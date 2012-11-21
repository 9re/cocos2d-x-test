#include "BackKeyLayer.h"

bool BackKeyLayer::init()
{
  if (!CCLayer::init())
    {
      return false;
    }

  CCLog("BackKeyLayer::init()");
  
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  setKeypadEnabled(true);
#endif

  return true;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void BackKeyLayer::keyBackClicked()
{
  CCLog("BackKeyLayer::keyBackClicked");
  CCDirector::sharedDirector()->end();
  exit(0);
}
#endif
