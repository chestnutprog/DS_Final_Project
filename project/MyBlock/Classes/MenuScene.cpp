
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
#include "ModeChoose.h"


USING_NS_CC;
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

    auto _label = Label::createWithBMFont("fonts/futura-48.fnt", StringUtils::format("Block Crash"));
    _label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8);
    addChild(_label, 1);

    vector<tuple<std::string, std::function<void(void)>>> _menu = {
        { "Start Game",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, ModeChoose::createScene());
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
    
    return true;
}

