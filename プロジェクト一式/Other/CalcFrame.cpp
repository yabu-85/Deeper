#include "CalcFrame.h"


void OneFrame::CalcFrame(int t) { if (time == t) OnFrame(); }
void DoubleFrame::CalcFrame(int t) {
	if (time <= t && endTime >= t) OnFrame();
}
void BoundaryDoubleFrame::CalcFrame(int t)
{
	if (time <= t && endTime >= t) {
		OnFrame();
		if (time == t) OnEnter();
		else if (endTime == t) OnExit();
	}
}

EnemyAttackController::~EnemyAttackController()
{
	for (OneFrame* frame : frameList) delete frame;
	frameList.clear();
}

void EnemyAttackController::Update(int currentTime)
{
	for (OneFrame* action : frameList) {
		action->CalcFrame(currentTime);
	}
}