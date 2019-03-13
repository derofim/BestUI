#pragma once
#include "GrContext.h"
#include "SkTime.h"
#include <map>

class UIShard;
typedef void(UIShard::*TimerCallBackFun)(void);
#define timer_sel(sel)  static_cast<TimerCallBackFun>(&sel)

class GameTimer
{
public:
	GameTimer();
	void SetTimer(UIShard *target, TimerCallBackFun fun, double ftime);
	void KillTimer(TimerCallBackFun fun);
	void update();

	TimerCallBackFun GetCallBackFun()
	{
		return callfun;
	}
private:
	double fLastStamp;
	double fElapse;
	TimerCallBackFun callfun;
	UIShard *pTarget;
};
class GameTimerManage
{
public:
	GameTimerManage();
	void UpdateAllGameTimer();
	void SetTimer(UIShard *target, TimerCallBackFun fun, double fElapse);
	void KillTimer(TimerCallBackFun fun);
private:
	//std::map<TimerCallBackFun, GameTimer *> timerlist;

	std::vector<GameTimer *> timerlist;
};
