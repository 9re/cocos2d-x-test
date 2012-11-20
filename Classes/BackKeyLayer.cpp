#include "BackKeyLayer.h"

CCLayer* BackKeyLayer::create()
{
  CCLog("BackKeyLayer::create");
  return CCLayer::create();
}

void BackKeyLayer::keyBackClicked()
{
  CCDirector::sharedDirector()->end();
  exit(0);
}
