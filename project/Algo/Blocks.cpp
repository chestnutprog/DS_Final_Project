#include "Blocks.h"
using namespace std;
bool Blocks::swap(int x1, int y1, int x2, int y2)
{
    // if after swap (x,y) could crash, then swap (x,y) 
    // else don't do operations on blocks.
}
vector<pair<int, int>> Blocks::crash()
{
    // Return all of crashed blocks,
    // And let the blocks above fall,
    // then put new block in empty position.
}

int Blocks::getColor(int x,int y){
    return blocks[x][y].color;
}