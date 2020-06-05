#pragma once
#include <cstdlib>
class BaseBlock{
public:
    int color;
    static const int totalColors = 5;
    BaseBlock(){
        color = rand() % totalColors;
    }
};