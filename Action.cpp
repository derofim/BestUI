#include "Action.h"



Blink::Blink(double time, int nBlink)
{
	nTicks=nBlink;
}

Action::Action()
{

}

void Action::SetAction(double time)
{
	initstamp = SkTime::GetMSecs();
	endstamp = initstamp + time*1000;
}


void Action::StopAction()
{

}

void Action::update()
{
	if (SkTime::GetMSecs() > endstamp)
	{
		StopAction();
	}
}