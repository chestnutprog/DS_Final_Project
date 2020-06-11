#include <string>
#include "BaseBlock.h"
#include "Blocks.h"
#include "GameScene.h"
#include "CallbackWaitAll.h"
#include "cocos2d.h"
using namespace cocos2d;
const std::string BaseBlock::BlockImgArray[] = {
    "images/b1.png",
    "images/b2.png",
    "images/b3.png",
    "images/b4.png",
    "images/b5.png",
    "images/b6.png"};
BaseBlock *BaseBlock::create(Blocks &_blocks, int color, pos_t pos)
{
    BaseBlock *sprite = new BaseBlock(_blocks, color, pos);
    if (sprite->init())
    {
        sprite->autorelease();
    }
    else
    {
        delete sprite;
        sprite = NULL;
        return NULL;
    }
    return sprite;
}

bool BaseBlock::init()
{
    if (!Sprite::initWithFile(BlockImgArray[color]))
    {
        return false;
    }
    setContentSize(Size(_blocks.block_size, _blocks.block_size));
    setPosition(_blocks.LeftMargin + (position.second + 0.5) * _blocks.block_size, _blocks.BottomMargin + (position.first + 0.5) * _blocks.block_size);
    //scheduleOnce(CC_SCHEDULE_SELECTOR(Block::appearSchedule), 0.3);
    return true;
}

void BaseBlock::dropTo(pos_t pos, CallbackWaitAll_counter counter) {
    position = pos;
    setPosition(_blocks.LeftMargin + (position.second + 0.5) * _blocks.block_size, _blocks.BottomMargin + (position.first + 1) * _blocks.block_size);
    auto real_position = Vec2(_blocks.LeftMargin + (position.second + 0.5) * _blocks.block_size, _blocks.BottomMargin + (position.first + 0.5) * _blocks.block_size);
    runAction(Sequence::create(MoveTo::create(0.2, real_position), CallFunc::create([counter]() {}), nullptr));
}

void BaseBlock::swapTo(pos_t pos, int flag, CallbackWaitAll_counter counter) {
    auto old_position = Vec2(_blocks.LeftMargin + (position.second + 0.5) * _blocks.block_size, _blocks.BottomMargin + (position.first + 0.5) * _blocks.block_size);
    auto real_position = Vec2(_blocks.LeftMargin + (pos.second + 0.5) * _blocks.block_size, _blocks.BottomMargin + (pos.first + 0.5) * _blocks.block_size);
    if (flag) {
        position = pos;
        runAction(Sequence::create(MoveTo::create(0.2, real_position), CallFunc::create([counter]() {counter; }), nullptr));
        //runAction(Sequence::create(MoveTo::create(0.2, real_position), MoveTo::create(0.2, old_position), CallFunc::create([&]() {((GameScene&)(_blocks._father)).cant_touch--; }), nullptr));
    }
    else {
        runAction(Sequence::create(MoveTo::create(0.2, real_position), MoveTo::create(0.2, old_position), CallFunc::create([counter]() {counter; }), nullptr));
    }
}


void BaseBlock::vanish(CallbackWaitAll_counter counter)
{
    // 延时显示动画
    setTexture("images/star.png");
    setContentSize(Size(_blocks.block_size, _blocks.block_size));
    ScaleTo* scale_to = ScaleTo::create(0.2, 0.5);
    Sequence* sequence = Sequence::create(DelayTime::create(0.2), scale_to, CallFunc::create([counter]() {counter; }), CallFunc::create([&]() { vanishCallback(); }), nullptr);
    runAction(sequence);
}

void BaseBlock::vanishCallback()
{
    removeFromParent();
}
