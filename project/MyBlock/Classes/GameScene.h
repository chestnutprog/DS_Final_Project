#pragma once

#include "cocos2d.h"
#include "Blocks.h"
#include "PostProcess.h"

// 必须要能够Layer才能接收触摸事件和进入退出事件
class GameScene : public cocos2d::Scene
{
	friend class Blocks;

public:
	static Scene *createScene();
	static Scene *createScene(int height, int width, int numOfColor);

	int cant_touch = 0;
	int moved = 0;

	virtual bool init();
	virtual void update(float);
	void addScore(int delta_score); // 分数增加
	void combo(int combo);

	CREATE_FUNC(GameScene); //释放内存
	static GameScene* GameScene::create(int height, int width, int numOfColor);

private:
	int _score;			  // 得分
	int _animation_score; // 显示在记分牌的分数
	bool _masked;		  // Game Layer's mask

	cocos2d::Label *_score_label;
	cocos2d::ProgressTimer *_progress_timer; // 倒计时时间条
	cocos2d::Label *_combo_label;			 // 显示combo
	Layer *game_layer = Layer::create();

	Blocks _blocks;

	void addScoreCallback(float dt); // 分数改变动画
	void tickProgress(float dt);	 // 更新进度条

	PostProcess *m_blurX_PostProcessLayer;
	PostProcess *m_blurY_PostProcessLayer;

public:
	GameScene() : GameScene(8, 8, 6) {}

	GameScene(int height, int width, int numOfColor) : _blocks(*this, height, width, numOfColor)
	{
		cocos2d::Scene::Scene();
	}
};
