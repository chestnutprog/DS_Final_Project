
#include "MenuScene.h"
//#include "GameScene.h"
#include "RankList.h"


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

    Sprite* rank_background = Sprite::create("images/rank_bg.jpg");
    rank_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    addChild(rank_background, 0);

    if (player_num > 4) player_num = 4;//只显示前四强
    for (int i = 1; i <= player_num; i++)
    {
         Label* player_score_label = Label::createWithTTF(std::to_string(_player_list[i].p_score), "fonts/Marker Felt.ttf", 14);
         player_score_label->setColor(Color3B::WHITE);
         player_score_label->setPosition(origin.x + visibleSize.width / 2 - 30,
             origin.y + visibleSize.height / 2 - 30*i);
         addChild(player_score_label);

         Label* player_label = Label::createWithTTF(StringUtils::format(_player_list[i].name), "fonts/Marker Felt.ttf", 14);
         player_label->setColor(Color3B::WHITE);
         player_label->setPosition(origin.x + visibleSize.width / 2 + 10,
             origin.y + visibleSize.height / 2 - 30 * i);
         addChild(player_label);
    }

    return true;
}
