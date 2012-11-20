#ifndef BACKKEYLAYER_H
#define BACKKEYLAYER_H

#include "cocos2d.h"

USING_NS_CC;


class BackKeyLayer : public CCLayer
{
 public:
  static CCLayer* create(void);
  virtual void keyBackClicked();
};

#endif
