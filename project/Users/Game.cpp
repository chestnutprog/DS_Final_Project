#include "Game.h"

Game::Game() {};

Game::~Game() {};

Game::Game(string uname, int level) {
	record.username = uname;
	this->level = level;
	this->HP = 100;//�ͼ�ʱ�����

}

void Game::Start() {
	//��ui��ϵ��������
}

void Game::Pause() {
	//��ui��ϵ��������
}

Record Game::GameOver() {
	record.score = this->score;
	return record;
}

vector<Block> Game::Prompt() {
	vector<Block> blocks;
	//�õ�������Ĳ���
	return blocks;
}

Help Game::GetAbout() {
	Help help;

	vector<string> ss1;//����
	//string s1;
	//s1 = "��д����";
	//ss1.pushback(s1);

	vector<string> ss2;//ͼƬ·��
	//string t1;
	//t1 = "��д·��";
	//ss2.pushback(t1);

	help.ss1 = ss1;
	help.ss2 = ss2;
	return help;
}
