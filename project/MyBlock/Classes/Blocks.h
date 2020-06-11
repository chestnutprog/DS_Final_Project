#pragma once
#include <vector>
#include <list>
#include <memory>
#include "CallbackWaitAll.h"
#include "BaseBlock.h"
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class GameScene;
class Blocks : public cocos2d::Node
{
public:
    GameScene& _father;
    const int height = 9;
    const int width = 9;
    const int numOfColor = 4;
    const float LeftMargin = 20;
    const float RightMargin = 20;
    const float BottomMargin = 70;
    int getColor(int x, int y);
    bool swap(int row, int col, int direction, CallbackWaitAll_counter _counter);
    bool try_swap(int x_1, int y_1, int direction);
    bool has_solution();
    vector<tuple<int, int, bool>> Blocks::solutions();
    void Blocks::crash(int depth, shared_ptr<CallbackWaitAll> _counter);
    vector<pair<int, int>> try_crash();
    float block_size;

private:
    vector<vector<BaseBlock*>> blocks{ height,vector<BaseBlock*>(width) };

public:
    Blocks(GameScene& father);
    Blocks(GameScene& father,int height,int width,int numOfColor);
};