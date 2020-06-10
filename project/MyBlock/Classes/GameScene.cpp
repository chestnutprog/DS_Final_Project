#include "AudioEngine.h"
#include "GameScene.h"
#include <cmath>
#include <algorithm>
#include "PostProcess.h"

const double _PI = 2 * acos(0);

USING_NS_CC;

//场景层次
const int BackGroundLevel = 0; // 背景层
const int GameBoardLevel = 1;  // 实际的游戏操作层
const int FlashLevel = 3;	   // 显示combo的动画层
const int MenuLevel = 10;	   // 菜单层

// combo文字
const std::vector<std::string> ComboTextArray{
	"Good",
	"Great",
	"Unbelievable"};

// 声音文件
const std::string BackgourndMusic = "sounds/background.mp3";
const std::string WelcomeEffect = "sounds/welcome.mp3";
const std::string PopEffect = "sounds/pop.mp3";
const std::string UnbelievableEffect = "sounds/unbelievable.mp3";


// 消除时候类型和效果
//const std::string EliminateStartImg = "images/star.png";


// 实例化主场景和层
Scene* GameScene::createScene()
{
	Scene* game_scene = GameScene::create();
	return game_scene;
}

// 初始化游戏场景
bool GameScene::init()
{
	if (!Scene::init())
		return false;


	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// 加载游戏界面背景(最底层
	Sprite *game_background = Sprite::create("images/game_bg.jpg");
	game_background->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(game_background, BackGroundLevel);

	addChild(game_layer,1);
	// 初始化游戏分数
	_score = 0;
	_animation_score = 0;

	_score_label = Label::createWithTTF(StringUtils::format("score: %d", _score), "fonts/Marker Felt.ttf", 40);
	_score_label->setTextColor(cocos2d::Color4B::YELLOW);
	_score_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.9);
	_score_label->setName("score");
	addChild(_score_label, 2); // 将记分牌加入背景层

	// 倒计时时间条
	_progress_timer = ProgressTimer::create(Sprite::create("images/progress_bar.png")); //创建一个倒计时时间条
	_progress_timer->setBarChangeRate(Point(1, 0));
	_progress_timer->setType(ProgressTimer::Type::BAR);
	_progress_timer->setMidpoint(Point(0, 1));
	_progress_timer->setPosition(Point(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.8)); //位置略低于记分牌
	_progress_timer->setPercentage(100.0);																		// 时间初始为满
	addChild(_progress_timer, BackGroundLevel);																	// 将时间条加入背景层
	schedule(CC_SCHEDULE_SELECTOR(GameScene::tickProgress), 1.0);												//以1s为单位

	// 播放音效
	AudioEngine::play2d(BackgourndMusic.c_str(), true, 1.0f);
	AudioEngine::play2d(WelcomeEffect.c_str());

	// 添加combo文字
	_combo_label = Label::createWithTTF(StringUtils::format("Ready Go"), "fonts/Marker Felt.ttf", 40);
	_combo_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	addChild(_combo_label, FlashLevel);

	_combo_label->runAction(Sequence::create(DelayTime::create(0.8), MoveBy::create(0.3, Vec2(200, 0)), CallFunc::create([=]() {
												 // 准备动画出现
												 _combo_label->setVisible(false);																 //准备动画隐藏
												 _combo_label->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2); //连击动画位置
											 }),
											 NULL));
	//动画动作序列Sequence：label->runAction(Sequence::create(MoveBy::create(1, Point(100,100)), RotateBy::create(1, 360),NULL));

	//添加触摸事件监听
	EventListenerTouchOneByOne *touch_listener = EventListenerTouchOneByOne::create();
	touch_listener->onTouchBegan = [&](Touch* touch, Event* event) {
		log("lets's see what happened");
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());

		const Size visibleSize = Director::getInstance()->getVisibleSize();
		Rect rect = Rect(_blocks.LeftMargin, _blocks.BottomMargin, visibleSize.width - _blocks.LeftMargin - _blocks.RightMargin, _blocks.block_size * _blocks.height);
		//log("%f %f %f %f", _blocks.LeftMargin, _blocks.BottomMargin, visibleSize.width - _blocks.LeftMargin - _blocks.RightMargin, _blocks.block_size * _blocks.height);
		//log("%d", cant_touch);
		if (!cant_touch && rect.containsPoint(locationInNode))
		{
			log("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			cant_touch++;
			return true;
		}
		//log("???");
		return false;
	};

	touch_listener->onTouchMoved = [&](Touch* touch, Event* event) {
		if (!moved && touch->getLocation().distance(touch->getStartLocation()) > _blocks.block_size * 0.4) {
			moved = 1;
			cant_touch++;
			auto after_swap = [&]() {cant_touch--; };
			auto direction_vec = touch->getLocation() - touch->getStartLocation();
			direction_vec.rotate(Vec2(), _PI / 4);
			auto angle = atan2(direction_vec.x, direction_vec.y);
			int direction = std::max(std::min(int(floor(angle * 2 / _PI)) + 2, 3), 0);

			int block_col = floor((touch->getStartLocation().x - _blocks.LeftMargin) / _blocks.block_size);
			int block_row = floor((touch->getStartLocation().y - _blocks.BottomMargin) / _blocks.block_size);
			_blocks.swap(block_row, block_col, direction, CallbackWaitAll::create(after_swap));
		}
	};

	touch_listener->onTouchEnded = [&](Touch* touch, Event* event) {
		cant_touch--;
		moved=0;
		log("onTouchEnded");
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touch_listener, this); // 父类的 _eventDispatcher

	m_blurX_PostProcessLayer = PostProcess::create("shader/blur.vert", "shader/blur.frag");
	m_blurX_PostProcessLayer->setAnchorPoint(Point::ZERO);
	m_blurX_PostProcessLayer->setPosition(Point::ZERO);
	
	this->addChild(m_blurX_PostProcessLayer, 5);

	// blur y layer
	m_blurY_PostProcessLayer = PostProcess::create("shader/blur.vert", "shader/blur.frag");
	m_blurY_PostProcessLayer->setAnchorPoint(Point::ZERO);
	m_blurY_PostProcessLayer->setPosition(Point::ZERO);
	this->addChild(m_blurY_PostProcessLayer, 6);

	// 默认定时器按帧更新
	scheduleUpdate();

	return true;
}



void GameScene::addScoreCallback(float dt)
{
	_animation_score += min(rand() % 10, _score - _animation_score);
	_score_label->setString(StringUtils::format("score: %d", _animation_score));

	// 分数已经逐个增加到记分牌
	if (_animation_score == _score)
		unschedule(CC_SCHEDULE_SELECTOR(GameScene::addScoreCallback));
}

void GameScene::addScore(int delta_score)
{
	// 获得记分牌，更新分数
	_score += delta_score;
	log("real_score: %d", _score);
	// 进入加分动画
	schedule(CC_SCHEDULE_SELECTOR(GameScene::addScoreCallback), 0.03);
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
		unschedule(CC_SCHEDULE_SELECTOR(GameScene::tickProgress));
	}
}


void GameScene::update(float dt) {
	// blur in X direction
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	auto& blurXstate = m_blurX_PostProcessLayer->ProgramState();
	auto blurOffset = Vec2(1.0f / visibleSize.width, 0.0);
	float blurStrength = 1.0f;
	blurXstate.setUniform(blurXstate.getUniformLocation("u_blurOffset"), &blurOffset,sizeof(blurOffset));
	blurXstate.setUniform(blurXstate.getUniformLocation("u_blurStrength"), &blurStrength,sizeof(blurStrength));

	m_blurX_PostProcessLayer->draw(game_layer);

	// blur in Y direction
	auto blurOffsetY = Vec2(0.0, 1.0f / visibleSize.height);
	auto& blurYstate = m_blurY_PostProcessLayer->ProgramState();
	blurYstate.setUniform(blurYstate.getUniformLocation("u_blurOffset"), &blurOffsetY, sizeof(blurOffsetY));
	blurYstate.setUniform(blurYstate.getUniformLocation("u_blurStrength"), &blurStrength, sizeof(blurStrength));

	m_blurY_PostProcessLayer->draw(m_blurX_PostProcessLayer);
}