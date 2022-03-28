#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

enum Color
{
	COLOR_RED,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_NONE
};

struct fieldNode {
	cocos2d::Sprite* sprite;
	bool isEmpty;
	Color col;
};

class GraphicsScene : public cocos2d::Layer {

public: 
	static cocos2d::Scene* CreateScene();
	virtual bool init() override;

	virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	virtual void onTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	CREATE_FUNC(GraphicsScene);

	void update(float) override;
	
private:
	const char* namesArray[4] = {"RedSquare.png", "BlueSquare.png", "GreenSquare.png", "YellowSquare.png"};
	void DropDown();
	void DestroyNode(int x, int y);
	int CheckGroupNode(int x, int y, Color col);
	void RestoreColors(int x, int y, Color col);
	int CheckGroup(int x, int y, Color col, int* count);
	void DestroyGroup(int x, int y, Color col);
	bool IsEmpty(int x, int y);
	bool CheckEnd();

	bool isClicked = false;

	std::vector <std::vector<fieldNode>> field;

	cocos2d::Sprite * backgroundSprite;
	cocos2d::Label* labelTouchInfo;
	cocos2d::Label* labelPoints;
	cocos2d::Label* labelEndGame;
	cocos2d::ui::TextField* textFieldHeight;
	cocos2d::ui::TextField* textFieldWidth;
	cocos2d::ui::TextField* textFieldColorsCount;

	int pointCount = 0;

	int fieldWidth;
	int fieldHeight;
	int colorsCount;

	int nodeSizeInPixels = 0;

	int startFieldX = 0;
	int startFieldY = 0;

	int endFieldX = 0;
	int endFieldY = 0;

	int touchPosX = 0;
	int touchPosY = 0;

	int startPointWidth = 0;
	int startPointHeight = 0;

	float spriteMultiplier = 0;

	int maxSymbolsCountTextField = 2;
};