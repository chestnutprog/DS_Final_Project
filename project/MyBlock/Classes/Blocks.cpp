#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <complex>
#include "Blocks.h"
#include "GameScene.h"
#include "CallbackWaitAll.h"
using namespace std;
// left up right down (in ui)
// left down right up (in [])
const pair<int, int> directions[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
bool Blocks::try_swap(int row, int col, int direction)
{
    // if after swap (x,y) could crash, then swap (x,y)
    // else don't do operations on blocks.
    auto &&op = directions[direction];
    if (row + op.first < 0 || row + op.first >= height || col + op.second < 0 || col + op.second >= width)
        return false;
    std::swap(blocks[row][col], blocks[row + op.first][col + op.second]);
    vector<pair<int, int>> result = try_crash();
    std::swap(blocks[row][col], blocks[row + op.first][col + op.second]);
    return !result.empty();
}
bool Blocks::swap(int row, int col, int direction)
{
    // if after swap (x,y) could crash, then swap (x,y)
    // else don't do operations on blocks.
    auto&& op = directions[direction];
    if (row + op.first < 0 || row + op.first >= height || col + op.second < 0 || col + op.second >= width)
        return false;

    log("swap %d %d %d", row, col, direction);

    ((GameScene&)_father).cant_touch++;
    auto&& result = try_swap(row, col, direction);

    {
        auto counter = make_shared<CallbackWaitAll>([&]() {crash(); });
        if (result)
        {
            std::swap(blocks[row][col], blocks[row + op.first][col + op.second]);
            blocks[row + op.first][col + op.second]->swapTo({ row + op.first, col + op.second }, true, counter);
            blocks[row][col]->swapTo({ row , col }, true, counter);
        }
        else {
            blocks[row + op.first][col + op.second]->swapTo({ row , col }, false, counter);
            blocks[row][col]->swapTo({ row + op.first, col + op.second }, false, counter);
        }
        ((GameScene&)_father).cant_touch--;
    }

    return 0;//return result;
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
                c[i][j] = (blocks[i][j]->color == blocks[i][j - 1]->color) ? c[i][j - 1] + 1 : 1;
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
                if ((blocks[i][j]->color != blocks[i][j + 1]->color))
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
                c[i][j] = (blocks[i][j]->color == blocks[i - 1][j]->color) ? c[i - 1][j] + 1 : 1;
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
                if ((blocks[i][j]->color != blocks[i + 1][j]->color))
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

// Return is thers any swap that could make a crash.
bool Blocks::has_solution()
{
    // traverse each of the element to swap
    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 0; j < width - 1; j++)
        {
            if (try_swap(i, j, 1)) // if it can swap with down
                return true;
            if (try_swap(i, j, 3)) // if it can swap with right
                return true;
        }
    }
    return false;
}

// X , Y , direction
vector<tuple<int, int, bool>> Blocks::solutions()
{
    vector<tuple<int, int, bool>> solutions;
    //traverse each of the element to swap
    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 0; j < width - 1; j++)
        {
            if (try_swap(i, j, 1)) // if it can swap with down
                solutions.emplace_back(i, j, false);
            if (try_swap(i, j, 3)) // if it can swap with right
                solutions.emplace_back(i, j, true);
        }
    }
    return solutions;
}

// Block (oldposition x, y) (new position x,y)
// this is a helper function to calc animation;
void Blocks::crash()
{

    auto _after_vanish = [&]() {
        function<void(void)> _cb = [&]() {
            if (try_crash().size()) {
                crash();
            }
        };
        {
            auto counter = make_shared<CallbackWaitAll>(_cb);
            BaseBlock* new_blocks[width][height];
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    new_blocks[j][i] = blocks[i][j];
            int pos[width];
            for (int j = 0; j < width; j++)
                pos[j] = std::remove(new_blocks[j], new_blocks[j] + height, nullptr) - new_blocks[j];
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++) {
                    if (i >= pos[j])
                        new_blocks[j][i] = BaseBlock::create(*this, rand() % 6, { i,j }), _father.addChild(new_blocks[j][i], 1);
                    if (new_blocks[j][i] != blocks[i][j]) {
                        blocks[i][j] = new_blocks[j][i];
                        blocks[i][j]->dropTo({ i,j }, counter);
                    }
                }
        }
    };
    {
        auto counter = make_shared<CallbackWaitAll>(_after_vanish);
        vector<pair<int, int>> result = try_crash();
        for (auto i : result) {
            blocks[i.first][i.second]->vanish(counter); //= BaseBlock::create(*this, 1, {i.first, i.second});
            blocks[i.first][i.second] = nullptr;
        }
    }


}

int Blocks::getColor(int x, int y)
{
    return blocks[x][y]->color;
}
