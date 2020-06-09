#pragma once
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Blocks.h"

class Block;

// block的行列值结构体
struct BlockPos
{
	int row;
	int col;
};

// block的逻辑结构体
struct BlockProto
{
	int type;
	bool marked;
};

// 必须要能够Layer才能接收触摸事件和进入退出事件
class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene *createScene();

	int cant_touch = 0;
	int moved = 0;

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(GameScene); //释放内存

private:
	int _score;			  // 得分
	int _animation_score; // 显示在记分牌的分数

	cocos2d::Label *_score_label;
	cocos2d::ProgressTimer *_progress_timer; // 倒计时时间条
	cocos2d::Label *_combo_label;			 // 显示combo

	Blocks _blocks{*this};

	void fillGameBoard(int row, int col); // 随机填充游戏地图，直到确保没有可自动消除的
	//void drawGameBoard(); // 绘制游戏地图界面
	void dropBlocks(float dt); // 下降block
	void fillVacantBlocks();   // 填充空白

	void addScore(int delta_score);	 // 分数增加
	void addScoreCallback(float dt); // 分数改变动画
	void tickProgress(float dt);	 // 更新进度条
};

#endif // __HELLOWORLD_SCENE_H__