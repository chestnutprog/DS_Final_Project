#pragma once
#include <string>
#include <cstdlib>
#include <complex>
#include <memory>
#include "cocos2d.h"
#include "CallbackWaitAll.h"
using namespace std;
using pos_t = std::complex<int>;
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

    void dropTo(pos_t pos, shared_ptr<CallbackWaitAll> counter);
    void swapTo(pos_t pos, int flag, shared_ptr<CallbackWaitAll> counter);
    void vanish(shared_ptr<CallbackWaitAll> counter);
    void vanishCallback();
};