#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Record {
	string username;
	int score;
};

struct Block {
	int row;
	int col;
	int color;
};

struct Help {
	vector<string> ss1;//这个是公告要说的关于信息的一个数组
	vector<string> ss2;//这个是公告要展示的图片路径的一个数组
};

class Game
{
public:
	Game();
	~Game();
	Game(string uname, int level);
	void Start();
	void Pause();
	Record GameOver();
	vector<Block> Prompt();//提示的函数，返回两个可以交换后消除的块
	Help GetAbout();//得到帮助
private:
	int HP;
	int score;
	int level;
	Record record;
};

