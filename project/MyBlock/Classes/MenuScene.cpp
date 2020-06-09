
#include "MenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene *MenuScene::createScene()
{
    return MenuScene::create();
}

bool MenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //Sprite* menu_background = Sprite::create("images/menu_bg.jpg");
    //menu_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    //addChild(menu_background, 0);

    Label *start_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 35);
    start_label->setTextColor(cocos2d::Color4B::RED);

    MenuItemLabel *start_menu_item = MenuItemLabel::create(start_label, [&](Ref *sender) {
        CCLOG("click start game");

        Scene *main_game_scene = GameScene::createScene();
        TransitionScene *transition = TransitionFade::create(0.5f, main_game_scene, Color3B(255, 255, 255));
        Director::getInstance()->replaceScene(transition);
    });

    start_menu_item->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    Menu *menu = Menu::createWithItem(start_menu_item);
    menu->setPosition(Vec2::ZERO);

    addChild(menu, 1);
    return true;
}
