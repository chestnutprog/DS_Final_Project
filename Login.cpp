#include "LoginScene.h"
#include "ui\CocosGUI.h"
#include "MenuScene.h"

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

    auto loginButton = MenuItemFont::create("Login", CC_CALLBACK_1(LoginScene::LoginButtonCallback, this));// 添加登录按钮
    if (loginButton) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + loginButton->getContentSize().height / 2;
        loginButton->setPosition(Vec2(x, y));
    }

    auto registerButton = MenuItemFont::create("Register", CC_CALLBACK_1(LoginScene::RegisterButtonCallback, this));// 添加注册按钮
    if (registerButton) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + registerButton->getContentSize().height / 2 + 100;
        registerButton->setPosition(Vec2(x, y));
    }

    auto menu = Menu::create(loginButton, registerButton, backButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    usernameInput = TextField::create("username", "arial", 24);// 添加用户名文本框
    if (usernameInput) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height - 100.0f;
        usernameInput->setPosition(Vec2(x, y));
        this->addChild(usernameInput, 1);
    }

    passwordInput = TextField::create("password", "arial", 24);// 添加密码文本框
    if (passwordInput) {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height - 130.0f;
        passwordInput->setPosition(Vec2(x, y));
        this->addChild(passwordInput, 1);
    }

    return true;
}
// 登录函数
void LoginScene::LoginButtonCallback(cocos2d::Ref* pSender) {

    std::string username = usernameInput->getStringValue();
    std::string password = passwordInput->getStringValue();

}

// 注册函数
void LoginScene::RegisterButtonCallback(cocos2d::Ref* pSender) {

    std::string username = usernameInput->getStringValue();
    std::string password = passwordInput->getStringValue();

}



