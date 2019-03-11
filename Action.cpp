#include "Action.h"
#include "UIWidget.h"


Blink::Blink(double time, int nBlink)
{
	nBlinkTick=nBlink;
	SetRunTime(time);
	bVisble = false;
}

void Blink::StartAction()
{
	fLastStamp = SkTime::GetMSecs();
	Action::StartAction();
}


void Blink::StopAction()
{
	GetWidget()->SetVisible(true);
	Action::StopAction();
}

void Blink::update()
{
	double fEachBlink = GetRunTime() / nBlinkTick/2;
	
	if (SkTime::GetMSecs() > GetEndStamp())
	{
		StopAction();
		return;
	}
	if (SkTime::GetMSecs() - fLastStamp >= fEachBlink)
	{
		GetWidget()->SetVisible(bVisble);
		bVisble ^= 1;
		fLastStamp = SkTime::GetMSecs();
	}
}

Action::Action()
{
	pWidget = NULL;
	st = NotStart;
}

void Action::StartAction()
{
	//initstamp = SkTime::GetMSecs();
	endstamp = SkTime::GetMSecs() + runstamp;
	st = Start;
	
}

void Action::StopAction()
{
	st = Stop;
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

void ActionManage::AddAction(Action *act, UIWidget *pWidget)
{
	actionlist.push_back(act);
	act->SetWidget(pWidget);
	act->StartAction();
}

