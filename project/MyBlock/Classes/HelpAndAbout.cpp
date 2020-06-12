
#include "AudioEngine.h"
#include "MenuScene.h"
#include "HelpAndAbout.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

using namespace ui; // button namespace

Scene* HelpScene::createScene()
{
    return HelpScene::create();
}

bool HelpScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* help_background = Sprite::create("images/help_bg.jpg");
    help_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    addChild(help_background, 0);

    Button* menu_btn = Button::create("images/menu.png"); // ��ӷ��ذ�ť
    menu_btn->setScale(1.5);
    menu_btn->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 300));
    menu_btn->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        // ��ť����¼�
        switch (type)
        {
        case ui::Widget::TouchEventType::BEGAN:
            // ������Ч
            AudioEngine::play2d("sound/button.mp3");
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // �л�����Ϸ����
            auto menu_scene = MenuScene::createScene();
            TransitionScene* transition_scene = TransitionFade::create(0.5, menu_scene);
            Director::getInstance()->replaceScene(transition_scene);
        }
        break;
        default:
            break;
        }
        });

    addChild(menu_btn,0);

    return true;

}