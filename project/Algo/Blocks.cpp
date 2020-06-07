#include <bits/stdc++.h>
#include "Blocks.h"
using namespace std;
bool Blocks::try_swap(int x_1, int y_1, int x_2, int y_2)
{
    // if after swap (x,y) could crash, then swap (x,y)
    // else don't do operations on blocks.
    if (!(((x_1 == x_2) && (abs(y_1 - y_2) == 1)) || ((y_1 == y_2) && (abs(x_1 - x_2) == 1))))
        std::swap(blocks[x_1][y_1], blocks[x_2][y_2]);
    vector<pair<int, int>> result = try_crash();
    std::swap(blocks[x_1][y_1], blocks[x_2][y_2]);
    return !result.empty();
}
bool Blocks::swap(int x_1, int y_1, int x_2, int y_2)
{
    // if after swap (x,y) could crash, then swap (x,y)
    // else don't do operations on blocks.
    if (!(((x_1 == x_2) && (abs(y_1 - y_2) == 1)) || ((y_1 == y_2) && (abs(x_1 - x_2) == 1))))
        std::swap(blocks[x_1][y_1], blocks[x_2][y_2]);
    vector<pair<int, int>> result = try_crash();
    if (result.empty())
    {
        std::swap(blocks[x_1][y_1], blocks[x_2][y_2]);
        return 0;
    }
    return 1;
}
vector<pair<int, int>> Blocks::try_crash()
{
    bool v[height][width];
    int c[height][width];

    memset(v, 0, sizeof(v));
    memset(c, 0, sizeof(c));

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (j == 0)
            {
                c[i][j] = 1;
            }
            else
            {
                c[i][j] = (blocks[i][j].color == blocks[i][j - 1].color) ? c[i][j - 1] + 1 : 1;
            }

    for (int i = 0; i < height; i++)
    {
        int flag = 0;
        for (int j = width - 1; j >= 0; j--)
        {
            if (!flag)
            {
                if (c[i][j] >= 3)
                    flag = 1, v[i][j] = 1;
            }
            else
            {
                if ((blocks[i][j].color != blocks[i][j + 1].color))
                    flag = 0;
                else
                    v[i][j] = 1;
            }
        }
    }

    memset(c, 0, sizeof(c));
    for (int j = 0; j < width; j++)
        for (int i = 0; i < height; i++)
            if (i == 0)
            {
                c[i][j] = 1;
            }
            else
            {
                c[i][j] = (blocks[i][j].color == blocks[i - 1][j].color) ? c[i - 1][j] + 1 : 1;
            }

    for (int j = 0; j < width; j++)
    {
        int flag = 0;
        for (int i = height - 1; i >= 0; i--)
        {
            if (!flag)
            {
                if (c[i][j] >= 3)
                    flag = 1, v[i][j] = 1;
            }
            else
            {
                if ((blocks[i][j].color != blocks[i + 1][j].color))
                    flag = 0;
                else
                    v[i][j] = 1;
            }
        }
    }

    vector<pair<int, int>> result;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (v[i][j])
                result.push_back(make_pair(i, j));
    return result;
}

bool Blocks::has_solution()
{
    // Return is thers any swap that could make a crash.
    bool hasSolution;
    //traverse each of the element to swap
    for (int i = 0; i < width - 1; i++)
    {
        for (int j = 0; j < height - 1; j++)
        {
            if (try_swap(blocks[i][j], blocks[i + 1][j])) // if it can swap with right
                return true;
            if (try_swap(blocks[i][j], blocks[i][j + 1])) // if it can swap with down
                return true;
        }
    }
    return false;
}

// Block (oldposition x, y) (new position x,y)
// this is a helper function to calc animation;
vector<tuple<BaseBlock *, int, int, int, int>> Blocks::crash()
{
    vector<pair<int, int>> result = try_crash();
    for (auto i : result)
        blocks[i.first][i.second] = BaseBlock();

    vector<tuple<BaseBlock *, int, int, int, int>> moves;
    // Return all of crashed blocks,
    // And let the blocks above fall,
    // then put new block in empty position.
    return moves;
}

int Blocks::getColor(int x, int y)
{
    return blocks[x][y].color;
}
