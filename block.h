#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "cocos2d.h"

class Block : public cocos2d::Sprite
{
public:
	virtual bool init();

	int block_type; // ͼ������

	void appear(); // ���ֶ���
	void appearSchedule(float dt); 

	void vanish(); // ��ʧ����
	void vanishCallback(); 

	CREATE_FUNC(Block);
};

#endif

