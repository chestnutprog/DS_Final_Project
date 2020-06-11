#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>
#include <complex>
#include "Blocks.h"
#include "GameScene.h"
#include "CallbackWaitAll.h"
using namespace std;

// 消除分数单位
const int ScoreUnit = 100;

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
bool Blocks::swap(int row, int col, int direction, shared_ptr<CallbackWaitAll> _counter)
{
    // if after swap (x,y) could crash, then swap (x,y)
    // else don't do operations on blocks.
    auto&& op = directions[direction];
    if (row + op.first < 0 || row + op.first >= height || col + op.second < 0 || col + op.second >= width)
        return false;

    log("swap %d %d %d", row, col, direction);

    auto&& result = try_swap(row, col, direction);
    {
        auto _swap_finish_counter = CallbackWaitAll::create([_counter]() {log("whole swap finished"); _counter; });
        auto counter = CallbackWaitAll::create([=]() {log("swap finished"); crash(0,_swap_finish_counter); });

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
        
    }

    return 0;//return result;
}
vector<pair<int, int>> Blocks::try_crash()
{
    vector<vector<bool>> v(height, vector<bool>(width));
    vector<vector<int>> c(height, vector<int>(width));

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

    for (auto& row : c)
        fill(row.begin(), row.end(), 0);

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
void Blocks::crash(int depth, shared_ptr<CallbackWaitAll> _counter)
{
    auto _after_vanish = [this,depth,_counter]() {
        function<void(void)> _cb = [=]() {
            if (try_crash().size()) {
                crash(depth + 1, _counter);
            }
            else {
                log("crash finished");
            }
            _counter;
        };
        {
            log("after vanish start");
            auto counter = CallbackWaitAll::create(_cb);
            vector<vector<BaseBlock*>> new_blocks(width, vector<BaseBlock*>(height));
            //BaseBlock* new_blocks[width][height];
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    new_blocks[j][i] = blocks[i][j];
            vector<int> pos(width);
            for (int j = 0; j < width; j++)
                pos[j] = std::remove(new_blocks[j].begin(), new_blocks[j].end(), nullptr) - new_blocks[j].begin();
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++) {
                    if (i >= pos[j])
                        new_blocks[j][i] = BaseBlock::create(*this, rand() % numOfColor, { i,j }), _father.game_layer->addChild(new_blocks[j][i], 1);
                    if (new_blocks[j][i] != blocks[i][j]) {
                        blocks[i][j] = new_blocks[j][i];
                        blocks[i][j]->dropTo({ i,j }, counter);
                    }
                }
            log("after vanish finished");
        }
    };
    {
        auto counter = make_shared<CallbackWaitAll>(_after_vanish);
        vector<pair<int, int>> result = try_crash();
        int score = result.size() * (depth + 1) * ScoreUnit;
        ((GameScene&)_father).addScore(score);
        for (auto i : result) {
            blocks[i.first][i.second]->vanish(counter); //= BaseBlock::create(*this, 1, {i.first, i.second});
            blocks[i.first][i.second] = nullptr;
        }
        if (result.size() && depth > 0) {
            _father.combo(depth-1);
        }
    }


}

int Blocks::getColor(int x, int y)
{
    return blocks[x][y]->color;
}

Blocks::Blocks(GameScene& father) : Blocks(father, 8, 8, 6) {}

Blocks::Blocks(GameScene& father, int height, int width, int numOfColor) : _father{ father }, height{ height }, width{ width }, numOfColor{ numOfColor } {
    srand(time(nullptr));
    const Size visibleSize = Director::getInstance()->getVisibleSize();
    block_size = (visibleSize.width - LeftMargin - RightMargin) / width;
    do {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                blocks[i][j] = BaseBlock::create(*this, rand() % numOfColor, { i, j });
    } while (try_crash().size());
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            _father.game_layer->addChild(blocks[i][j], 1);
}