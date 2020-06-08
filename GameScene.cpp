#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "block.h"

USING_NS_CC;
using namespace CocosDenshion;

//�������
const int BackGroundLevel = 0; // ������
const int GameBoardLevel = 1;  // ʵ�ʵ���Ϸ������
const int FlashLevel = 3; // ��ʾcombo�Ķ�����
const int MenuLevel = 5; // �˵���

//blockͼ��
const std::vector<std::string> BlockImgArray{
	"images/b1.png",
	"images/b2.png",
	"images/b3.png",
	"images/b4.png",
	"images/b5.png",
	"images/b6.png"
};

// combo����
const std::vector<std::string> ComboTextArray{
	"Good",
	"Great",
	"Unbelievable"
};

// �����ļ�
const std::string BackgourndMusic = "sounds/background.mp3";
const std::string WelcomeEffect = "sounds/welcome.mp3";
const std::string PopEffect = "sounds/pop.mp3";
const std::string UnbelievableEffect = "sounds/unbelievable.mp3";

// ����������λ
const int ScoreUnit = 10;

// ����ʱ�����ͺ�Ч��
const int BlockEliminateType = 10;//blockΪ����״̬ʱ��type
const std::string EliminateStartImg = "images/star.png";

// ����߾�
const float LeftMargin = 20;
const float RightMargin = 20;
const float BottonMargin = 70;

// block������
const int RowNum = 8;
const int ColNum = 8;

// �����ѽ���block
const int EliminateInitFlag = 0;
const int EliminateOneReadyFlag = 1;
const int EliminateTwoReadyFlag = 2;

//������block
int getRandomSpriteIndex(int len)
{
	return rand() % len;
}

// ʵ�����������Ͳ�
Scene* GameScene::createScene()
{
	Scene* game_scene = Scene::create();
	Layer* game_layer = GameScene::create();
	game_scene->addChild(game_layer);
	return game_scene;
}


// ��ʼ����Ϸ����
bool GameScene::init()
{
	if (!Layer::init())
		return false;


	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ������Ϸ���汳��(��ײ�
	Sprite* game_background = Sprite::create("images/game_bg.jpg");
	game_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(game_background, BackGroundLevel);

	// ��ʼ����Ϸ��ͼ
	for (int i = 0; i < RowNum; i++)
	{
		std::vector<BlockProto> line_blocks;
		for (int j = 0; j < ColNum; j++)
		{
			BlockProto block_proto;
			block_proto.type = BlockEliminateType; // ��ʼ���ó�����״̬
			block_proto.marked = false;

			line_blocks.push_back(block_proto);//����block�������
		}
		_game_board.push_back(line_blocks);
	}

	// ������Ϸ��ͼ
	drawGameBoard();


	// ��ʼ����Ϸ����
	_score = 0;
	_animation_score = 0;
	
	_score_label = Label::createWithTTF(StringUtils::format("score: %d", _score), "fonts/Marker Felt.ttf", 20);
	_score_label->setTextColor(cocos2d::Color4B::YELLOW);
	_score_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.9);
	_score_label->setName("score");
	addChild(_score_label, BackGroundLevel);// ���Ƿ��Ƽ��뱳����


	// ��ʼ�����Ŀ�ʼ�ͽ�������
	_start_pos.row = -1;
	_start_pos.col = -1;

	_end_pos.row = -1;
	_end_pos.col = -1;


	// ��ʼ�ƶ�״̬
	_is_moving = false;
	_is_can_touch = true;
	_is_can_elimate = 0; // 0, 1, 2�����ȼ���0Ϊ��ʼ��1��ʾһ������ready��2��ʾ��������ready����������


	// ����ʱʱ����
	_progress_timer = ProgressTimer::create(Sprite::create("images/progress_bar.png"));//����һ������ʱʱ����
	_progress_timer->setBarChangeRate(Point(1, 0));
	_progress_timer->setType(ProgressTimer::Type::BAR);
	_progress_timer->setMidpoint(Point(0, 1));
	_progress_timer->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8));//λ���Ե��ڼǷ���
	_progress_timer->setPercentage(100.0); // ʱ���ʼΪ��
	addChild(_progress_timer, BackGroundLevel);// ��ʱ�������뱳����
	schedule(schedule_selector(GameScene::tickProgress), 1.0);//��1sΪ��λ


	// ������Ч
	SimpleAudioEngine::getInstance()->playBackgroundMusic(BackgourndMusic.c_str(), true);
	SimpleAudioEngine::getInstance()->playEffect(WelcomeEffect.c_str());


	// ���combo����
	_combo_label = Label::createWithTTF(StringUtils::format("Ready Go"), "fonts/Marker Felt.ttf", 40);
	_combo_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(_combo_label, FlashLevel);

	_combo_label->runAction(Sequence::create(DelayTime::create(0.8), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
		// ׼����������
		_combo_label->setVisible(false);//׼����������
		_combo_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);//��������λ��
		}), NULL));
	//������������Sequence��label->runAction(Sequence::create(MoveBy::create(1, Point(100,100)), RotateBy::create(1, 360),NULL));


	// ��Ӵ����¼�����
	EventListenerTouchOneByOne* touch_listener = EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this); // ����� _eventDispatcher


	// Ĭ�϶�ʱ����֡����
	scheduleUpdate();

	return true;
}
BlockPos GameScene::getBlockPosByCoordinate(float x, float y)
{
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;//block�ı߳�

	float row = (y - BottonMargin) / block_size;
	float col = (x - LeftMargin) / block_size;

	BlockPos pos;
	pos.row = row;
	pos.col = col;

	return pos;
}

// ��������Ϸ��ͼ��ֱ��ȷ��û�п��Զ�������
void GameScene::fillGameBoard(int row, int col)
{
	// �жϱ߽�
	if (row == -1 || row == RowNum || col == -1 || col == ColNum)
		return;

	// �����������
	int random_type = getRandomSpriteIndex(BlockImgArray.size());

	// ���
	if (_game_board[row][col].type == BlockEliminateType)
	{
		_game_board[row][col].type = random_type;

		// ���û���������������Χ���
		if (!hasEliminate())
		{
			// �ĸ�����ݹ����
			fillGameBoard(row + 1, col);
			fillGameBoard(row - 1, col);
			fillGameBoard(row, col - 1);
			fillGameBoard(row, col + 1);
		}
		else
			_game_board[row][col].type = BlockEliminateType; // ��ԭ
	}
}
bool GameScene::hasEliminate()
{
	// ����ͬ�Ҳ������Ϊ��������������block������true
	bool has_elminate = false;
	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
		{
			if (_game_board[i][j].type != BlockEliminateType)
			{
				// �ж������Ƿ���ͬ
				if (i - 1 >= 0
					&& _game_board[i - 1][j].type != BlockEliminateType
					&& _game_board[i - 1][j].type == _game_board[i][j].type
					&& i + 1 < RowNum
					&& _game_board[i + 1][j].type != BlockEliminateType
					&& _game_board[i + 1][j].type == _game_board[i][j].type)
				{
					has_elminate = true;
					break;
				}

				// �ж������Ƿ���ͬ
				if (j - 1 >= 0
					&& _game_board[i][j - 1].type != BlockEliminateType
					&& _game_board[i][j - 1].type == _game_board[i][j].type
					&& j + 1 < ColNum
					&& _game_board[i][j - 1].type != BlockEliminateType
					&& _game_board[i][j + 1].type == _game_board[i][j].type)
				{
					has_elminate = true;
					break;
				}
			}
		}

		if (has_elminate)
			break;
	}

	return has_elminate;
}
void GameScene::drawGameBoard()
{
	srand(unsigned(time(0))); 

	// ��֤��ʼû�п�������
	fillGameBoard(0, 0);


	// ������ɲ�������Ҫ��������
	bool is_need_regenerate = false;
	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
		{
			if (_game_board[i][j].type == BlockEliminateType)
			{
				is_need_regenerate = true;
			}
		}

		if (is_need_regenerate)
			break;
	}
	if (is_need_regenerate)
	{
		CCLOG("redraw game board");
		drawGameBoard();
		return;
	}


	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// ����������������Ϸ�߼���������
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;

	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
		{
			Block* block = Block::create();

			block->block_type = _game_board[i][j].type;
			block->setTexture(BlockImgArray[block->block_type]); // ����ͼ��	
			block->setContentSize(Size(block_size, block_size)); // ���óߴ�


			Point init_position(LeftMargin + (j + 0.5) * block_size, BottonMargin + (i + 0.5) * block_size + 0.5 * block_size);
			block->setPosition(init_position);
			Point real_position(LeftMargin + (j + 0.5) * block_size, BottonMargin + (i + 0.5) * block_size);
			Sequence* sequence = Sequence::create(MoveTo::create(0.5, real_position), CallFunc::create([=]() {
				block->setPosition(real_position); // block�Ӹ�������߳�һ���λ�������ƶ�����Ϊ���䶯��
				}), NULL);
			block->runAction(sequence);


			std::string block_name = StringUtils::format("%d_%d", i, j);
			block->setName(block_name); // ��ÿ��block���
			addChild(block, GameBoardLevel);//��block���뱳����
		}
	}
}
void GameScene::dropBlocks(float dt)
{
	_is_can_touch = false;


	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;

	// block������հ�
	for (int j = 0; j < ColNum; j++)
	{
		std::vector<Block*> blocks;
		for (int i = RowNum - 1; i >= 0; i--)
		{
			if (_game_board[i][j].type != BlockEliminateType)
			{
				std::string block_name = StringUtils::format("%d_%d", i, j);
				Block* block = (Block*)getChildByName(block_name);
				blocks.push_back(block);
			}// ��ÿհ��Ϸ���blocks
			else
				break; 
		}

		// ֻ���м��п�ȱ�Ŵ���
		if (blocks.size() == RowNum || blocks.empty())
			continue;

		// �ȷ���һ��
		std::reverse(blocks.begin(), blocks.end());

		// ÿ���½�
		int k = 0;
		while (k < RowNum)
		{
			// �ҵ���һ���հ׵�
			if (_game_board[k][j].type == BlockEliminateType)
				break;

			k++;
		}

		for (int i = 0; i < blocks.size(); i++)
		{
			_game_board[k][j].type = blocks[i]->block_type;
			_game_board[k][j].marked = false;

			
			Point new_position(LeftMargin + (j + 0.5) * block_size, BottonMargin + (k + 0.5) * block_size);
			Sequence* sequence = Sequence::create(MoveTo::create(0.1, new_position), CallFunc::create([=]() {
				blocks[i]->setPosition(new_position); 
				}), NULL);
			blocks[i]->runAction(sequence);
			std::string new_name = StringUtils::format("%d_%d", k, j);
			blocks[i]->setName(new_name);

			k++;
		}

		while (k < RowNum)
		{
			_game_board[k][j].type = BlockEliminateType;
			_game_board[k][j].marked = true;
			k++;
		}

	}

	// �½���������հ�
	fillVacantBlocks();

	// ����
	scheduleOnce(schedule_selector(GameScene::delayBatchEliminate), 0.9);

	_is_can_touch = true;
}

void GameScene::delayBatchEliminate(float dt)
{
	// �����Ƿ����������
	auto eliminate_set = getEliminateSet();
	if (!eliminate_set.empty())
	{
		batchEliminate(eliminate_set);

		// ������ϣ���ԭflag
		_is_can_elimate = EliminateInitFlag;

		// ��ԭ������ʼ����ֹλ��
		_start_pos.row = -1;
		_start_pos.col = -1;

		_end_pos.row = -1;
		_end_pos.col = -1;
	}
}

void GameScene::fillVacantBlocks()
{
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;

	int len = BlockImgArray.size();

	srand(unsigned(time(0))); // ��ʼ�������������

	// �Ȼ�ȡ�հ�blocks
	for (int i = 0; i < RowNum; i++)
		for (int j = 0; j < ColNum; j++)
		{
			if (_game_board[i][j].type == BlockEliminateType)
			{
				int random_type = getRandomSpriteIndex(len);
				_game_board[i][j].type = random_type;
				_game_board[i][j].marked = false;

				Block* block = Block::create();

				block->block_type = _game_board[i][j].type;
				block->setTexture(BlockImgArray[block->block_type]); // ���ͼ��	
				block->setContentSize(Size(block_size, block_size)); // ���óߴ�

				Point real_position(LeftMargin + (j + 0.5) * block_size, BottonMargin + (i + 0.5) * block_size);
				block->setPosition(real_position); 

				// block���ֶ���
				block->appear();

				std::string block_name = StringUtils::format("%d_%d", i, j);
				block->setName(block_name); // ��block���
				addChild(block, GameBoardLevel);//��block���뱳����
			}
		}
}

void GameScene::swapBlocks(BlockPos p1, BlockPos p2)
{
	// ����ʱ���ɻ���
	_is_can_touch = false;

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;


	// ��ò�����blocks��Ϣ
	std::string name1 = StringUtils::format("%d_%d", p1.row, p1.col);
	std::string name2 = StringUtils::format("%d_%d", p2.row, p2.col);

	Block* block1 = (Block*)getChildByName(name1);
	Block* block2= (Block*)getChildByName(name2);

	Point position1 = block1->getPosition();
	Point position2 = block2->getPosition();

	int type1 = block1->block_type;
	int type2 = block2->block_type;

	// �ڴ��н���block����
	std::swap(_game_board[p1.row][p1.col], _game_board[p2.row][p2.col]);

	// �ƶ�����, move action���������position

	MoveTo* move_1to2 = MoveTo::create(0.2, position2);
	MoveTo* move_2to1 = MoveTo::create(0.2, position1);

	block1->runAction(Sequence::create(0, move_1to2, CallFunc::create([=]() {
		
		block1->setPosition(position2);
		// ��������
		block1->setName(name2);

		_is_can_elimate++;

		}), NULL));
	    block2->runAction(Sequence::create(0, move_2to1, CallFunc::create([=]() {

		block2->setPosition(position1);
		block2->setName(name1);

		_is_can_elimate++;

		}), NULL));

	// �������������
	if (_is_can_elimate == EliminateTwoReadyFlag)
	{
		auto eliminate_set = getEliminateSet();
		if (!eliminate_set.empty())
		{
			batchEliminate(eliminate_set);
			// ������ϣ���ԭflag
			_is_can_elimate = EliminateInitFlag;

			// ��λ������ʼ����ֹλ��
			_start_pos.row = -1;
			_start_pos.col = -1;

			_end_pos.row = -1;
			_end_pos.col = -1;
		}
	}
	// �ָ�����״̬
	_is_can_touch = true;
}
// ɨ���������block����ӵ�����������

std::vector<BlockPos> GameScene::getEliminateSet()
{
	std::vector<BlockPos> res_eliminate_list;
	// ɨ��ȷ�����������Ľ�������������Ŵ��ڻ����3��������
	for (int i = 0; i < RowNum; i++)
		for (int j = 0; j < ColNum; j++)
		{
			// �ж������Ƿ���ͬ
			if (i - 1 >= 0
				&& _game_board[i - 1][j].type == _game_board[i][j].type
				&& i + 1 < RowNum
				&& _game_board[i + 1][j].type == _game_board[i][j].type)
			{
				// ������ŵ���������
				if (!_game_board[i][j].marked && _game_board[i][j].type != BlockEliminateType)
				{
					BlockPos pos;
					pos.row = i;
					pos.col = j;

					res_eliminate_list.push_back(pos);
					_game_board[i][j].marked = true;
				}
				if (!_game_board[i - 1][j].marked && _game_board[i - 1][j].type != BlockEliminateType)
				{
					BlockPos pos;
					pos.row = i - 1;
					pos.col = j;

					res_eliminate_list.push_back(pos);
					_game_board[i - 1][j].marked = true;
				}
				if (!_game_board[i + 1][j].marked && _game_board[i + 1][j].type != BlockEliminateType)
				{
					BlockPos pos;
					pos.row = i + 1;
					pos.col = j;

					res_eliminate_list.push_back(pos);
					_game_board[i + 1][j].marked = true;
				}
			}

			// �ж������Ƿ���ͬ
			if (j - 1 >= 0
				&& _game_board[i][j - 1].type == _game_board[i][j].type
				&& j + 1 < ColNum
				&& _game_board[i][j + 1].type == _game_board[i][j].type)
			{
				// ������ŵĺ�������
				if (!_game_board[i][j].marked && _game_board[i][j].type != BlockEliminateType)
				{
					BlockPos pos;
					pos.row = i;
					pos.col = j;

					res_eliminate_list.push_back(pos);
					_game_board[i][j].marked = true;
				}
				if (!_game_board[i][j - 1].marked && _game_board[i][j - 1].type != BlockEliminateType)
				{
					BlockPos pos;
					pos.row = i;
					pos.col = j - 1;

					res_eliminate_list.push_back(pos);
					_game_board[i][j - 1].marked = true;
				}
				if (!_game_board[i][j + 1].marked && _game_board[i][j + 1].type != BlockEliminateType)
				{
					BlockPos pos;
					pos.row = i;
					pos.col = j + 1;

					res_eliminate_list.push_back(pos);
					_game_board[i][j + 1].marked = true;
				}
			}
		}

	return res_eliminate_list;
}

void GameScene::batchEliminate(const std::vector<BlockPos>& eliminate_list)
{
	// ����������Ч
	SimpleAudioEngine::getInstance()->playEffect(PopEffect.c_str());

	// �л�blockͼ�꣬��Ϊ������Ч��Ȼ����ʧ
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;

	for (auto& pos : eliminate_list)
	{
		std::string block_name = StringUtils::format("%d_%d", pos.row, pos.col);
		Block* block = (Block*)(getChildByName(block_name));
		_game_board[pos.row][pos.col].type = BlockEliminateType; // ��ǳ���������
		block->setTexture(EliminateStartImg); // ���ó�����ͼ��
		block->setContentSize(Size(block_size, block_size)); // ���óߴ�
		block->vanish();
	}


	// combo
	std::string combo_text;
	int len = eliminate_list.size();
	if (len >= 4)
		SimpleAudioEngine::getInstance()->playEffect(UnbelievableEffect.c_str());

	if (len == 4)
		combo_text = ComboTextArray[0];
	else if (len > 4 && len <= 6)
		combo_text = ComboTextArray[1];
	else if (len > 6)
		combo_text = ComboTextArray[2];
	_combo_label->setString(combo_text);
	_combo_label->setVisible(true);
	_combo_label->runAction(Sequence::create(MoveBy::create(0.5, Vec2(0, -50)), CallFunc::create([=]() {
		_combo_label->setVisible(false);
		_combo_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
		}), NULL));

	// �޸ķ���
	addScore(ScoreUnit * eliminate_list.size());

	// �½�
	scheduleOnce(schedule_selector(GameScene::dropBlocks), 0.5);

}
BlockPos GameScene::checkGameHint()
{
	/***
	���Ż��㷨
	*/
	// ȫ��ɨ�裬�����ƶ�ÿ��Ԫ�ص��ĸ����������û�п������ģ�����Ϸ���뽩��
}

void GameScene::addScoreCallback(float dt)
{
	_animation_score++;
	_score_label->setString(StringUtils::format("score: %d", _animation_score));

	// �����Ѿ�������ӵ��Ƿ���
	if (_animation_score == _score)
		unschedule(schedule_selector(GameScene::addScoreCallback));
}

void GameScene::addScore(int delta_score)
{
	// ��üǷ��ƣ����·���
	_score += delta_score;
	// ����ӷֶ���
	schedule(schedule_selector(GameScene::addScoreCallback), 0.03);
}

void GameScene::tickProgress(float dt)
{
	// ����ʱ
	if (_progress_timer->getPercentage() > 0.0)
		_progress_timer->setPercentage(_progress_timer->getPercentage() - 1.0);
	else
	{
		_combo_label->setString("game over");
		_combo_label->setVisible(true);
		unschedule(schedule_selector(GameScene::tickProgress));
	}

}

void GameScene::update(float dt)
{
	// ������
	if (_start_pos.row == -1 && _start_pos.col == -1
		&& _end_pos.row == -1 && _end_pos.col == -1)
		_is_can_elimate = EliminateInitFlag;

	// ÿ֡����Ƿ񽩾�,������ǽ�������ʾ��ǰ��ʾ
	BlockPos game_hint_point = checkGameHint();
	if (game_hint_point.row == -1 && game_hint_point.col == -1)
	{
		_combo_label->setString("dead game");
		_combo_label->setVisible(true);
	}
	else
		CCLOG("game hint point: row %d, col %d", game_hint_point.row, game_hint_point.col);

}

bool GameScene::onTouchBegan(Touch* touch, Event* event)
{
	// ֻ���ڿɴ��������²ſ��Բ���
	if (_is_can_touch)
	{
		// ��¼��ʼ������block����
		_start_pos = getBlockPosByCoordinate(touch->getLocation().x, touch->getLocation().y);

		// ÿ�δ�����һ���µ��ƶ�����
		_is_moving = true;
	}

	return true;

}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

	// ֻ���ڿɴ��������²ſ��Բ���
	if (_is_can_touch)
	{
		// ���ݴ��������ķ���������block

		// �������λ�ƣ���ק���飬ע�ⷶΧ
		if (_start_pos.row > -1 && _start_pos.row < RowNum
			&& _start_pos.col > -1 && _start_pos.col < ColNum)
		{
			// ͨ���ж��ƶ��������λ�����ĸ���Χ�������ƶ��ķ���
			Vec2 cur_loacation = touch->getLocation();

			// ������ֻ��ȡһ�Σ���ֹ��񻥻�
			if (_end_pos.row == -1 && _end_pos.col == -1
				|| _end_pos.row == _start_pos.row && _end_pos.col == _start_pos.col)
				_end_pos = getBlockPosByCoordinate(cur_loacation.x, cur_loacation.y);

			if (_is_moving)
			{
				// ����ƫ�Ʒ��򽻻�����
				bool is_need_swap = false;

				if (_start_pos.col + 1 == _end_pos.col && _start_pos.row == _end_pos.row) // ˮƽ����
					is_need_swap = true;
				else if (_start_pos.col - 1 == _end_pos.col && _start_pos.row == _end_pos.row) // ˮƽ����
					is_need_swap = true;
				else if (_start_pos.row + 1 == _end_pos.row && _start_pos.col == _end_pos.col) // ��ֱ����
					is_need_swap = true;
				else if (_start_pos.row - 1 == _end_pos.row && _start_pos.col == _end_pos.col) // ��ֱ����
					is_need_swap = true;

				if (is_need_swap

					/***
					&&!TrySwap(p1, p2)) 
						*/
					){
					// ִ�н���

					swapBlocks(_start_pos, _end_pos);

					// �ع���ƶ�״̬
					_is_moving = false;
				}
			}

		}
	}
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
	_is_moving = false;
}

void GameScene::onEnter()
{
	Layer::onEnter();
}

void GameScene::onExit()
{
	Layer::onExit();
}