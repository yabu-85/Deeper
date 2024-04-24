#include "EnemyAttackBase.h"

EnemyFrame::EnemyFrame(int start, int end)
	: pParent_(nullptr), frame(start, end), inFrame_(false)
{
}

EnemyAttackController::~EnemyAttackController()
{
	for (EnemyFrame* action : actions) delete action;
	actions.clear();
}

void EnemyAttackController::Update(int currentTime)
{
	for (EnemyFrame* action : actions) {
		//�t���[�����EEnter�̔���
		if (action->IsTargetFrame(currentTime)) {
			if (!action->GetInFrame()) {
				action->OnEnter();
				action->SetInFrame(true);
			}
			action->Update();
		}

		//�I���t���[�����ǂ�����p����
		if (action->IsEndFrame(currentTime)) {
			action->OnExit();
			action->SetInFrame(false);
		}
	}
}
