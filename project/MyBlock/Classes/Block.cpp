#include "block.h"

USING_NS_CC;



bool Block::init()
{
	if (!Sprite::init())
		return false;

	// 初始化
	block_type = -1;

	return true;
}

void Block::appear()
{
	setVisible(false);
	scheduleOnce(CC_SCHEDULE_SELECTOR(Block::appearSchedule), 0.3);
}

void Block::appearSchedule(float dt)
{
	setVisible(true);
	setScale(0.5);
	runAction(ScaleTo::create(0.2, 1.0));
}

void Block::vanish()
{
	// 延时显示动画
	ScaleTo* scale_to = ScaleTo::create(0.2, 0.5);
	CallFunc* funcall = CallFunc::create([&]() {vanishCallback(); });
	Sequence* sequence = Sequence::create(DelayTime::create(0.2), scale_to, funcall, NULL);
	runAction(sequence);
}

void Block::vanishCallback()
{
	removeFromParent();
}




