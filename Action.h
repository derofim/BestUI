#pragma once
#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"
#include "SkTime.h"

//SkTime

class Action {
public:
	Action();
	void update();

	void StopAction();
	void SetAction(double time);
private:
	double initstamp;
	double endstamp;
};

class ActionManage
{
public:
	
private:
	std::vector<Action *> actionlist;
};

//˳��ִ��
class Sequence :public ActionManage
{
public:
	//RunAction()
};


//ͬʱִ��
class Spawn :public ActionManage
{
public:
};

//�ظ�ִ��
class Repeat
{
public:
};

class MoveTo :public Action
{

};

class MoveBy :public Action
{

};

class DelayTime :public Action
{
public:
};

class Blink :public Action
{
public:
	Blink(double time,int nBlink);
private:
	int nTicks;
};
