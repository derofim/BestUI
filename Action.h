#pragma once
//#include "UIWidget.h"

#include "GrContext.h"
#include "SkTime.h"
#include <map>


//
//
//
////Í¬Ê±Ö´ÐÐ
//class Spawn :public ActionManage
//{
//public:
//};
//





typedef std::function<void(void)> ActCallBackFun;
class UIWidget;
class Action {
public:

	enum ActState{
		Ready,
		Start,
		Stop
	};
	Action();

	virtual Action *clone() { return 0; }
	//void SetStopActionCallBack();
	virtual void StartAction() {};
	virtual void StopAction() {};


	virtual void update() {};

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
	double GetInitStamp()
	{
		return initstamp;
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
	void StopAllAction(UIWidget *pWidget);
	void UpdateAllAction();
private:
	std::vector<Action *> actionlist;
};

//typedef std::function<void(void)> TimerCallBackFun;


class Sequence /*:public ActionManage*/
{
public:
	Sequence(UIWidget *pUi, ActCallBackFun callback,...);
	double RunSequence();
	~Sequence();
private:
	ActionManage *pActionManage;
	ActCallBackFun fun;
	std::vector<Action *> list;
	UIWidget *pWidget;
	double fDelayTotal;
	int nRef;
	//RunAction()
};

class Repeat
{
public:
	Repeat(UIWidget *pUi, Action *act,int nRep, ActCallBackFun callback=0);
	Repeat(UIWidget *pUi, Sequence *seq, int nRep, ActCallBackFun callback = 0);
	~Repeat() {};
private:
	ActionManage *pActionManage;
	
};




class Blink :public Action
{
public:
	Blink(double runtime, int nBlink);
	virtual Blink *clone() { return  new Blink(*this); }
	void StartAction() override;
	void StopAction()override;
	void update() override;
private:
	int nBlinkTick;
	double fLastStamp;
	bool bVisble;
	
};


class MoveTo :public Action
{
public:
	MoveTo(double runtime, SkScalar x, SkScalar y);
	virtual MoveTo *clone() { return  new MoveTo(*this); }
	void StartAction() override;
	void StopAction()override;
	void update() override;
private:
	SkScalar move_x;
	SkScalar move_y;
	SkScalar range_x;
	SkScalar range_y;
	SkScalar init_x;
	SkScalar init_y;

};


class ScrollTo :public Action
{
public:
	ScrollTo(double runtime, SkScalar x,SkScalar y);
	virtual ScrollTo *clone() { return  new ScrollTo(*this); }
	void StartAction() override;
	void StopAction()override;
	void update() override;
private:
	SkScalar move_x;
	SkScalar move_y;
	SkScalar range_x;
	SkScalar range_y;
	SkScalar init_x;
	SkScalar init_y;
};


class RotateTo :public Action 
{
public:
	RotateTo(double runtime, SkScalar degrees);

	virtual RotateTo *clone() { return  new RotateTo(*this); }
	void StartAction() override;
	void StopAction()override;
	void update() override;
private:
	SkScalar fDegress;
};

class DelayTime :public Action
{
public:
	DelayTime(double runtime, ActCallBackFun callback=0);
	virtual DelayTime *clone() { return  new DelayTime(*this); }
	void update() override;
	void StopAction() override ;
	void StartAction() override {};
private:
	ActCallBackFun fun;
};
