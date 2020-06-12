
#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include <string.h>
#include "client.h"
#include <functional>
#include "GameScene.h"

using namespace ui;
using namespace std;

class LoginScene : public cocos2d::Scene {
private:
    TextField* usernameInput;
    TextField* passwordInput;
    TextField* newpasswordInput;
    Label* messageBox;

public:
    static cocos2d::Scene* createScene();

    //auto usernameInput;

    virtual bool init();

    void LoginButtonCallback(Ref* pSender);//µÇÂ¼º¯Êý
    void RegisterButtonCallback(Ref* pSender);//×¢²áº¯Êý
    void ChangeButtonCallBack(Ref* psender);

    void LoginCallBack(bool suc, string response);//µÇÂ¼»Øµ÷
    void RegCallBack(bool suc, string response);//×¢²á»Øµ÷
    void ChangeCallBack(bool suc, string response);

    CREATE_FUNC(LoginScene);

};
#endif // !__LOGIN_SCENE_H__