
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "RankList.h"
#include <tuple>
#include <string>
#include <functional>
#include "HelpAndAbout.h"
#include "Login.h"
//#include "HelpAndAbout.h"
//#include "Register.h"


USING_NS_CC;
//using namespace CocosDenshion;
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


    vector<tuple<std::string, std::function<void(void)>>> _menu = {
        { "Start Game",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, GameScene::createScene());
                Director::getInstance()->replaceScene(transition_scene);
            }
        },
        { "Rank List",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, RankScene::createScene());
                Director::getInstance()->replaceScene(transition_scene);
            }
        },
       { "Help And About",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, HelpScene::createScene());
                Director::getInstance()->replaceScene(transition_scene);
            }
        },
       { "Login and Register",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, LoginScene::createScene());
                Director::getInstance()->replaceScene(transition_scene);
            }
        }

    };
    {
        int i = 0;
        for (auto& [title, action] : _menu) {
            Button* _button = Button::create("cocosui/backtotopnormal.png", "cocosui/backtotoppressed.png");
            _button->setTitleText(title);
            _button->setTitleFontSize(24);
            _button->setScale(1.0f);
            _button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * ( 0.7 - (i++)/8.0)));
            _button->addTouchEventListener([action](Ref* sender, Widget::TouchEventType type) {
                switch (type)
                {
                case ui::Widget::TouchEventType::BEGAN:
                    AudioEngine::play2d("sound/pop.mp3");
                    break;
                case ui::Widget::TouchEventType::ENDED:
                    action();
                    break;
                }
                });
            addChild(_button);
        }
    }
    

    /*
   Label* start_label = Label::createWithTTF("Start Game", "fonts/Marker Felt.ttf", 35);
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

