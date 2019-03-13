#pragma once
//#include "UIWidget.h"

#include "GrContext.h"
#include "SkTime.h"
#include <map>

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
		Ready,
		Start,
		Stop
	};
	Action();
	//void SetStopActionCallBack();
	virtual void StartAction()=0;
	virtual void StopAction()=0;


	virtual void update() = 0;

	bool ActionIsStart();
	void ReadyAction(double fDelayTime = 0);

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
	double initstamp;
	double endstamp;
	double runstamp;
	UIWidget *pWidget;
	ActState st;
	bool bStartFlag;
  
};

class ActionManage
{
public:
	ActionManage();
	void AddAction(Action *act, UIWidget *pWidget,double fDelayTime=0);
	void UpdateAllAction();
private:
	std::vector<Action *> actionlist;
};

//typedef std::function<void(void)> TimerCallBackFun;


class Sequence /*:public ActionManage*/
{
public:
	Sequence(UIWidget *pUi,...);
	~Sequence();
private:
	ActionManage *pActionManage;
	//RunAction()
};


//class Sequence2 :public Action {
//public:
//	Sequence2(Action *act1,...);
//	void update() override {};
//	void StopAction() override {};
//	void StartAction() override {};
//};


class Blink :public Action
{
public:
	Blink(double runtime, int nBlink);
	void StartAction() override;
	void StopAction()override;
	void update() override;
private:
	int nBlinkTick;
	double fLastStamp;
	bool bVisble;
	
};

class DelayTime :public Action
{
public:
	DelayTime(double runtime);
	void update() override;
	void StopAction() override {};
	void StartAction() override {};
};
