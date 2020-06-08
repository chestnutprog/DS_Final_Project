#ifndef __HELP_ABOUT_H__
#define __HELP_ABOUT_H__

#include "cocos2d.h"

class HelpScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(HelpScene);
};

#endif