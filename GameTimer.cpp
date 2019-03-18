#include "GameTimer.h"

GameTimer::GameTimer()
{
}

void GameTimer::update()
{
	double curstamp = SkTime::GetMSecs();
	if (curstamp >= fLastStamp + fElapse)
	{
		fLastStamp = curstamp;
		(pTarget->*callfun)();
	}
}

void GameTimer::SetTimer(UIShard *target, TimerCallBackFun fun, double ftime)
{
	callfun = fun;
	fElapse = ftime * 1000;
	fLastStamp = SkTime::GetMSecs();
	pTarget = target;
}

void GameTimer::KillTimer(TimerCallBackFun fun)
{
	callfun = NULL;
	fElapse = 1000000 * 1000;
}

GameTimerManage::GameTimerManage()
{

}

//size_t getAddress(TimerCallBackFun function) {
//	typedef void (fnType)(void);
//	fnType * fnPointer = function.target<fnType *>();
//	return (size_t)fnPointer;
//}

void GameTimerManage::SetTimer(UIShard *target, TimerCallBackFun fun, double fElapse)
{
	GameTimer *pTimer = new GameTimer();
	pTimer->SetTimer(target, fun, fElapse);
	if (IsTimerRuning(fun) == false)
	{
		timerlist.push_back(pTimer);
	}

	//char *pAddress = (char *)fun;
	/*auto it = timerlist.find(target->*fun);
	if (it == timerlist.end())
	{
		GameTimer *pTimer = new GameTimer();
		pTimer->SetTimer(target,fun, fElapse);
		timerlist.insert(std::pair<TimerCallBackFun, GameTimer*>(fun, pTimer));
	}*/
}

void GameTimerManage::UpdateAllGameTimer()
{
	for (auto iter = timerlist.begin(); iter != timerlist.end();)
	{
		//(*iter).second->update();
		if ((*iter)->GetCallBackFun() == NULL)
		{
			delete *iter;
			iter = timerlist.erase(iter);
		}
		else
		{
			(*iter)->update();
			iter++;
		}
	}
}


bool GameTimerManage::IsTimerRuning(TimerCallBackFun fun)
{
	for (auto iter = timerlist.begin(); iter != timerlist.end(); iter++)
	{
		if ((*iter)->GetCallBackFun() == fun)
			return true;
	}
	return false;
}

void GameTimerManage::KillTimer(TimerCallBackFun fun)
{
	for (auto iter = timerlist.begin(); iter != timerlist.end(); iter++)
	{
		if ((*iter)->GetCallBackFun() == fun)
		{
			(*iter)->KillTimer(fun);
			break;
		}

	}
}