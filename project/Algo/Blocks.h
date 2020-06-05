#pragma once
#include <vector>
#include "BaseBlock.h"
using namespace std;
class Blocks
{
    BaseBlock blocks[8][8];

public:
    const static int height = 8;
    const static int width = 8;
    int getColor(int x, int y);
    bool swap(int x1, int y1, int x2, int y2);
    bool has_solution();
    vector<tuple<BaseBlock *, int, int, int, int>> crash();
    vector<pair<int, int>> try_crash();
    //Blocks();
};