#include "SwordBossState.h"
#include "StateManager.h"
#include "../VFXManager.h"
#include "../GameManager/GameManager.h"
#include "../Player/Player.h"
#include "../Player/Aim.h"
#include "../Enemy/SwordBoss.h"
#include "../Enemy/EnemyUi.h"
#include "../Stage/CreateStage.h"
#include "../Engine/Model.h"
#include "../Engine/Global.h"

#include "../BehaviorTree/IsEnemyActionReadyNode.h"
#include "../BehaviorTree/ChangeStateNode.h"
#include "../BehaviorTree/BehaviourNode.h"
#include "../BehaviorTree/PlayerConditionNode.h"
#include "../BehaviorTree/IsEnemyStateNode.h"
#include "../BehaviorTree/IsEnemyPermission.h"

#include "../Action/MoveAction.h"
#include "../Action/RotateAction.h"

namespace {
	static const int APPER_TIME = 180;
	static const int FOUND_SEARCH = 10;
	static const int FPS = 60;
	static const int MIN_MOVE_TIME = 6;
	static const int MAX_MOVE_TIME = 5;

	static const float FAST_SPEED = 0.06f;
	static const float SLOW_SPEED = 0.04f;
	static const float ROTATE_RATIO = 0.07f;

}

SwordBossAppear::SwordBossAppear(StateManager* owner) : StateBase(owner), time_(0)
{
}

void SwordBossAppear::Update()
{
	time_++;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	
	float tsize = (float)time_ / (float)APPER_TIME;
	e->SetScale(XMFLOAT3(tsize, tsize, tsize));

	if (time_ > APPER_TIME) owner_->ChangeState("Combat");
}

void SwordBossAppear::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::IDLE, 0.1f);

	XMFLOAT3 pos = e->GetPosition();
	VFXManager::CreatVfxEnemySpawn(pos, APPER_TIME);
}

void SwordBossAppear::OnExit()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
}

//--------------------------------------------------------------------------------

SwordBossDead::SwordBossDead(StateManager* owner) : StateBase(owner), time_(0)
{
}

void SwordBossDead::Update()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	time_++;

	int startT = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::DEAD).startFrame;
	int endT = e->GetAnimationController()->GetAnim((int)SWORDBOSS_ANIMATION::DEAD).endFrame;
	int allTime = (endT - startT);
	
	if (time_ >= allTime) {
		Model::SetAnimeStop(e->GetModelHandle(), true);
		e->DeadExit();
	}
}

void SwordBossDead::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::DEAD, 0.1f);
	e->DeadEnter();
	time_ = 0;
}

//--------------------------------------------------------------------------------

SwordBossCombat::SwordBossCombat(StateManager* owner) : StateBase(owner), time_(0)
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	
	//----------ビヘイビアツリーの設定------------
	root_ = new Root();

	Selector* selector1 = new Selector();
	root_->SetRootNode(selector1);

	Selector* waitSelector = new Selector();
	Selector* moveSelector = new Selector();
	IsEnemyCombatState* wCon = new IsEnemyCombatState(waitSelector, "Wait", e);
	IsEnemyCombatState* mCon = new IsEnemyCombatState(moveSelector, "Move", e);
	selector1->AddChildren(wCon);
	selector1->AddChildren(mCon);

	//-------------------------------------Wait--------------------------------------
	EnemyChangeCombatStateNode* action3 = new EnemyChangeCombatStateNode(e, "Attack");
	IsEnemyAttackPermission* condition5 = new IsEnemyAttackPermission(action3, e);
	IsPlayerInRangeNode* condition6 = new IsPlayerInRangeNode(condition5, e->GetAttackDistance(), e, GameManager::GetPlayer());
	IsEnemyAttackReady* condition8 = new IsEnemyAttackReady(condition6, e);
	waitSelector->AddChildren(condition8);

	EnemyChangeCombatStateNode* action1 = new EnemyChangeCombatStateNode(e, "Move");
	IsEnemyMovePermission* condition2 = new IsEnemyMovePermission(action1, e);
	IsEnemyActionReady* condition3 = new IsEnemyActionReady(condition2, e);
	waitSelector->AddChildren(condition3);

	//-------------------------------------Move--------------------------------------
	moveSelector->AddChildren(condition8);

}

void SwordBossCombat::Update()
{
	time_++;
	if (time_ % 10 == 0) root_->Update();
	
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetCombatStateManager()->Update();

}

void SwordBossCombat::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetEnemyUi()->InitTargetFoundUi();
	e->GetRotateAction()->Initialize();
	e->GetRotateAction()->SetTarget(GameManager::GetPlayer());
	e->GetRotateAction()->SetRatio(ROTATE_RATIO);

}

SwordBossCombat::~SwordBossCombat()
{
	delete root_;
}

//-------------------------------------CombatState-------------------------------------------

SwordBossWait::SwordBossWait(StateManager* owner) : StateBase(owner)
{
}

void SwordBossWait::Update()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetRotateAction()->Update();

	//壁に当たったか調べて
	e->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
	if (e->GetOrientedMoveAction()->CheckWallCollision(1)) {
		e->GetOrientedMoveAction()->SetDirection(XMVector3Normalize(XMVECTOR{ 0.0f, 0.0f, 1.0f, 0.0f }));
	}
	
	e->GetOrientedMoveAction()->Update();

}

void SwordBossWait::OnEnter()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(SLOW_SPEED);
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::IDLE, 0.1f);

	//プレイヤーから指定の範囲内で
	//ゲーム参考にしてから作る
	XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
	XMFLOAT3 ePos = e->GetPosition();
	XMFLOAT3 vec = { pPos.x - ePos.x, 0.0f, pPos.z - ePos.z };
	float dist = sqrt(vec.x * vec.x + vec.z * vec.z);
	if(dist <= e->GetCombatDistance()) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	else {
		int r = rand() % 2;
		if(r == 0) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 1, 0, 0, 0 });
		else if (r == 1) e->GetOrientedMoveAction()->SetDirection(XMVECTOR{ -1, 0, 0, 0 });
	}
}

//------------------------------------Move--------------------------------------------

SwordBossMove::SwordBossMove(StateManager* owner) : StateBase(owner), time_(0)
{
}

void SwordBossMove::Update()
{
	time_--;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());

	if (e->GetMoveAction()->IsInRange() || (time_ % 5 == 0 && e->GetMoveAction()->IsOutTarget(3.0f)) ) {
		e->GetMoveAction()->UpdatePath(GameManager::GetPlayer()->GetPosition());
	}

	e->GetMoveAction()->Update();
	e->GetRotateAction()->Update();

	//移動時間終了
	if(time_ <= 0) owner_->ChangeState("Wait");
}

void SwordBossMove::OnEnter()
{
	time_ = FPS * MIN_MOVE_TIME + rand() % MAX_MOVE_TIME * FPS;
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->SetMoveSpeed(FAST_SPEED);
	e->GetAnimationController()->SetNextAnime((int)SWORDBOSS_ANIMATION::RUN, 0.1f);

}

void SwordBossMove::OnExit()
{
	SwordBoss* e = static_cast<SwordBoss*>(owner_->GetGameObject());
	e->GetMoveAction()->StopMove();
}

//-------------------------------------Attack-------------------------------------------

//AttackStateの中でAttack1とかAttack2とか分けるようにする

//攻撃しようとなったらすぐに次に攻撃する種類を決めるようにする
//攻撃によって必ず次の攻撃に移る物がある、それは、、どうしよう

//横ー＞上のその逆もあり得る
//この場合も考慮しないといけない
//今までのコンボを覚えるようにしていれば出来そう
//同じ行動はしないようにする

#include <vector>
#include <array>
using std::vector;
using std::pair;
enum SWORD_BOSS {
	Slash_Up = 0,
	Slash_Down,
	Slash_Right,
	Slash_Left,
	Thrust,
	Slash_Max,
};

struct AttackData {
	int attackNumber;			//番号
	int animId;					//AnimationControllerのIDを割り当てる
	vector<int> derivative;		//派生可能モーション
	pair<int, int> attack;		//攻撃判定フレーム
	pair<int, int> rotateMove;	//移動・回転フレーム
	
	bool IsAttackFrame(int time) { return attack.first >= time && attack.second <= time; }
	bool IsAttackEndFrame(int time) { return attack.second == time; }
	bool IsRotateMoveFrame(int time) { return rotateMove.first >= time && rotateMove.second <= time; }

	AttackData(int number, int anim, vector<int> derivat) : attackNumber(number), animId(anim), derivative(derivat) {};
	AttackData(int number, int anim, vector<int> derivat, pair<int, int> atk, pair<int, int> rm)
		: attackNumber(number), animId(anim), derivative(derivat), attack(atk), rotateMove(rm) {};
};

struct AttackManager {
	bool isNextReady;			//次の行動していいか
	vector<int> infoList;		//コンボ予定リスト
	vector<AttackData> list;	//全部の攻撃情報登録リスト

	void SetIsNextReady(bool b) { isNextReady = b; }
	bool IsNextReady() { return isNextReady; }
	
	bool IsInfoListEmpty() { return infoList.empty(); }
	void AddList(const AttackData& info) { list.push_back(info); }

	void SelectAction() {
		infoList.clear();
		std::vector<AttackData> copyList = list;
		int numSelected = std::rand() % list.size() + 1;

		//ランダム並べ替え
		for (int i = 0; i < list.size(); ++i) {
			int j = rand() % (i + 1);
			std::swap(copyList[i], copyList[j]);
		}

		//選択する
		for (int i = 0; i < list.size(); i++) infoList.push_back(copyList.at(i).attackNumber);
	};

	//今やってる攻撃終わった
	void EndAction() {
		infoList.erase(infoList.begin());
	}

	int GetAttackNumber() {
		return infoList.front();
	}

	AttackManager() : isNextReady(false) {};

};

namespace {
	AttackData ATTACK_INFO_LIST[] = {
		AttackData(0, (int)SWORDBOSS_ANIMATION::ATTACK1, { Slash_Right }, { 37, 80 }, { 0, 45 }),
		AttackData(1, (int)SWORDBOSS_ANIMATION::ATTACK2, { Slash_Up }, { 40, 56 }, { 0, 45 }),
		AttackData(1, (int)SWORDBOSS_ANIMATION::ATTACK2, { Slash_Up }, { 40, 56 }, { 0, 45 }),
	};

	AttackManager atk = AttackManager();

}

SwordBossAttack::SwordBossAttack(StateManager* owner) : StateBase(owner), time_(0)
{
	pBoss_ = static_cast<SwordBoss*>(owner_->GetGameObject());

	for (int i = 0; i < std::size(ATTACK_INFO_LIST) -1; i++) atk.AddList(ATTACK_INFO_LIST[i]);
	atk.SelectAction();
}

void SwordBossAttack::Update()
{
	time_++;
	int number = atk.GetAttackNumber();
	if (number < 0 || number >= 2) {
		number = rand() % 2;
	}

	switch (number) {
	case (int)Slash_Up:
		Attack1Update();
		break;
	case (int)Slash_Down:
		Attack2Update();
		break;
	default:
		OutputDebugString("Attack No 1&2\n\n");
		break;
	}

	//回転・移動
	if (atk.list[number].IsRotateMoveFrame(time_)) {
		pBoss_->GetRotateAction()->Update();
		pBoss_->GetOrientedMoveAction()->SetTarget(GameManager::GetPlayer()->GetPosition());
		pBoss_->GetOrientedMoveAction()->Update();
	}
	if (atk.list[number].IsAttackFrame(time_)) pBoss_->CalcPoly();
	if (atk.list[number].IsAttackEndFrame(time_)) pBoss_->AttackEnd();

	int startT = pBoss_->GetAnimationController()->GetAnim(atk.list[number].animId).startFrame;
	int endT = pBoss_->GetAnimationController()->GetAnim(atk.list[number].animId).endFrame;
	int allTime = (endT - startT);
	
	if (time_ >= allTime) {
		atk.EndAction();
		if (atk.IsNextReady() && !atk.IsInfoListEmpty()) owner_->ChangeState("Attack");
		else {
			atk.SelectAction();
			owner_->ChangeState("Wait");
		}
	}
}

void SwordBossAttack::OnEnter()
{
	time_ = 0;
	pBoss_->GetOrientedMoveAction()->SetDirection(XMVECTOR{ 0, 0, 1, 0 });
	pBoss_->SetCombatReady(false);
	pBoss_->GetAnimationController()->SetNextAnime(atk.list[atk.GetAttackNumber()].animId, 1.0f);

	atk.SetIsNextReady(false);
}

void SwordBossAttack::OnExit()
{
	pBoss_->GetRotateAction()->SetRatio(ROTATE_RATIO);
	pBoss_->SetAttackCoolDown(rand() % 150);
	pBoss_->AttackEnd();

}

void SwordBossAttack::Attack1Update()
{
	OutputDebugString("Attack1\n\n");
	
	float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), pBoss_->GetPosition());
	if (dist <= 3.0f) atk.SetIsNextReady(true);
	else atk.SetIsNextReady(false);

}

void SwordBossAttack::Attack2Update()
{
	OutputDebugString("Attack2\n\n");
	
	float dist = CalculationDistance(GameManager::GetPlayer()->GetPosition(), pBoss_->GetPosition());
	if (dist <= 3.0f) atk.SetIsNextReady(true);
	else atk.SetIsNextReady(false);

}

//--------------------------------------------------------------------------------