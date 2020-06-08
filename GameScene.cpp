#include "SimpleAudioEngine.h"
#include "GameScene.h"
#include "block.h"

USING_NS_CC;
using namespace CocosDenshion;

//场景层次
const int BackGroundLevel = 0; // 背景层
const int GameBoardLevel = 1;  // 实际的游戏操作层
const int FlashLevel = 3; // 显示combo的动画层
const int MenuLevel = 5; // 菜单层

//block图标
const std::vector<std::string> BlockImgArray{
	"images/b1.png",
	"images/b2.png",
	"images/b3.png",
	"images/b4.png",
	"images/b5.png",
	"images/b6.png"
};

// combo文字
const std::vector<std::string> ComboTextArray{
	"Good",
	"Great",
	"Unbelievable"
};

// 声音文件
const std::string BackgourndMusic = "sounds/background.mp3";
const std::string WelcomeEffect = "sounds/welcome.mp3";
const std::string PopEffect = "sounds/pop.mp3";
const std::string UnbelievableEffect = "sounds/unbelievable.mp3";

// 消除分数单位
const int ScoreUnit = 10;

// 消除时候类型和效果
const int BlockEliminateType = 10;//block为消除状态时的type
const std::string EliminateStartImg = "images/star.png";

// 界面边距
const float LeftMargin = 20;
const float RightMargin = 20;
const float BottonMargin = 70;

// block行列数
const int RowNum = 8;
const int ColNum = 8;

// 计数已交换block
const int EliminateInitFlag = 0;
const int EliminateOneReadyFlag = 1;
const int EliminateTwoReadyFlag = 2;

//获得随机block
int getRandomSpriteIndex(int len)
{
	return rand() % len;
}

// 实例化主场景和层
Scene* GameScene::createScene()
{
	Scene* game_scene = Scene::create();
	Layer* game_layer = GameScene::create();
	game_scene->addChild(game_layer);
	return game_scene;
}


// 初始化游戏场景
bool GameScene::init()
{
	if (!Layer::init())
		return false;


	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 加载游戏界面背景(最底层
	Sprite* game_background = Sprite::create("images/game_bg.jpg");
	game_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(game_background, BackGroundLevel);

	// 初始化游戏地图
	for (int i = 0; i < RowNum; i++)
	{
		std::vector<BlockProto> line_blocks;
		for (int j = 0; j < ColNum; j++)
		{
			BlockProto block_proto;
			block_proto.type = BlockEliminateType; // 初始化置成消除状态
			block_proto.marked = false;

			line_blocks.push_back(block_proto);//将空block加入该行
		}
		_game_board.push_back(line_blocks);
	}

	// 绘制游戏地图
	drawGameBoard();


	// 初始化游戏分数
	_score = 0;
	_animation_score = 0;
	
	_score_label = Label::createWithTTF(StringUtils::format("score: %d", _score), "fonts/Marker Felt.ttf", 20);
	_score_label->setTextColor(cocos2d::Color4B::YELLOW);
	_score_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.9);
	_score_label->setName("score");
	addChild(_score_label, BackGroundLevel);// 将记分牌加入背景层


	// 初始滑动的开始和结束坐标
	_start_pos.row = -1;
	_start_pos.col = -1;

	_end_pos.row = -1;
	_end_pos.col = -1;


	// 初始移动状态
	_is_moving = false;
	_is_can_touch = true;
	_is_can_elimate = 0; // 0, 1, 2三个等级，0为初始，1表示一个精灵ready，2表示两个精灵ready，可以消除


	// 倒计时时间条
	_progress_timer = ProgressTimer::create(Sprite::create("images/progress_bar.png"));//创建一个倒计时时间条
	_progress_timer->setBarChangeRate(Point(1, 0));
	_progress_timer->setType(ProgressTimer::Type::BAR);
	_progress_timer->setMidpoint(Point(0, 1));
	_progress_timer->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8));//位置略低于记分牌
	_progress_timer->setPercentage(100.0); // 时间初始为满
	addChild(_progress_timer, BackGroundLevel);// 将时间条加入背景层
	schedule(schedule_selector(GameScene::tickProgress), 1.0);//以1s为单位


	// 播放音效
	SimpleAudioEngine::getInstance()->playBackgroundMusic(BackgourndMusic.c_str(), true);
	SimpleAudioEngine::getInstance()->playEffect(WelcomeEffect.c_str());


	// 添加combo文字
	_combo_label = Label::createWithTTF(StringUtils::format("Ready Go"), "fonts/Marker Felt.ttf", 40);
	_combo_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(_combo_label, FlashLevel);

	_combo_label->runAction(Sequence::create(DelayTime::create(0.8), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
		// 准备动画出现
		_combo_label->setVisible(false);//准备动画隐藏
		_combo_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);//连击动画位置
		}), NULL));
	//动画动作序列Sequence：label->runAction(Sequence::create(MoveBy::create(1, Point(100,100)), RotateBy::create(1, 360),NULL));


	// 添加触摸事件监听
	EventListenerTouchOneByOne* touch_listener = EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touch_listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touch_listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this); // 父类的 _eventDispatcher


	// 默认定时器按帧更新
	scheduleUpdate();

	return true;
}
BlockPos GameScene::getBlockPosByCoordinate(float x, float y)
{
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;//block的边长

	float row = (y - BottonMargin) / block_size;
	float col = (x - LeftMargin) / block_size;

	BlockPos pos;
	pos.row = row;
	pos.col = col;

	return pos;
}

// 随机填充游戏地图，直到确保没有可自动消除的
void GameScene::fillGameBoard(int row, int col)
{
	// 判断边界
	if (row == -1 || row == RowNum || col == -1 || col == ColNum)
		return;

	// 随机生成类型
	int random_type = getRandomSpriteIndex(BlockImgArray.size());

	// 填充
	if (_game_board[row][col].type == BlockEliminateType)
	{
		_game_board[row][col].type = random_type;

		// 如果没有消除则继续向周围填充
		if (!hasEliminate())
		{
			// 四个方向递归填充
			fillGameBoard(row + 1, col);
			fillGameBoard(row - 1, col);
			fillGameBoard(row, col - 1);
			fillGameBoard(row, col + 1);
		}
		else
			_game_board[row][col].type = BlockEliminateType; // 还原
	}
}
bool GameScene::hasEliminate()
{
	// 有相同且不被标记为消除的连续三个block即返回true
	bool has_elminate = false;
	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
		{
			if (_game_board[i][j].type != BlockEliminateType)
			{
				// 判断上下是否相同
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

				// 判断左右是否相同
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

	// 保证初始没有可消除的
	fillGameBoard(0, 0);


	// 如果生成不完美需要重新生成
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

	// 添加消除对象矩阵，游戏逻辑与界面解耦
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;

	for (int i = 0; i < RowNum; i++)
	{
		for (int j = 0; j < ColNum; j++)
		{
			Block* block = Block::create();

			block->block_type = _game_board[i][j].type;
			block->setTexture(BlockImgArray[block->block_type]); // 分配图标	
			block->setContentSize(Size(block_size, block_size)); // 设置尺寸


			Point init_position(LeftMargin + (j + 0.5) * block_size, BottonMargin + (i + 0.5) * block_size + 0.5 * block_size);
			block->setPosition(init_position);
			Point real_position(LeftMargin + (j + 0.5) * block_size, BottonMargin + (i + 0.5) * block_size);
			Sequence* sequence = Sequence::create(MoveTo::create(0.5, real_position), CallFunc::create([=]() {
				block->setPosition(real_position); // block从高于自身边长一半的位置往下移动，作为掉落动画
				}), NULL);
			block->runAction(sequence);


			std::string block_name = StringUtils::format("%d_%d", i, j);
			block->setName(block_name); // 给每个block编号
			addChild(block, GameBoardLevel);//将block加入背景层
		}
	}
}
void GameScene::dropBlocks(float dt)
{
	_is_can_touch = false;


	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;

	// block下落填补空白
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
			}// 获得空白上方的blocks
			else
				break; 
		}

		// 只有中间有空缺才处理
		if (blocks.size() == RowNum || blocks.empty())
			continue;

		// 先反序一下
		std::reverse(blocks.begin(), blocks.end());

		// 每列下降
		int k = 0;
		while (k < RowNum)
		{
			// 找到第一个空白的
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

	// 下降后填补顶部空白
	fillVacantBlocks();

	// 更新
	scheduleOnce(schedule_selector(GameScene::delayBatchEliminate), 0.9);

	_is_can_touch = true;
}

void GameScene::delayBatchEliminate(float dt)
{
	// 检验是否可连续消除
	auto eliminate_set = getEliminateSet();
	if (!eliminate_set.empty())
	{
		batchEliminate(eliminate_set);

		// 消除完毕，还原flag
		_is_can_elimate = EliminateInitFlag;

		// 还原滑动起始和终止位置
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

	srand(unsigned(time(0))); // 初始化随机数发生器

	// 先获取空白blocks
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
				block->setTexture(BlockImgArray[block->block_type]); // 添加图标	
				block->setContentSize(Size(block_size, block_size)); // 设置尺寸

				Point real_position(LeftMargin + (j + 0.5) * block_size, BottonMargin + (i + 0.5) * block_size);
				block->setPosition(real_position); 

				// block出现动画
				block->appear();

				std::string block_name = StringUtils::format("%d_%d", i, j);
				block->setName(block_name); // 给block编号
				addChild(block, GameBoardLevel);//将block加入背景层
			}
		}
}

void GameScene::swapBlocks(BlockPos p1, BlockPos p2)
{
	// 交换时不可滑动
	_is_can_touch = false;

	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;


	// 获得操作的blocks信息
	std::string name1 = StringUtils::format("%d_%d", p1.row, p1.col);
	std::string name2 = StringUtils::format("%d_%d", p2.row, p2.col);

	Block* block1 = (Block*)getChildByName(name1);
	Block* block2= (Block*)getChildByName(name2);

	Point position1 = block1->getPosition();
	Point position2 = block2->getPosition();

	int type1 = block1->block_type;
	int type2 = block2->block_type;

	// 内存中交换block类型
	std::swap(_game_board[p1.row][p1.col], _game_board[p2.row][p2.col]);

	// 移动动画, move action并不会更新position

	MoveTo* move_1to2 = MoveTo::create(0.2, position2);
	MoveTo* move_2to1 = MoveTo::create(0.2, position1);

	block1->runAction(Sequence::create(0, move_1to2, CallFunc::create([=]() {
		
		block1->setPosition(position2);
		// 交换名称
		block1->setName(name2);

		_is_can_elimate++;

		}), NULL));
	    block2->runAction(Sequence::create(0, move_2to1, CallFunc::create([=]() {

		block2->setPosition(position1);
		block2->setName(name1);

		_is_can_elimate++;

		}), NULL));

	// 交换后进行消除
	if (_is_can_elimate == EliminateTwoReadyFlag)
	{
		auto eliminate_set = getEliminateSet();
		if (!eliminate_set.empty())
		{
			batchEliminate(eliminate_set);
			// 消除完毕，还原flag
			_is_can_elimate = EliminateInitFlag;

			// 复位滑动起始和终止位置
			_start_pos.row = -1;
			_start_pos.col = -1;

			_end_pos.row = -1;
			_end_pos.col = -1;
		}
	}
	// 恢复触摸状态
	_is_can_touch = true;
}
// 扫描检查可消除block，添加到可消除集合

std::vector<BlockPos> GameScene::getEliminateSet()
{
	std::vector<BlockPos> res_eliminate_list;
	// 扫描确定可以三消的结果集，横竖连着大于或等于3个就消除
	for (int i = 0; i < RowNum; i++)
		for (int j = 0; j < ColNum; j++)
		{
			// 判断上下是否相同
			if (i - 1 >= 0
				&& _game_board[i - 1][j].type == _game_board[i][j].type
				&& i + 1 < RowNum
				&& _game_board[i + 1][j].type == _game_board[i][j].type)
			{
				// 添加连着的竖向三个
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

			// 判断左右是否相同
			if (j - 1 >= 0
				&& _game_board[i][j - 1].type == _game_board[i][j].type
				&& j + 1 < ColNum
				&& _game_board[i][j + 1].type == _game_board[i][j].type)
			{
				// 添加连着的横向三个
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
	// 播放消除音效
	SimpleAudioEngine::getInstance()->playEffect(PopEffect.c_str());

	// 切换block图标，作为消除特效，然后消失
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float block_size = (visibleSize.width - LeftMargin - RightMargin) / ColNum;

	for (auto& pos : eliminate_list)
	{
		std::string block_name = StringUtils::format("%d_%d", pos.row, pos.col);
		Block* block = (Block*)(getChildByName(block_name));
		_game_board[pos.row][pos.col].type = BlockEliminateType; // 标记成消除类型
		block->setTexture(EliminateStartImg); // 设置成消除图标
		block->setContentSize(Size(block_size, block_size)); // 设置尺寸
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

	// 修改分数
	addScore(ScoreUnit * eliminate_list.size());

	// 下降
	scheduleOnce(schedule_selector(GameScene::dropBlocks), 0.5);

}
BlockPos GameScene::checkGameHint()
{
	/***
	待优化算法
	*/
	// 全盘扫描，尝试移动每个元素到四个方向，如果都没有可消除的，则游戏陷入僵局
}

void GameScene::addScoreCallback(float dt)
{
	_animation_score++;
	_score_label->setString(StringUtils::format("score: %d", _animation_score));

	// 分数已经逐个增加到记分牌
	if (_animation_score == _score)
		unschedule(schedule_selector(GameScene::addScoreCallback));
}

void GameScene::addScore(int delta_score)
{
	// 获得记分牌，更新分数
	_score += delta_score;
	// 进入加分动画
	schedule(schedule_selector(GameScene::addScoreCallback), 0.03);
}

void GameScene::tickProgress(float dt)
{
	// 倒计时
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
	// 清除标记
	if (_start_pos.row == -1 && _start_pos.col == -1
		&& _end_pos.row == -1 && _end_pos.col == -1)
		_is_can_elimate = EliminateInitFlag;

	// 每帧检查是否僵局,如果不是僵局则显示当前提示
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
	// 只有在可触摸条件下才可以操作
	if (_is_can_touch)
	{
		// 记录开始触摸的block坐标
		_start_pos = getBlockPosByCoordinate(touch->getLocation().x, touch->getLocation().y);

		// 每次触碰算一次新的移动过程
		_is_moving = true;
	}

	return true;

}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

	// 只有在可触摸条件下才可以操作
	if (_is_can_touch)
	{
		// 根据触摸滑动的方向来交换block

		// 计算相对位移，拖拽精灵，注意范围
		if (_start_pos.row > -1 && _start_pos.row < RowNum
			&& _start_pos.col > -1 && _start_pos.col < ColNum)
		{
			// 通过判断移动后触摸点的位置在哪个范围来决定移动的方向
			Vec2 cur_loacation = touch->getLocation();

			// 触摸点只获取一次，防止跨格互换
			if (_end_pos.row == -1 && _end_pos.col == -1
				|| _end_pos.row == _start_pos.row && _end_pos.col == _start_pos.col)
				_end_pos = getBlockPosByCoordinate(cur_loacation.x, cur_loacation.y);

			if (_is_moving)
			{
				// 根据偏移方向交换精灵
				bool is_need_swap = false;

				if (_start_pos.col + 1 == _end_pos.col && _start_pos.row == _end_pos.row) // 水平向右
					is_need_swap = true;
				else if (_start_pos.col - 1 == _end_pos.col && _start_pos.row == _end_pos.row) // 水平向左
					is_need_swap = true;
				else if (_start_pos.row + 1 == _end_pos.row && _start_pos.col == _end_pos.col) // 竖直向上
					is_need_swap = true;
				else if (_start_pos.row - 1 == _end_pos.row && _start_pos.col == _end_pos.col) // 竖直向下
					is_need_swap = true;

				if (is_need_swap

					/***
					&&!TrySwap(p1, p2)) 
						*/
					){
					// 执行交换

					swapBlocks(_start_pos, _end_pos);

					// 回归非移动状态
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