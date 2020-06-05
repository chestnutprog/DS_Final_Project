#pragma once
#include<vector>
#include"BaseBlock.h"
using namespace std;
class Blocks
{
    const static int width = 8;
    const static int height = 8;
    BaseBlock blocks[8][8];
public:
    int getColor(int x,int y);
    bool swap(int x1,int y1,int x2,int y2);
    vector<pair<int, int>> crash();
    Blocks();
};