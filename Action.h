#pragma once
//#include "UIWidget.h"

#include "GrContext.h"
#include "SkTime.h"

//SkTime
//
//class Action {
//public:
//	Action();
//	void update();
//
//	void StopAction();
//	void SetAction(double time);
//private:
//	double initstamp;
//	double endstamp;
//};
//

//
////顺序执行
//class Sequence :public ActionManage
//{
//public:
//	//RunAction()
//};
//
//
////同时执行
//class Spawn :public ActionManage
//{
//public:
//};
//
////重复执行
//class Repeat
//{
//public:
//};
//
//class MoveTo :public Action
//{
//
//};
//
//class MoveBy :public Action
//{
//
//};
//
//class DelayTime :public Action
//{
//public:
//};
//
//class Blink :public Action
//{
//public:
//	Blink(double time,int nBlink);
//private:
//	int nTicks;
//};


//typedef std::function<void(UIWidget *p)> CallBackFun;

class UIWidget;
class Action {
public:

	enum ActState{
		NotStart,
		Start,
		Stop
	};
	Action();
	//void SetStopActionCallBack();
	virtual void StartAction();
	virtual void StopAction();

	virtual void update() = 0;

	void SetRunTime(double ftime)
	{
		runstamp = ftime * 1000;
	}
	double GetRunTime()
	{
		return runstamp;
	}
	double GetEndStamp()
	{
		return endstamp;
	}
	UIWidget* GetWidget()
	{
		return pWidget;
	}
	void SetWidget(UIWidget *pUi)
	{
		pWidget = pUi;
	}

	ActState GetActionState()
	{
		return st;
	}
private:
	double endstamp;
	double runstamp;
	UIWidget *pWidget;
	ActState st;
  
};

class ActionManage
{
public:
	ActionManage();
	void AddAction(Action *act, UIWidget *pWidget);
	void UpdateAllAction();
private:
	std::vector<Action *> actionlist;
};

class Blink :public Action
{
public:
	Blink(double runtime, int nBlink);
	void StartAction();
	void StopAction();
	void update() override;
private:
	int nBlinkTick;
	double fLastStamp;
	bool bVisble;
	
};
