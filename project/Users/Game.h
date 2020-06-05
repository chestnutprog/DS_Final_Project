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
	vector<string> ss1;//����ǹ���Ҫ˵�Ĺ�����Ϣ��һ������
	vector<string> ss2;//����ǹ���Ҫչʾ��ͼƬ·����һ������
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
	vector<Block> Prompt();//��ʾ�ĺ����������������Խ����������Ŀ�
	Help GetAbout();//�õ�����
private:
	int HP;
	int score;
	int level;
	Record record;
};

