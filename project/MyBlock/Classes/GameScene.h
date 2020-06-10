#pragma once

#include "cocos2d.h"
#include "Blocks.h"
#include "PostProcess.h"

// 必须要能够Layer才能接收触摸事件和进入退出事件
class GameScene : public cocos2d::Scene
{
	friend class Blocks;
public:
	static cocos2d::Scene *createScene();

	int cant_touch = 0;
	int moved = 0;

	virtual bool init();
	virtual void update(float);
	void addScore(int delta_score);	 // 分数增加

	CREATE_FUNC(GameScene); //释放内存

private:
	int _score;			  // 得分
	int _animation_score; // 显示在记分牌的分数

	cocos2d::Label *_score_label;
	cocos2d::ProgressTimer *_progress_timer; // 倒计时时间条
	cocos2d::Label *_combo_label;			 // 显示combo
	Layer* game_layer = Layer::create();

	Blocks _blocks{ *this };

	void addScoreCallback(float dt); // 分数改变动画
	void tickProgress(float dt);	 // 更新进度条

	PostProcess* m_blurX_PostProcessLayer;
	PostProcess* m_blurY_PostProcessLayer;
};
