
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "RankList.h"
#include "HelpAndAbout.h"
#include "Register.h"


USING_NS_CC;
using namespace CocosDenshion;
using namespace ui; // button namespace


Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

bool MenuScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* menu_background = Sprite::create("images/menu_bg.jpg");
    menu_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    addChild(menu_background, 0);


    Button* start_btn = Button::create("img/start.png"); // 添加开始按钮
    start_btn->setScale(1.5);
    start_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 30));
    start_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        // 按钮点击事件
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            // 播放音效
            SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // 切换到游戏场景
            auto game_scene = GameScene::createScene();
            TransitionScene* transition_scene = TransitionFade::create(0.5, game_scene);
            Director::getInstance()->replaceScene(transition_scene);
        }
        break;
        default:
            break;
        }
        });
    
    addChild(start_btn);

    Button* login_btn = Button::create("img/login.png"); // 添加注册登录按钮
    login_btn->setScale(1.5);
    login_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 60));
    login_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        // 按钮点击事件
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            // 播放音效
            SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // 切换到游戏场景
            auto login_scene = LoginScene::createScene();
            TransitionScene* transition_scene = TransitionFade::create(0.5, login_scene);
            Director::getInstance()->replaceScene(transition_scene);
        }
        break;
        default:
            break;
        }
        });

    addChild(login_btn);

    Button* rank_btn = Button::create("img/rank.png"); // 添加排行榜按钮
    start_btn->setScale(1.5);
    start_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 90));
    start_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        // 按钮点击事件
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            // 播放音效
            SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // 切换到排行榜场景
            auto rank_scene = RankScene::createScene();
            TransitionScene* transition_scene = TransitionFade::create(0.5, rank_scene);
            Director::getInstance()->replaceScene(transition_scene);
        }
        break;
        default:
            break;
        }
        });

    addChild(rank_btn);

    Button* help_btn = Button::create("img/help.png"); // 添加帮助按钮
    start_btn->setScale(1.5);
    start_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 120));
    start_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        // 按钮点击事件
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            // 播放音效
            SimpleAudioEngine::getInstance()->playEffect("sound/button.wav");
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // 切换到帮助场景
            auto help_scene = HelpScene::createScene();
            TransitionScene* transition_scene = TransitionFade::create(0.5, help_scene);
            Director::getInstance()->replaceScene(transition_scene);
        }
        break;
        default:
            break;
        }
        });

    addChild(help_btn);


   /*Label* start_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 35);
    start_label->setTextColor(cocos2d::Color4B::RED);


    MenuItemLabel* start_menu_item = MenuItemLabel::create(start_label, [&](Ref* sender) {
        CCLOG("click start game");

        Scene* main_game_scene = GameScene::createScene();
        TransitionScene* transition = TransitionFade::create(0.5f, main_game_scene, Color3B(255, 255, 255));
        Director::getInstance()->replaceScene(transition);
        });

    start_menu_item->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    Menu* menu = Menu::createWithItem(start_menu_item);
    menu->setPosition(Vec2::ZERO);

    addChild(menu, 1);
    */

    return true;
}

