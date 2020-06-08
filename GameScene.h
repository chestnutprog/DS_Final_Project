#pragma once
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

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
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void update(float dt);

	// 触摸检测
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	CREATE_FUNC(GameScene);//释放内存

private:
	std::vector<std::vector<BlockProto>> _game_board; // Block阵列，储存类型和状态
	BlockPos _start_pos, _end_pos; // 滑动的起始和终止位置标号

	bool _is_moving; // 是否在移动中
	bool _is_can_touch; // 是否可触摸
	int _is_can_elimate; // 是否可以消除

	BlockPos getBlockPosByCoordinate(float x, float y);// 获取block在游戏地图的坐标

	int _score; // 得分
	cocos2d::Label* _score_label; 
	int _animation_score; // 显示在记分牌的分数
	cocos2d::ProgressTimer* _progress_timer; // 倒计时时间条
	cocos2d::Label* _combo_label; // 显示combo

	void fillGameBoard(int row, int col); // 随机填充游戏地图，直到确保没有可自动消除的
	void drawGameBoard(); // 绘制游戏地图界面
	void dropBlocks(float dt); // 下降block
	void fillVacantBlocks(); // 填充空白

	void swapBlocks(BlockPos p1, BlockPos p2);// 交换两个block

	bool hasEliminate(); // 检查是否有随机出现的可以被消除的block
	std::vector<BlockPos> getEliminateSet(); // 获得可消除的block集合
	void batchEliminate(const std::vector<BlockPos>& eliminate_list); // 执行消除
	void delayBatchEliminate(float dt); // 执行消除后还原设置
	BlockPos checkGameHint(); // 获取游戏提示，判断游戏是否陷入僵局

	void addScore(int delta_score); // 分数增加
	void addScoreCallback(float dt); // 分数改变动画
	void tickProgress(float dt); // 更新进度条

};

#endif // __HELLOWORLD_SCENE_H__