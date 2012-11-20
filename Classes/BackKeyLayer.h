#ifndef BACKKEYLAYER_H
#define BACKKEYLAYER_H

#include "cocos2d.h"

USING_NS_CC;


class BackKeyLayer : public CCLayer
{
 public:
  virtual bool init();
  // needed for custom init()
  CREATE_FUNC(BackKeyLayer);
  // back key for android
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  virtual void keyBackClicked();
#endif
};

#endif
