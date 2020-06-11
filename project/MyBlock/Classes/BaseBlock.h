#pragma once
#include <string>
#include <cstdlib>
#include <memory>
#include "cocos2d.h"
#include "CallbackWaitAll.h"
using namespace std;
using pos_t = std::pair<int, int>;
class Blocks;
class BaseBlock : public cocos2d::Sprite
{
    const static std::string BlockImgArray[];

public:
    const int color;
    pos_t position;
    Blocks &_blocks;

    virtual bool init();

    BaseBlock(Blocks &_blocks, int color, pos_t pos) : _blocks{_blocks}, color{color}, position{pos} {}

    static BaseBlock* create(Blocks& _blocks, int color, pos_t pos);

    void dropTo(pos_t pos, CallbackWaitAll_counter counter);
    void swapTo(pos_t pos, int flag, CallbackWaitAll_counter counter);
    void vanish(CallbackWaitAll_counter counter);
    void vanishCallback();
};