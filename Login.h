
#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"

class LoginScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void loginButtonCallback(Ref* pSender);//µÇÂ¼º¯Êý
    void registerButtonCallback(Ref* pSender);//×¢²áº¯Êý


    CREATE_FUNC(LoginScene);

}£»
#endif // !__LOGIN_SCENE_H__