#pragma once
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class Block;

// block������ֵ�ṹ��
struct BlockPos
{
	int row;
	int col;
};

// block���߼��ṹ��
struct BlockProto
{
	int type;
	bool marked;
};


// ����Ҫ�ܹ�Layer���ܽ��մ����¼��ͽ����˳��¼�
class GameScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	virtual void update(float dt);

	// �������
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	CREATE_FUNC(GameScene);//�ͷ��ڴ�

private:
	std::vector<std::vector<BlockProto>> _game_board; // Block���У��������ͺ�״̬
	BlockPos _start_pos, _end_pos; // ��������ʼ����ֹλ�ñ��

	bool _is_moving; // �Ƿ����ƶ���
	bool _is_can_touch; // �Ƿ�ɴ���
	int _is_can_elimate; // �Ƿ��������

	BlockPos getBlockPosByCoordinate(float x, float y);// ��ȡblock����Ϸ��ͼ������

	int _score; // �÷�
	cocos2d::Label* _score_label; 
	int _animation_score; // ��ʾ�ڼǷ��Ƶķ���
	cocos2d::ProgressTimer* _progress_timer; // ����ʱʱ����
	cocos2d::Label* _combo_label; // ��ʾcombo

	void fillGameBoard(int row, int col); // ��������Ϸ��ͼ��ֱ��ȷ��û�п��Զ�������
	void drawGameBoard(); // ������Ϸ��ͼ����
	void dropBlocks(float dt); // �½�block
	void fillVacantBlocks(); // ���հ�

	void swapBlocks(BlockPos p1, BlockPos p2);// ��������block

	bool hasEliminate(); // ����Ƿ���������ֵĿ��Ա�������block
	std::vector<BlockPos> getEliminateSet(); // ��ÿ�������block����
	void batchEliminate(const std::vector<BlockPos>& eliminate_list); // ִ������
	void delayBatchEliminate(float dt); // ִ��������ԭ����
	BlockPos checkGameHint(); // ��ȡ��Ϸ��ʾ���ж���Ϸ�Ƿ����뽩��

	void addScore(int delta_score); // ��������
	void addScoreCallback(float dt); // �����ı䶯��
	void tickProgress(float dt); // ���½�����

};

#endif // __HELLOWORLD_SCENE_H__