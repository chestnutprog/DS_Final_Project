#include <bits/stdc++.h>
#include "Blocks.h"
#include "BaseBlock.h"
using namespace std;
int x_1, x_2, y_1, y_2;
int main()
{
    Blocks blocks;
    vector<pair<int, int>> crashed;
    while (1)
    {
        do
        {
            for (int i = 0; i < blocks.height; i++)
            {
                for (int j = 0; j < blocks.width; j++)
                    cout << blocks.getColor(i, j) << " ";
                cout << endl;
            }
            crashed = blocks.try_crash();
            cout << (!crashed.empty() ? "Could crash!" : "Can't crash!") << endl;
            cout << crashed.size() << " blocks has been crashed! At ";
            for (auto i : crashed)
                cout << "(" << i.first << "," << i.second << ") ";
            cout << endl;
            blocks.crash();
        } while (!crashed.empty());
        cin >> x_1 >> y_1 >> x_2 >> y_2;
        auto r = blocks.swap(x_1, y_1, x_2, y_2);
    }
}