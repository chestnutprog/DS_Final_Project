
#include "MenuScene.h"
//#include "GameScene.h"
#include "RankList.h"
#include "ui/CocosGUI.h"
#include "AudioEngine.h"
#include "cocos2d.h"
#include "client.h"


USING_NS_CC;
using namespace ui;

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

    Label* player_score_label = Label::createWithTTF(("SCORE"), "fonts/Marker Felt.ttf", 30);
    player_score_label->setColor(Color3B::WHITE);
    player_score_label->setPosition(origin.x + visibleSize.width / 2 - 100,
        origin.y + visibleSize.height / 2 - 400 * -1);

    addChild(player_score_label);

    Label* player_label = Label::createWithTTF(("name"), "fonts/Marker Felt.ttf", 30);
    player_label->setColor(Color3B::WHITE);
    player_label->setPosition(origin.x + visibleSize.width / 2 + 100,
        origin.y + visibleSize.height / 2 - 400 * -1);
    addChild(player_label);

    Client::getInstance()->ranklist([this, origin, visibleSize](bool is_success, string message, auto vec) 
        {
        if (is_success) {
            int i = 1;
            for (auto [name, score] : vec) {
                // show name score
               
                Label* player_score_label = Label::createWithTTF(std::to_string(score), "fonts/Marker Felt.ttf", 20);
                player_score_label->setColor(Color3B::WHITE);
                player_score_label->setPosition(origin.x + visibleSize.width / 2 - 100,
                    origin.y + visibleSize.height/2 +400 - 50 * i);
                
                addChild(player_score_label);

                Label* player_label = Label::createWithTTF((name), "fonts/Marker Felt.ttf", 20);
                player_label->setColor(Color3B::WHITE);
                player_label->setPosition(origin.x + visibleSize.width / 2 + 100,
                    origin.y + visibleSize.height/2 +400 - 50 * i);
                addChild(player_label);

                i++;
            }
        }
        else {
            //show message
            
        }
        });

   
    /*
    if (player_num > 4) player_num = 4;//只显示前四强
    for (int i = 1; i <= player_num; i++)
    {
         Label* player_score_label = Label::createWithTTF(std::to_string(_player_list[i].p_score), "fonts/Marker Felt.ttf", 14);
         player_score_label->setColor(Color3B::WHITE);
         player_score_label->setPosition(origin.x + visibleSize.width / 2 - 100,
             origin.y + visibleSize.height / 2 - 100*i);
         addChild(player_score_label);

         Label* player_label = Label::createWithTTF(StringUtils::format(_player_list[i].name), "fonts/Marker Felt.ttf", 14);
         player_label->setColor(Color3B::WHITE);
         player_label->setPosition(origin.x + visibleSize.width / 2 + 100,
             origin.y + visibleSize.height / 2 - 100 * i);
         addChild(player_label);
    }
    */
    Button* menu_btn = Button::create("images/menu.png"); // 添加返回按钮
    menu_btn->setScale(1.25);
    menu_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 300));
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

    addChild(menu_btn, 0);

    return true;
}
