
#include "AudioEngine.h"
#include "ui/CocosGUI.h"
#include "ModeChoose.h"
#include "GameScene.h"
#include "MenuScene.h"
#include <tuple>
#include <string>
#include <functional>

USING_NS_CC;
using namespace ui; // button namespace


Scene* ModeChoose::createScene()
{
    return ModeChoose::create();
}

bool ModeChoose::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* menu_background = Sprite::create("images/menu_bg.jpg");
    menu_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    addChild(menu_background, 0);


    vector<tuple<std::string, std::function<void(void)>>> _menu = {
        { "Easy Mode",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, GameScene::createScene(6,6,4));
                Director::getInstance()->replaceScene(transition_scene);
            }
        },
        { "Middle Mode",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, GameScene::createScene(7,7,5));
                Director::getInstance()->replaceScene(transition_scene);
            }
        },
       { "Hard Mode",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, GameScene::createScene(8,8,6));
                Director::getInstance()->replaceScene(transition_scene);
            }
        },
       { "Back",[]() {
                TransitionScene* transition_scene = TransitionFade::create(0.5, MenuScene::createScene());
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
            _button->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * (0.7 - (i++) / 8.0)));
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

