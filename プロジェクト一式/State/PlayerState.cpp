#include "PlayerState.h"
#include "StateManager.h"
#include "../Animation/AnimationController.h"
#include "../Other/VFXManager.h"
#include "../Other/InputManager.h"
#include "../GameManager/GameManager.h"
#include "../Engine/Model.h"
#include "../Engine/Easing.h"
#include "../Engine/SceneManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Player/PlayerWeapon.h"
#include "../Weapon/WeaponBase.h"
#include "../Weapon/WeaponObjectManager.h"
#include "../UI/Interaction.h"

namespace {
	//���
	static const int AVO_TIME = 40;
	static const float AVO_SPEED = 2.0f;
	static const int INVINCIBLE_TIME = 15;
	static const int UP_COLLIDER_ACTIVE = 38;

	static const float AVO_START_LEAP = 0.2f;
	static const float AVO_END_LEAP = 1.0f;

	//�o��
	static const int APPER_TIME = 60;
	static const float APPER_SPEED = 0.5f;
	static const float APPER_VFX_HEIGHT = 0.7f;
	static const XMFLOAT3 APPER_COMPULSION_POS = { 0.0f, 5.0f, 13.0f};
	
	//�ޏ�
	static const float DISAPPER_SPEED = 0.1f;

	//����擾
	static const float CHANGE_TIME = 60;

}

//--------------------------------------------------------------------------------

PlayerWait::PlayerWait(StateManager* owner) : StateBase(owner)
{
}

void PlayerWait::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->WeaponChangeIndex();
	p->CalcNoMove();
	p->Move();
	if (p->GetAim()->IsTarget()) p->AimTargetRotate();

	//����
	if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
	if (InputManager::IsCmdDown(InputManager::AVO)) owner_->ChangeState("Avo");
	if (p->GetPlayerWeapon()->GetMainWeapon() && InputManager::IsCmdDown(InputManager::MAIN_ATK)) owner_->ChangeState("Atk");
	if (p->GetPlayerWeapon()->GetSubWeapon() && InputManager::IsCmdDown(InputManager::SUB_ATK)) owner_->ChangeState("SubAtk");
	if (InputManager::IsCmd(InputManager::ACTION) && GameManager::GetWeaponObjectManager()->IsInPlayerRange()) owner_->ChangeState("Change");
}

void PlayerWait::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::IDLE, 0.1f);
}

//------------------------------------Walk--------------------------------------------

PlayerWalk::PlayerWalk(StateManager* owner) : StateBase(owner)
{
}

void PlayerWalk::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	
	//����
	if (!InputManager::CmdWalk()) owner_->ChangeState("Wait");
	else if (InputManager::IsCmdDown(InputManager::AVO)) owner_->ChangeState("Avo");
	else if (p->GetPlayerWeapon()->GetMainWeapon() && InputManager::IsCmdDown(InputManager::MAIN_ATK)) owner_->ChangeState("Atk");
	else if (p->GetPlayerWeapon()->GetSubWeapon() && InputManager::IsCmdDown(InputManager::SUB_ATK)) owner_->ChangeState("SubAtk");
	else if (InputManager::IsCmd(InputManager::ACTION) && GameManager::GetWeaponObjectManager()->IsInPlayerRange()) owner_->ChangeState("Change");
	else {
		//���͂��ĂȂ����炻�̏���
		p->GetPlayerWeapon()->WeaponChangeIndex();
		p->CalcMove();
		p->Move();
		p->Rotate();
	}
}

void PlayerWalk::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::RUN, 0.05f);
}

//---------------------------------------WeaponChange-----------------------------------------

PlayerWeaponChange::PlayerWeaponChange(StateManager* owner) : StateBase(owner), time_(0)
{
}

void PlayerWeaponChange::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->CalcNoMove();
	p->Move();
	time_++;

	//�����ĂȂ�����I��
	if (!InputManager::IsCmd(InputManager::ACTION)) {
		if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
		else owner_->ChangeState("Wait");
		return;
	}

	//����
	if (InputManager::IsCmdDown(InputManager::AVO)) owner_->ChangeState("Avo");
	else if (p->GetPlayerWeapon()->GetMainWeapon() && InputManager::IsCmdDown(InputManager::MAIN_ATK)) owner_->ChangeState("Atk");
	else if (p->GetPlayerWeapon()->GetSubWeapon() && InputManager::IsCmdDown(InputManager::SUB_ATK)) owner_->ChangeState("SubAtk");

	//InputUI�̃p�[�Z���g�ݒ�
	float par = (float)time_ / CHANGE_TIME;
	Interaction::SetParcent(par);

	//�؂�ւ����Ԃ܂Ń{�^������������
	if (time_ >= CHANGE_TIME) {
		//��ԋ߂�������擾���ăZ�b�g
		WeaponBase* weapon = GameManager::GetWeaponObjectManager()->GetNearestWeapon();
		if (weapon) p->GetPlayerWeapon()->SetWeapon(weapon);

		//��Ԑ���
		if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
		else owner_->ChangeState("Wait");
	}
}

void PlayerWeaponChange::OnEnter()
{
	time_ = 0;
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::IDLE, 0.1f);
}

void PlayerWeaponChange::OnExit()
{
	Interaction::SetParcent(0.0f);
}

//--------------------------------------Avo------------------------------------------

PlayerAvo::PlayerAvo(StateManager* owner) : StateBase(owner), avoTime_(0), nextCmd_(0)
{
}

void PlayerAvo::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	avoTime_++;

	//����ړ�
	float per = 1.0f - ((float)avoTime_ / (float)AVO_TIME);
	float value = Easing::Leap(AVO_START_LEAP, AVO_END_LEAP, per);
	p->Move(value * AVO_SPEED);

	//Collider��Valid
	if (avoTime_ == INVINCIBLE_TIME) p->GetSphereCollider(0)->SetValid(true);
	if (avoTime_ == UP_COLLIDER_ACTIVE) p->GetSphereCollider(1)->SetValid(true);

	//����
	if (InputManager::IsCmdDown(InputManager::AVO)) nextCmd_ = 1;
	if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) nextCmd_ = 2;
	if (InputManager::IsCmdDown(InputManager::SUB_ATK)) nextCmd_ = 3;

	//�I��
	if (avoTime_ >= AVO_TIME) {
		if (nextCmd_ == 1) owner_->ChangeState("Avo");
		else if (p->GetPlayerWeapon()->GetMainWeapon() && nextCmd_ == 2) owner_->ChangeState("Atk");
		else if (p->GetPlayerWeapon()->GetSubWeapon() && nextCmd_ == 3) owner_->ChangeState("SubAtk");
		else if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
		else owner_->ChangeState("Wait");
	}
}

void PlayerAvo::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->Avo();
	nextCmd_ = 0;
	avoTime_ = 0;
}

void PlayerAvo::OnExit()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->ResetMovement();
}

//---------------------------------------Atk-----------------------------------------

PlayerAtk::PlayerAtk(StateManager* owner) : StateBase(owner), nextCmd_(0)
{
}

void PlayerAtk::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetMainWeapon()->UpdateState();

	//����
	if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) nextCmd_ = 0;
	if (InputManager::IsCmdDown(InputManager::AVO)) nextCmd_ = 1;
	if (InputManager::IsCmdDown(InputManager::SUB_ATK)) nextCmd_ = 2;
	
	if (p->GetPlayerWeapon()->GetMainWeapon()->IsAtkEnd()) {
		if (p->GetPlayerWeapon()->GetSubWeapon() && nextCmd_ == 2) owner_->ChangeState("SubAtk");
		else if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
		else owner_->ChangeState("Wait");
		return;
	}

	if (p->GetPlayerWeapon()->GetMainWeapon()->IsCancellable() && nextCmd_ == 1) {
		owner_->ChangeState("Avo");
		return;
	}

	if (p->GetPlayerWeapon()->GetMainWeapon()->IsNextReady()) {
		if(nextCmd_ == 1) owner_->ChangeState("Avo");
		else if (p->GetPlayerWeapon()->GetSubWeapon() && nextCmd_ == 2) owner_->ChangeState("SubAtk");
		else if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
	}
}

void PlayerAtk::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetMainWeapon()->ChangeAttackState();
	p->GetPlayerWeapon()->GetMainWeapon()->Visible();
	if (p->GetPlayerWeapon()->GetSubWeapon()) p->GetPlayerWeapon()->GetSubWeapon()->Invisible();
	nextCmd_ = 0;
}

void PlayerAtk::OnExit()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetMainWeapon()->ResetState();
}
 
//---------------------------------------SubAtk-----------------------------------------

PlayerSubAtk::PlayerSubAtk(StateManager* owner) : StateBase(owner), nextCmd_(0)
{
}

void PlayerSubAtk::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetSubWeapon()->UpdateState();

	if (InputManager::IsCmdDown(InputManager::SUB_ATK)) nextCmd_ = 0; 
	if (InputManager::IsCmdDown(InputManager::AVO)) nextCmd_ = 1;
	if (InputManager::IsCmdDown(InputManager::MAIN_ATK)) nextCmd_ = 2;

	//�����Ԃɐ���
	if (nextCmd_ == 1 && p->GetPlayerWeapon()->GetSubWeapon()->IsCancellable()) {
		owner_->ChangeState("Avo");
		return;
	}

	//�R���{�I��NextCmd�̃X�e�[�g��
	if (p->GetPlayerWeapon()->GetSubWeapon()->IsAtkEnd()) {
		if (nextCmd_ == 1) owner_->ChangeState("Avo");
		else if (p->GetPlayerWeapon()->GetMainWeapon() && nextCmd_ == 2) owner_->ChangeState("Atk");
		else if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
		else owner_->ChangeState("Wait");
	}
}

void PlayerSubAtk::OnEnter()
{
	nextCmd_ = 0;
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetSubWeapon()->ChangeAttackState();
	p->GetPlayerWeapon()->GetMainWeapon()->Invisible();
	if(p->GetPlayerWeapon()->GetSubWeapon()) p->GetPlayerWeapon()->GetSubWeapon()->Visible();

}

void PlayerSubAtk::OnExit()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetPlayerWeapon()->GetSubWeapon()->ResetState();
	if (p->GetPlayerWeapon()->GetSubWeapon()->IsBlockend()) {
		p->GetPlayerWeapon()->SubWeaponRemove();
	}
}

//--------------------------------------Hear------------------------------------------

PlayerHear::PlayerHear(StateManager* owner) : StateBase(owner)
{
}

void PlayerHear::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	if (p->GetKnockBackTime() > 0) return;

	if (InputManager::CmdWalk()) owner_->ChangeState("Walk");
	owner_->ChangeState("Wait");
}

void PlayerHear::OnEnter()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	p->GetAnimationController()->SetNextAnim((int)PLAYER_ANIMATION::BACK_STEP, 0.3f);

}

//----------------------------------------Dead----------------------------------------

PlayerDead::PlayerDead(StateManager* owner) : StateBase(owner), time_(0)
{
}

void PlayerDead::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	time_++;
	
	int s = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::DEAD).startFrame;
	int e = p->GetAnimationController()->GetAnim((int)PLAYER_ANIMATION::DEAD).endFrame;

	if (time_ >= (e - s)) {
		Model::SetAnimStop(p->GetModelHandle(), true);
		GameManager::GetSceneManager()->ChangeScene(SCENE_ID_RESULT);
	}
}

void PlayerDead::OnEnter()
{
	time_ = 0;
}

//--------------------------------------Appear------------------------------------------

PlayerAppear::PlayerAppear(StateManager* owner) : StateBase(owner), time_(0), apperPos_{0.0f, 0.0f, 0.0f}
{
}

void PlayerAppear::Update()
{
	Player* p = static_cast<Player*>(owner_->GetGameObject());
	XMFLOAT3 pos = p->GetPosition();
	time_++;
	
	//�v���C���[�EAimComplusion�̍��W�Z�b�g
	pos.y -= APPER_SPEED;
	p->SetPosition(pos);
	p->GetAim()->SetCompulsion(Float3Add(apperPos_, APPER_COMPULSION_POS), apperPos_);

	//�I���EVFX�Ƃ����
	if (time_ >= APPER_TIME) {
		p->SetPosition(apperPos_);
		XMFLOAT3 vfxPos = { apperPos_.x, apperPos_.y + APPER_VFX_HEIGHT, apperPos_.z };
		VFXManager::CreateVfxExplode1(vfxPos);
		owner_->ChangeState("Wait");
	}
}

void PlayerAppear::OnEnter()
{
	apperPos_ = owner_->GetGameObject()->GetPosition();
	XMFLOAT3 startPos = { apperPos_.x, apperPos_.y + (float)APPER_TIME * APPER_SPEED, apperPos_.z };
	owner_->GetGameObject()->SetPosition(startPos);
	time_ = 0;
}

//-------------------------------------DisApper-------------------------------------------

PlayerDisAppear::PlayerDisAppear(StateManager* owner) : StateBase(owner)
{
}

void PlayerDisAppear::Update()
{
	XMFLOAT3 pos = owner_->GetGameObject()->GetPosition();
	owner_->GetGameObject()->SetPosition({ pos.x, pos.y - DISAPPER_SPEED, pos.z });
}
