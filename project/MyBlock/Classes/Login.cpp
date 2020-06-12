#include "Login.h"
#include "ui\CocosGUI.h"
#include "MenuScene.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace ui; // button namespace

Scene* LoginScene::createScene()
{
    return LoginScene::create();
}

bool LoginScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* login_background = Sprite::create("images/login_bg.jpg");
    login_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    addChild(login_background, 0);

    Button* menu_btn = Button::create("images/menu.png"); // 添加返回按钮
    menu_btn->setScale(1.25);
    menu_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 -300));
    menu_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        // 按钮点击事件
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            // 播放音效
            AudioEngine::play2d("sound/button.mp3");
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // 切换到游戏场景
            auto menu_scene = MenuScene::createScene();
            TransitionScene* transition_scene = TransitionFade::create(0.5, menu_scene);
            Director::getInstance()->replaceScene(transition_scene);
        }
        break;
        default:
            break;
        }
        });

    addChild(menu_btn);

    auto loginButton = MenuItemFont::create("Login", CC_CALLBACK_1(LoginScene::LoginButtonCallback, this));// 添加登录按钮
    if (loginButton) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height  - 720.0f;
        loginButton->setColor(Color3B::BLUE);
        loginButton->setPosition(Vec2(x, y));
    }

    auto registerButton = MenuItemFont::create("Register", CC_CALLBACK_1(LoginScene::RegisterButtonCallback, this));// 添加注册按钮
    if (registerButton) {
        float x = origin.x + visibleSize.width / 2 - 200.0f;
        float y = origin.y + visibleSize.height - 720.0f;
        registerButton->setColor(Color3B::BLUE);
        registerButton->setPosition(Vec2(x, y));
    }

    auto changeButton = MenuItemFont::create("Change Password", CC_CALLBACK_1(LoginScene::ChangeButtonCallBack, this));// 添加注册按钮
    if (changeButton) {
        float x = origin.x + visibleSize.width / 2 + 200.0f;
        float y = origin.y + visibleSize.height - 720.0f;
        changeButton->setColor(Color3B::BLUE);
        changeButton->setPosition(Vec2(x, y));
    }

   
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    menu->addChild(loginButton);
    menu->addChild(registerButton);
    menu->addChild(changeButton);
    this->addChild(menu, 1);
    

    usernameInput = TextField::create("username", "arial", 24);// 添加用户名文本框
    
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height - 360.0f;
        usernameInput->setPosition(Vec2(x, y));
        usernameInput->setColor(Color3B::BLUE);
       
        usernameInput->setCursorEnabled(true);
        addChild(usernameInput, 1);
    


    passwordInput = TextField::create("password", "arial", 24);// 添加密码文本框
    if (passwordInput) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height - 480.0f;
        passwordInput->setPosition(Vec2(x, y));
        passwordInput->setColor(Color3B::BLUE);
        passwordInput->setCursorEnabled(true);

        addChild(passwordInput, 1);
    }

    newpasswordInput = TextField::create("newPassword", "arial", 24);// 添加新密码文本框
    if (newpasswordInput) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height - 600.0f;
        newpasswordInput->setPosition(Vec2(x, y));

        newpasswordInput->setColor(Color3B::BLUE);
        newpasswordInput->setCursorEnabled(true);

        addChild(newpasswordInput, 1);
    }

    
    messageBox = Label::create();
    messageBox->setSystemFontName("arial");
    messageBox->setSystemFontSize(60);
    messageBox->setColor(Color3B::RED);
    if (messageBox) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height - 600.0f;
        messageBox->setPosition(Vec2(x, y));
        this->addChild(messageBox, 1);
    }
    
    return true;
}
// 登录函数
void LoginScene::LoginButtonCallback(cocos2d::Ref* pSender) {

    std::string username = usernameInput->getString();
    std::string password = passwordInput->getString();
    
    Client::getInstance()->login(username,password, CC_CALLBACK_2(LoginScene::LoginCallBack, this));
}
//登录回调
void LoginScene::LoginCallBack(bool suc, string response) {

    if (!suc) {
        this->messageBox->setString(response);

        auto relogin_scene = LoginScene::createScene();
        TransitionScene* transition_scene = TransitionFade::create(0.5, relogin_scene);
        Director::getInstance()->replaceScene(transition_scene);

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        messageBox->runAction(Sequence::create(DelayTime::create(6.0), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
            messageBox->setVisible(false);
            messageBox->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 600.0f);
            }),
            NULL));


        return;
    }
    else {
        this->messageBox->setString("Login Successful！");

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        messageBox->runAction(Sequence::create(DelayTime::create(6.0), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
            messageBox->setVisible(false);
            messageBox->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 600.0f);
            }),
            NULL));

        auto game_scene = GameScene::createScene();
        TransitionScene* transition_scene = TransitionFade::create(0.5, game_scene);
        Director::getInstance()->replaceScene(transition_scene);

    }

}

// 注册函数
void LoginScene::RegisterButtonCallback(cocos2d::Ref* pSender) {

    std::string username = usernameInput->getString();
    std::string password = passwordInput->getString();

    Client::getInstance()->reg(username, password, CC_CALLBACK_2(LoginScene::RegCallBack, this));
}

//注册回调
void LoginScene::RegCallBack(bool suc, string response) {

    if (!suc) {
        this->messageBox->setString(response);

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        messageBox->runAction(Sequence::create(DelayTime::create(6.0), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
            messageBox->setVisible(false);
            messageBox->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 600.0f);
            }),
            NULL));

        auto relogin_scene = LoginScene::createScene();
        TransitionScene* transition_scene = TransitionFade::create(0.5, relogin_scene);
        Director::getInstance()->replaceScene(transition_scene);

        return;
    }

}

// 修改密码函数
void LoginScene::ChangeButtonCallBack(cocos2d::Ref* pSender) {

    std::string username = usernameInput->getString();
    std::string password = passwordInput->getString();
    std::string newpassword = newpasswordInput->getString();

    Client::getInstance()->updatePassword(password , newpassword, CC_CALLBACK_2(LoginScene::LoginCallBack, this));

}

//修改密码回调
void LoginScene::ChangeCallBack(bool suc, string response) {

    if (!suc) {
        this->messageBox->setString(response);

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        messageBox->runAction(Sequence::create(DelayTime::create(6.0), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
        messageBox->setVisible(false);																 
        messageBox->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 600.0f);
            }),
            NULL));

        auto relogin_scene = LoginScene::createScene();
        TransitionScene* transition_scene = TransitionFade::create(0.5, relogin_scene);
        Director::getInstance()->replaceScene(transition_scene);

        return;
    }
    else {
        this->messageBox->setString("Change Password Successful！");

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        messageBox->runAction(Sequence::create(DelayTime::create(6.0), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
            messageBox->setVisible(false);
            messageBox->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 600.0f);
            }),
            NULL));
    }
}


