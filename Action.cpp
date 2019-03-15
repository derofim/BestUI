#include "Action.h"
#include "UIWidget.h"

#include "UIRoot.h"
Blink::Blink(double runtime, int nBlink)
{
	nBlinkTick=nBlink;
	SetRunTime(runtime);
	bVisble = false;
}

void Blink::StartAction()
{
	fLastStamp = SkTime::GetMSecs();

}


void Blink::StopAction()
{
	GetWidget()->SetVisible(true);
}

void Blink::update()
{
	double fEachBlink = GetRunTime() / nBlinkTick/2;
	if (ActionIsStart() == false)
		return;
	if (SkTime::GetMSecs() - fLastStamp >= fEachBlink)
	{
		GetWidget()->SetVisible(bVisble);
		bVisble ^= 1;
		fLastStamp = SkTime::GetMSecs();
	}
}

MoveTo::MoveTo(double runtime, SkScalar x, SkScalar y)
{
	SetRunTime(runtime);
	move_x = x;
	move_y = y;
	
}

void MoveTo::StartAction()
{
	init_x = GetWidget()->GetSkRect().left();
	init_y = GetWidget()->GetSkRect().top();
	/*range_x = fabs(move_x - GetWidget()->GetSkRect().left());
	range_y = fabs(move_y - GetWidget()->GetSkRect().top());*/
	range_x = move_x - GetWidget()->GetSkRect().left();
	range_y = move_y - GetWidget()->GetSkRect().top();
}
void MoveTo::StopAction()
{
	GetWidget()->SetPosition(move_x, move_y);
}

void MoveTo::update()
{
	if (ActionIsStart() == false)
		return;
	double curstamp = SkTime::GetMSecs();
	double rate=(curstamp - GetInitStamp())/ GetRunTime();
	SkScalar x, y;
	x = rate * range_x+init_x;
	y = rate * range_y+init_y;
	GetWidget()->SetPosition(x, y);
}


RotateTo::RotateTo(double runtime, SkScalar degrees)
{
	SetRunTime(runtime);
	fDegress=degrees;
}

void RotateTo::StartAction()
{
}

void RotateTo::StopAction()
{
	GetWidget()->SetDegress(fDegress);
}

void RotateTo::update()
{
	if (ActionIsStart() == false)
		return;
	double curstamp = SkTime::GetMSecs();
	double rate = (curstamp - GetInitStamp()) / GetRunTime();
	GetWidget()->SetDegress(rate*fDegress);
}

DelayTime::DelayTime(double runtime, ActCallBackFun callback)
{
	SetRunTime(runtime);
	fun = callback;
}

void DelayTime::update()
{
	ActionIsStart();
}


void DelayTime::StopAction()
{
	if (fun != 0)
		fun();
}


Repeat::Repeat(UIWidget *pUi, Action *act, int nRep, ActCallBackFun callback)
{
	pActionManage = gActionManage;
	double fDelayTime = 0;
	 
	Action *pAct = act;
	for (int k = 0; k < nRep; k++)
	{
		if (k != 0)
		{
			pAct = pAct->clone();
		}
		pActionManage->AddAction(pAct, pUi, fDelayTime);
		
		fDelayTime += pAct->GetRunTime();
	}
	if (callback != NULL)
	{
		pActionManage->AddAction(new DelayTime(fDelayTime / 1000, callback), pUi, 0);
	}
}

Repeat::Repeat(UIWidget *pUi, Sequence *seq, int nRep, ActCallBackFun callback)
{
	double fDelayTime = 0;
	for (int k = 0; k < nRep; k++)
	{
		fDelayTime+=seq->RunSequence();
	}
	if (callback != NULL)
	{
		pActionManage->AddAction(new DelayTime(fDelayTime / 1000, callback), pUi, 0);
	}
}

Sequence::Sequence(UIWidget *pUi, ActCallBackFun callback, ...)
{
	pActionManage = gActionManage;
	fun = callback;
	pWidget = pUi;
	va_list params;
	va_start(params, callback);

	for (;;)
	{
		Action *pAct = va_arg(params, Action *);
		if (pAct == NULL)
			break;
		list.push_back(pAct);
		
	}
	va_end(params);
	nRef = 0;
	fDelayTotal = 0;

	
}

double Sequence::RunSequence()
{
	double fDelayTime = 0;

	if (nRef > 0)
	{
		for (auto iter = list.begin(); iter != list.end(); iter++)
		{
			Action *pAct = (*iter)->clone();
			(*iter) = pAct;
		}
	}
	for (auto iter = list.begin(); iter != list.end();iter++)
	{
		Action *pAct = (*iter);
		pActionManage->AddAction(pAct, pWidget, fDelayTime+ fDelayTotal);
		fDelayTime += pAct->GetRunTime();
	}

	if (fun != NULL)
	{
		pActionManage->AddAction(new DelayTime((fDelayTime + fDelayTotal) / 1000, fun), pWidget, 0);
	}
	nRef++;
	fDelayTotal += fDelayTime;
	return fDelayTime;
}

Sequence::~Sequence()
{
	list.clear();
}




Action::Action()
{
	pWidget = NULL;
	bStartFlag = false;
}

bool Action::ActionIsStart()
{
	bool bRet = false;
	double curstamp=SkTime::GetMSecs();
	if (curstamp < initstamp)
		st = Ready;
	else if (curstamp >= initstamp && curstamp <= endstamp)
	{
		st = Start;
		bRet = true;
		if (bStartFlag == false)
		{
			StartAction();
			bStartFlag = true;
		}
	}
	else
	{
		st = Stop;
		StopAction();
	}

	return bRet;


}



void Action::ReadyAction(double fDelayTime)
{
	initstamp = SkTime::GetMSecs() + fDelayTime;
	endstamp = initstamp + runstamp;
//	printf("%f	%f\n", initstamp, endstamp);
}


ActionManage::ActionManage()
{

}

void ActionManage::UpdateAllAction()
{
	for (auto iter = actionlist.begin(); iter != actionlist.end();)
	{
		if ((*iter)->GetActionState() == Action::ActState::Stop)
		{
			delete *iter;
			iter=actionlist.erase(iter);
			
		}
		else
		{
			(*iter)->update();
			iter++;
		}
		
	}
}



void ActionManage::AddAction(Action *act, UIWidget *pWidget, double fDelayTime)
{
	actionlist.push_back(act);
	act->SetWidget(pWidget);
	act->ReadyAction(fDelayTime);
}

