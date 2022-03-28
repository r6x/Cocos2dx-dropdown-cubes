#include "GraphicsScene.h"

USING_NS_CC;

int globFieldHeight = 10;
int globFieldWidth = 20;
int globColorsCount = 4;

Scene* GraphicsScene::CreateScene() {
	auto scene = Scene::create();
	auto layer = GraphicsScene::create();
	scene->addChild(layer);
	return scene;
}

bool GraphicsScene::init() {
	if (!Layer::init()) {
		return false;
	}
	//for same generating each time
	//long seed = (long)__TIME__;
	srand(rand());

    backgroundSprite = Sprite::create("background.png");
	backgroundSprite->setAnchorPoint(Vec2(0, 0));
	backgroundSprite->setPosition(0, 0);

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	
	fieldHeight = globFieldHeight;
	fieldWidth = globFieldWidth;
	colorsCount = globColorsCount;

	//calculate position
	if (fieldHeight < fieldWidth) {
		nodeSizeInPixels = (winSize.width * 0.8) / fieldWidth;
	}
	else {
		nodeSizeInPixels = (winSize.height * 0.8) / fieldHeight;
		
	}
	startPointWidth = (winSize.width - (nodeSizeInPixels * fieldWidth)) / 2;
	startPointHeight = winSize.height - winSize.height / 10 - nodeSizeInPixels * fieldHeight;

	startFieldX = startPointWidth;
	startFieldY = startPointHeight;// -nodeSizeInPixels * fieldHeight + nodeSizeInPixels;

	endFieldX = startPointWidth + nodeSizeInPixels * fieldWidth;
	endFieldY = startPointHeight + nodeSizeInPixels * fieldHeight;

	spriteMultiplier = ((float)nodeSizeInPixels / 50.0);

	//buttons
	auto button = ui::Button::create("startButton.png", "selectedStartButton.png", "disabledStartButton.png");
	button->setTitleText("Start");
	button->setTitleColor(Color3B(0, 0, 0));
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			if (textFieldHeight->getString() != "") {
				globFieldHeight = stoi(textFieldHeight->getString());
			}
			if (textFieldWidth->getString() != "") {
				globFieldWidth = stoi(textFieldWidth->getString());
			}
			if (textFieldColorsCount->getString() != "") {
				globColorsCount = stoi(textFieldColorsCount->getString());
			}
			Director::getInstance()->replaceScene(GraphicsScene::CreateScene());
			cocos2d::log("button clicked");
			break;
		default:
			break;
		}
		});
	//calculate button pos
	button->setPosition(Vec2( 600, winSize.height - 50));
	button->setScale(2);
	backgroundSprite->addChild(button);

	//labels
	auto labelHeight = Label::createWithSystemFont("Height:", "Arial", 30);
	labelHeight->setPosition(100, winSize.height - 50);
	labelHeight->setTextColor(Color4B::BLACK);
	backgroundSprite->addChild(labelHeight);

	auto labelWidth = Label::createWithSystemFont("Width:", "Arial", 30);
	labelWidth->setPosition(250, winSize.height - 50);
	labelWidth->setTextColor(Color4B::BLACK);
	backgroundSprite->addChild(labelWidth);

	auto labelColorsCount = Label::createWithSystemFont("Colors:", "Arial", 30);
	labelColorsCount->setPosition(400, winSize.height - 50);
	labelColorsCount->setTextColor(Color4B::BLACK);
	backgroundSprite->addChild(labelColorsCount);

	auto labelP = Label::createWithSystemFont("Points:", "Arial", 30);
	labelP->setPosition(winSize.width - 270, winSize.height - 50);
	labelP->setTextColor(Color4B::BLACK);
	backgroundSprite->addChild(labelP);

	labelPoints = Label::createWithSystemFont(std::to_string(pointCount), "Arial", 30);
	labelPoints->setPosition(winSize.width - 200, winSize.height - 50);
	labelPoints->setTextColor(Color4B::BLACK);
	backgroundSprite->addChild(labelPoints);

	labelEndGame = Label::createWithSystemFont("Game over", "Arial", 30);
	labelEndGame->setPosition(winSize.width/2, winSize.height/2 + 150);
	labelEndGame->setTextColor(Color4B::BLACK);
	backgroundSprite->addChild(labelEndGame);
	labelEndGame->setLocalZOrder(1);
	labelEndGame->setVisible(false);

	//textFields
	textFieldHeight = ui::TextField::create(std::to_string(fieldHeight), "Arial", 30);
	textFieldHeight->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	textFieldHeight->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	textFieldHeight->setPosition(Vec2( 170, winSize.height - 50));
	textFieldHeight->setMaxLengthEnabled(true);
	textFieldHeight->setMaxLength(maxSymbolsCountTextField);
	textFieldHeight->setTextColor(Color4B(0, 0, 0, 255));
	textFieldHeight->addEventListener([&](cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type) {
		switch (type) {
		case ui::TextField::EventType::ATTACH_WITH_IME:
			textFieldHeight->setColor(Color3B::BLACK);
			break;
		case ui::TextField::EventType::DETACH_WITH_IME:
			textFieldHeight->setColor(Color3B(100, 100, 100));
			break;
		case ui::TextField::EventType::INSERT_TEXT:
			std::string str = textFieldHeight->getString();
			for (int i = str.size(); i >= 0; --i) {
				int a = str[i];
				if (a < 48 || a>57) {
					str.erase(i);
					textFieldHeight->setString(str);
				}
			}
			break;
		}
		});

	backgroundSprite->addChild(textFieldHeight);

	textFieldWidth = ui::TextField::create(std::to_string(fieldWidth), "Arial", 30);
	textFieldWidth->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	textFieldWidth->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	textFieldWidth->setPosition(Vec2(320, winSize.height - 50));
	textFieldWidth->setMaxLengthEnabled(true);
	textFieldWidth->setMaxLength(maxSymbolsCountTextField);
	textFieldWidth->setTextColor(Color4B(0, 0, 0, 255));
	textFieldWidth->addEventListener([&](cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type) {
		switch (type) {
		case ui::TextField::EventType::ATTACH_WITH_IME:
			textFieldWidth->setColor(Color3B::BLACK);
			break;
		case ui::TextField::EventType::DETACH_WITH_IME:
			textFieldWidth->setColor(Color3B(100, 100, 100));
			break;
		case ui::TextField::EventType::INSERT_TEXT:
			std::string str = textFieldWidth->getString();
			for (int i = str.size(); i >= 0; --i) {
				int a = str[i];
				if (a < 48 || a>57) {
					str.erase(i);
					textFieldWidth->setString(str);
				}
			}
			break;
		}
		});

	backgroundSprite->addChild(textFieldWidth);

	textFieldColorsCount = ui::TextField::create(std::to_string(colorsCount), "Arial", 30);
	textFieldColorsCount->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
	textFieldColorsCount->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
	textFieldColorsCount->setPosition(Vec2(470, winSize.height - 50));
	textFieldColorsCount->setMaxLengthEnabled(true);
	textFieldColorsCount->setMaxLength(1);
	textFieldColorsCount->setTextColor(Color4B(0, 0, 0, 255));
	textFieldColorsCount->addEventListener([&](cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type) {
		switch (type) {
		case ui::TextField::EventType::ATTACH_WITH_IME:
			textFieldColorsCount->setColor(Color3B::BLACK);
			break;
		case ui::TextField::EventType::DETACH_WITH_IME:
			textFieldColorsCount->setColor(Color3B(100, 100, 100));
			break;
		case ui::TextField::EventType::INSERT_TEXT:
			std::string str = textFieldColorsCount->getString();
			for (int i = str.size(); i >= 0; --i) {
				int a = str[i];
				if (a < 48 || a>52 ) {
					str.erase(i);
					textFieldColorsCount->setString(str);
				}
			}
			break;
		}
		});

	backgroundSprite->addChild(textFieldColorsCount);
	//generating random field at start
	for (int j = 0; j < fieldHeight; ++j) {
		std::vector<fieldNode> tempVector;
		fieldNode t;
		for (int i = 0; i < fieldWidth; ++i) {
			int pos = rand() % colorsCount;
			switch (pos)
			{
			case COLOR_RED:
				t.col = COLOR_RED;
				break;
			case COLOR_BLUE:
				t.col = COLOR_BLUE;
				break;
			case COLOR_GREEN:
				t.col = COLOR_GREEN;
				break;
			case COLOR_YELLOW:
				t.col = COLOR_YELLOW;
				break;
			}
			t.sprite = Sprite::create(namesArray[pos]);
			t.sprite->setAnchorPoint(Vec2(0, 0));
			t.sprite->setPosition(startPointWidth + i * nodeSizeInPixels, startPointHeight + j * nodeSizeInPixels);
			t.sprite->setScaleX(spriteMultiplier);
			t.sprite->setScaleY(spriteMultiplier);
			t.isEmpty = false;
			backgroundSprite->addChild(t.sprite);
			tempVector.push_back(t);
		}
		field.push_back(tempVector);
		tempVector.clear();
	}
	
	this->addChild(backgroundSprite, 0);

	//for mouse click debug
	/*labelTouchInfo = Label::createWithSystemFont("", "Arial", 30);

	labelTouchInfo->setColor(Color3B(0,0,0));
	labelTouchInfo->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));*/

	auto touchListener = EventListenerTouchOneByOne::create();

	touchListener->onTouchBegan = CC_CALLBACK_2(GraphicsScene::onTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GraphicsScene::onTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GraphicsScene::onTouchMoved, this);
	touchListener->onTouchCancelled = CC_CALLBACK_2(GraphicsScene::onTouchCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//this->addChild(labelTouchInfo);
	this->scheduleUpdate();
	return true;
}

bool GraphicsScene::onTouchBegan(Touch* touch, Event* event) {
	Vec2 pos = touch->getLocation();
	int a = 0;
	if (pos.x > startFieldX && pos.x< endFieldX && pos.y > startFieldY && pos.y < endFieldY) {
		isClicked = true;
		touchPosX = (int)(pos.x - startFieldX) / nodeSizeInPixels;
		touchPosY = (int)(pos.y - startFieldY) / nodeSizeInPixels;
		/* for mouse click debug
		labelTouchInfo->setString("You touched here");
		labelTouchInfo->setPosition(touch->getLocation());*/
	}
	return true;
}

void GraphicsScene::onTouchEnded(Touch* touch, Event* event) {
	cocos2d::log("touch ended");
}

void GraphicsScene::onTouchMoved(Touch* touch, Event* event) {
	cocos2d::log("touch moved");
}

void GraphicsScene::onTouchCancelled(Touch* touch, Event* event) {
	cocos2d::log("touch cancelled");
}

void GraphicsScene::update(float delta) {
	//loop
	// destroy group of nodes
	if (isClicked) {
		if (!IsEmpty(touchPosX, touchPosY)) {
			isClicked = false;
			Color color = field[touchPosY][touchPosX].col;
			pointCount += CheckGroupNode(touchPosX, touchPosY, color);
		}
	}
	labelPoints->setString(std::to_string(pointCount));
	//move non empty nodes to empty nodes
	DropDown();
	if (CheckEnd()) {
		labelEndGame->setVisible(true);
	}
}

void GraphicsScene::DropDown() {
	for (int y = 1; y < fieldHeight; ++y) {
		for (int x = 0; x < fieldWidth; ++x) {
			if (field[y - 1][x].isEmpty && !field[y][x].isEmpty) {
				field[y - 1][x].sprite->setTexture(field[y][x].sprite->getTexture());
				field[y - 1][x].col = field[y][x].col;
				field[y - 1][x].isEmpty = false;
				DestroyNode(x, y);
			}
		}
	}
}

void GraphicsScene::DestroyNode(int x, int y) {
	isClicked = false;
	field[y][x].sprite->setTexture("EmptySquare.png");
	field[y][x].isEmpty = true;
	field[y][x].col = COLOR_NONE;
}

int GraphicsScene::CheckGroupNode(int x, int y, Color col) {
	int result = 0;
	CheckGroup(x, y, col, &result);
	if (result > 1) {
		//needed restore colors after check group because it sets col member at field array to COLORS_NONE 
		RestoreColors(x, y, col);
		DestroyGroup(x, y, col);
		return result + 1;
	}
	RestoreColors(x, y, col);
	return 0;
}

int GraphicsScene::CheckGroup(int x, int y, Color col, int* res) {
	//returns 1 less than actual because of sameBlocks init with 0
	if (x + 1 != fieldWidth) {
		if (field[y][x + 1].col == col) {
			//COLOR_NONE needed as a mark to avoid cycles at for example group of block in shape of cube
			field[y][x].col = COLOR_NONE;;
			*res += CheckGroup(x + 1, y, col, res);
		}
	}
	if (x != 0) {
		if (field[y][x - 1].col == col) {
			field[y][x].col = COLOR_NONE;
			*res += CheckGroup(x - 1, y, col, res);
		}
	}
	if (y + 1 != fieldHeight) {
		if (field[y + 1][x].col == col) {
			field[y][x].col = COLOR_NONE;
			*res += CheckGroup(x, y + 1, col, res);
		}
	}
	if (y != 0) {
		if (field[y - 1][x].col == col ) {
			field[y][x].col = COLOR_NONE;
			*res += CheckGroup(x, y - 1, col, res);
		}
	}
	field[y][x].col = COLOR_NONE;
	return 1;
}

void GraphicsScene::DestroyGroup(int x, int y, Color col) {
	if (x + 1 != fieldWidth) {
		if (field[y][x + 1].col == col) {
			field[y][x].col = COLOR_NONE;
			DestroyGroup(x + 1, y, col);
		}
	}
	if (x != 0) {
		if (field[y][x - 1].col == col) {
			field[y][x].col = COLOR_NONE;
			DestroyGroup(x - 1, y, col);
		}
	}
	if (y + 1 != fieldHeight) {
		if (field[y + 1][x].col == col) {
			field[y][x].col = COLOR_NONE;
			DestroyGroup(x, y + 1, col);
		}
	}
	if (y != 0) {
		if (field[y - 1][x].col == col) {
			field[y][x].col = COLOR_NONE;
			DestroyGroup(x, y - 1, col);
		}
	}
	DestroyNode(x, y);
}

void GraphicsScene::RestoreColors(int x, int y, Color col) {
	if (x + 1 != fieldWidth) {
		if (field[y][x + 1].col == COLOR_NONE && !field[y][x + 1].isEmpty) {
			field[y][x].col = col;;
			RestoreColors(x + 1, y, col);
		}
	}
	if (x != 0) {
		if (field[y][x - 1].col == COLOR_NONE && !field[y][x - 1].isEmpty) {
			field[y][x].col = col;
			RestoreColors(x - 1, y, col);
		}
	}
	if (y + 1 != fieldHeight) {
		if (field[y + 1][x].col == COLOR_NONE && !field[y + 1][x].isEmpty) {
			field[y][x].col = col;
			RestoreColors(x, y + 1, col);
		}
	}
	if (y != 0) {
		if (field[y - 1][x].col == COLOR_NONE && !field[y - 1][x].isEmpty) {
			field[y][x].col = col;
			RestoreColors(x, y - 1, col);
		}
	}
	field[y][x].col = col;
	return;
}

bool GraphicsScene::IsEmpty(int x, int y) {
	return field[y][x].isEmpty;
}

bool GraphicsScene::CheckEnd() {
	int result = 0;
	for (int y = 0; y < fieldHeight; ++y) {
		for (int x = 0; x < fieldWidth; ++x) {
			Color col = field[y][x].col;
			if (!field[y][x].isEmpty && col != COLOR_NONE) {
				CheckGroup(x, y, col, &result);
				RestoreColors(x, y, col);
				cocos2d::log("Result: ");
				cocos2d::log(std::to_string(result).c_str());
				//check if there any moves left until we find group of block > = 3 
				if (result > 1) {
					return false;
				}
				result = 0;
			}
		}
	}
	return true;
}
