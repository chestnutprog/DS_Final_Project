#include <bits/stdc++.h>
#include "Blocks.h"
#include "BaseBlock.h"
using namespace std;
int x_1, x_2, y_1, y_2;
int main()
{
    Blocks blocks;
    while (1)
    {
        for (int i = 0; i < blocks.height; i++)
        {
            for (int j = 0; j < blocks.width; j++)
                cout << blocks.getColor(i, j) << " ";
            cout << endl;
        }
        cin >> x_1 >> y_1 >> x_2 >> y_2;
        auto r = blocks.swap(x_1, y_1, x_2, y_2);
        cout << (r ? "Could crash!" : "Can't crash!") << endl;
        auto crashed = blocks.crash();
        cout << crashed.size() << " blocks has been crashed!" << endl;
    }
}