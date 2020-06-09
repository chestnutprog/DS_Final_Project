#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "cocos2d.h"

class Block : public cocos2d::Sprite
{
public:
	virtual bool init();

	int block_type; // 图标类型

	void appear(); // 出现动画
	void appearSchedule(float dt); 

	void vanish(); // 消失动画
	void vanishCallback(); 

	CREATE_FUNC(Block);
};

#endif

