
#include "SimpleAudioEngine.h"
#include "MenuScene.h"
#include "HelpAndAbout.h"


USING_NS_CC;

Scene* RankScene::createScene()
{
    return RankScene::create();
}

bool RankScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* rank_background = Sprite::create("images/help_bg.jpg");
    rank_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    addChild(rank_background, 0);
}