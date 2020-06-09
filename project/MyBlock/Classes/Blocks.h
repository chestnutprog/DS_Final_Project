#pragma once
#include <vector>
#include <list>
#include <memory>
#include "CallbackWaitAll.h"
#include "BaseBlock.h"
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class Blocks : public cocos2d::Node
{
public:
    cocos2d::Node& _father;
    const static int height = 8;
    const static int width = 9;
    inline const static float LeftMargin = 20;
    inline const static float RightMargin = 20;
    inline const static float ButtomMargin = 70;
    int getColor(int x, int y);
    bool swap(int row, int col, int direction, CallbackWaitAll_counter _counter);
    bool try_swap(int x_1, int y_1, int direction);
    bool has_solution();
    vector<tuple<int, int, bool>> Blocks::solutions();
    void Blocks::crash(CallbackWaitAll_counter _counter);
    vector<pair<int, int>> try_crash();
    float block_size;

private:
    BaseBlock* blocks[height][width];

public:
    Blocks(cocos2d::Node& father) : _father{ father }
    {
        const Size visibleSize = Director::getInstance()->getVisibleSize();
        block_size = (visibleSize.width - LeftMargin - RightMargin) / width;
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                blocks[i][j] = BaseBlock::create(*this, rand() % 6, { i, j });
                _father.addChild(blocks[i][j], 1);
            }
    }
};