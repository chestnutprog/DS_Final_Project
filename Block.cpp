#include "block.h"

USING_NS_CC;



bool Block::init()
{
	if (!Sprite::init())
		return false;

	// ��ʼ��
	block_type = -1;

	return true;
}

void Block::appear()
{
	// ��ʱ��ʾ����
	setVisible(false);
	scheduleOnce(schedule_selector(Block::appearSchedule), 0.3);
}

void Block::appearSchedule(float dt)
{
	setVisible(true);
	setScale(0.5);

	ScaleTo* scale_to = ScaleTo::create(0.2, 1.0);
	runAction(scale_to);
}

void Block::vanish()
{
	// ��ʱ��ʾ����
	ScaleTo* scale_to = ScaleTo::create(0.2, 0.5);
	CallFunc* funcall = CallFunc::create(this, callfunc_selector(Block::vanishCallback));
	Sequence* sequence = Sequence::create(DelayTime::create(0.2), scale_to, funcall, NULL);
	runAction(sequence);
}

void Block::vanishCallback()
{
	removeFromParent();
}




