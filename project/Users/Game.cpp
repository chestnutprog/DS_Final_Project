#include "Game.h"

Game::Game() {};

Game::~Game() {};

Game::Game(string uname, int level) {
	record.username = uname;
	this->level = level;
	this->HP = 100;//和计时器差不多

}

void Game::Start() {
	//与ui联系，下周做
}

void Game::Pause() {
	//与ui联系，下周做
}

Record Game::GameOver() {
	record.score = this->score;
	return record;
}

vector<Block> Game::Prompt() {
	vector<Block> blocks;
	//得到两个块的操作
	return blocks;
}

Help Game::GetAbout() {
	Help help;

	vector<string> ss1;//内容
	//string s1;
	//s1 = "填写内容";
	//ss1.pushback(s1);

	vector<string> ss2;//图片路径
	//string t1;
	//t1 = "填写路径";
	//ss2.pushback(t1);

	help.ss1 = ss1;
	help.ss2 = ss2;
	return help;
}
