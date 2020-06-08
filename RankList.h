#ifndef __RANK_LIST_H__
#define __RANK_LIST_H__

#include "cocos2d.h"

class Player;

// 玩家信息
struct PlayerInf
{
	char name[255];
	int p_score;
};


class RankScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(RankScene);

private:
	std::vector<PlayerInf> _player_list; // 存储玩家信息
	int player_num; // 存储的玩家总人数
};

#endif