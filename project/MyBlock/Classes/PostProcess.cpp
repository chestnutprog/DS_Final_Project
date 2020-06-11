#include "PostProcess.h"
#include <string>
#include "cocos2d.h"
#include "renderer/backend/Device.h"

using namespace cocos2d;

bool PostProcess::init(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
    if (!Layer::init()) {
        return false;
    }
    auto vertexSource = FileUtils::getInstance()->getStringFromFile(vertexShaderFile);
    auto fragSource = FileUtils::getInstance()->getStringFromFile(fragmentShaderFile);
    _program = backend::Device::getInstance()->newProgram(vertexSource, fragSource);
    int     fogEquation = 0;

    _progState = new (std::nothrow) backend::ProgramState(_program);

    //_program->updateUniforms();

    auto visibleSize = Director::getInstance()->getVisibleSize();

    _renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
    _renderTexture->retain();

    _sprite = Sprite::createWithTexture(_renderTexture->getSprite()->getTexture());
    _sprite->setTextureRect(Rect(0, 0, _sprite->getTexture()->getContentSize().width,
        _sprite->getTexture()->getContentSize().height));
    _sprite->setAnchorPoint(Point::ZERO);
    _sprite->setPosition(Point::ZERO);
    _sprite->setFlippedY(true);
    //_sprite->setProgram(_program);
    _sprite->setProgramState(_progState);
    this->addChild(_sprite);

    return true;
}

void PostProcess::draw(cocos2d::Layer* layer)
{
    _renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    layer->visit();
    _renderTexture->end();
}

PostProcess* PostProcess::create(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
    auto p = new (std::nothrow) PostProcess();
    if (p && p->init(vertexShaderFile, fragmentShaderFile)) {
        p->autorelease();
        return p;
    }
    delete p;
    return nullptr;
}