#include "TestWeaponMain.h"
#include "../GameManager.h"
#include "../VFXManager.h"
#include "../Player/Player.h"
#include "../Player/PlayerCommand.h"
#include "../Player/Aim.h"
#include "../Engine/PolyLine.h"
#include "../Engine/Global.h"
#include "../Engine/SegmentCollider.h"
#include "../Engine/Model.h"
#include "../State/StateManager.h"
#include "../Enemy/EnemyManager.h"

namespace {
    static const float WEAPON_SIZE = 1.2f;
    static const float MOVE_SPEED = 0.3f;
    static const float ROTATE_RATIO = 0.2f;
    static const int ROTATE_FRAME = 5;

    //攻撃アニメーションのフレーム
    static const int ANIM_FRAME1[2] = { 595, 650 };
    static const int ANIM_FRAME2[2] = { 650, 680 };
    static const int ANIM_FRAME3[2] = { 665, 680 };
    
    //10から～30フレームまでが攻撃フレーム
    static const int ANIM_ATTACK_FRAME1[2] = { 17, 33 };
    static const int ANIM_ATTACK_FRAME2[2] = { 15, 25 };
    static const int ANIM_ATTACK_FRAME3[2] = { 0, 25 };

    static const int COMBO_TIME1 = ANIM_FRAME1[1] - ANIM_FRAME1[0];
    static const int COMBO_TIME2 = ANIM_FRAME2[1] - ANIM_FRAME2[0];
    static const int COMBO_TIME3 = ANIM_FRAME3[1] - ANIM_FRAME3[0];

}

TestWeaponMain::TestWeaponMain(GameObject* parent)
	: WeaponBase(parent, "TestWeaponMain"), pPlayer_(nullptr), seg_(nullptr), damage_(0), wandPos_(0,0,0), pPolyLine_(nullptr)
{
    transform_.pParent_ = nullptr;
}

TestWeaponMain::~TestWeaponMain()
{
}

void TestWeaponMain::Initialize()
{
    hModel_ = Model::Load("Model/BlueBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = static_cast<Player*>(GetParent());
    Model::GetBoneIndex(pPlayer_->GetModelHandle(), "Weapon", &boneIndex_, &partIndex_);
    assert(boneIndex_ >= 0);

    damage_ = 20;

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new TestWeaponCombo1(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo2(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo3(pStateManager_));
    pStateManager_->ChangeState("");
    pStateManager_->Initialize();

    seg_ = new SegmentCollider(XMFLOAT3(), XMVECTOR());
    seg_->SetValid(false);
    AddAttackCollider(seg_);

    pPolyLine_ = new PolyLine;
    pPolyLine_->Load("tex2.png");

}

void TestWeaponMain::Update()
{

}

void TestWeaponMain::Draw()
{
    if (!IsVisibled()) return;
   
    wandPos_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    transform_.rotate_ = Model::GetBoneAnimRotate(pPlayer_->GetModelHandle(), boneIndex_, partIndex_);
    
    XMFLOAT3 pPos = pPlayer_->GetPosition();
    seg_->SetCenter({ wandPos_.x - pPos.x, wandPos_.y - pPos.y, wandPos_.z - pPos.z });

    if (transform_.rotate_.x >= 90.0f || transform_.rotate_.x <= -90.0f) {
        transform_.rotate_.y *= -1.0f;
    }
    transform_.rotate_.y += pPlayer_->GetRotate().y;

    Transform t = transform_;
    t.position_ = wandPos_;
    Model::SetTransform(hModel_, t);
    Model::Draw(hModel_);

    pPolyLine_->Draw();
}

void TestWeaponMain::Release()
{
    SAFE_RELEASE(pPolyLine_);
    SAFE_DELETE(pPolyLine_);

}

void TestWeaponMain::OnAttackCollision(GameObject* pTarget)
{
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        EnemyBase* e = static_cast<EnemyBase*>(pTarget);
        e->ApplyDamage(damage_);
        e->KnockBack(MEDIUM);
        e->SetAllColliderValid(false);

        VFXManager::CreatVfxExplode1(wandPos_);
    }
}

void TestWeaponMain::ChangeAttackState()
{
    atkEnd_ = false;
    pStateManager_->ChangeState("Combo1");

}

void TestWeaponMain::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("");
    GameManager::GetEnemyManager()->ResetAllEnemyCollider();
    seg_->SetValid(false);

}

void TestWeaponMain::CalcDamage()
{
    XMFLOAT3 tar = XMFLOAT3(transform_.rotate_.x, transform_.rotate_.y, 0.0f);
    XMFLOAT3 target;
    target.x = (float)sin(XMConvertToRadians(tar.y));
    target.y = -(float)tan(XMConvertToRadians(tar.x));
    target.z = (float)cos(XMConvertToRadians(tar.y));

    if (tar.x >= 90.0f || tar.x <= -90.0f) {
        target.x *= -1.0f;
        target.y *= -1.0f;
        target.z *= -1.0f;
    }

    XMFLOAT3 vec = target;
    XMVECTOR vVec = XMLoadFloat3(&vec);
    vVec = XMVector3Normalize(vVec) * WEAPON_SIZE;
    seg_->SetVector(vVec);
    seg_->SetValid(true);

    XMStoreFloat3(&vec, vVec);
    vec = XMFLOAT3(wandPos_.x + vec.x, wandPos_.y + vec.y, wandPos_.z + vec.z);
    pPolyLine_->AddPosition(wandPos_, vec);

}

//--------------------state---------------------------------------------------

TestWeaponCombo1::TestWeaponCombo1(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void TestWeaponCombo1::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    if (time_ < ROTATE_FRAME) {
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(ROTATE_RATIO);
        else if (p->GetCommand()->CmdWalk()) p->Rotate(ROTATE_RATIO);
    }

    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (p->GetCommand()->CmdAtk()) next_ = true;

    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if(time_ >= ANIM_ATTACK_FRAME1[0] && time_ <= ANIM_ATTACK_FRAME1[1])
    m->CalcDamage();
    
    time_++;
    if (time_ >= COMBO_TIME1) {
        if (next_ == true) {
            GameManager::GetEnemyManager()->ResetAllEnemyCollider();
            owner_->ChangeState("Combo2");
        }
        else m->ResetState();
    }
}

void TestWeaponCombo1::OnEnter()
{
    time_ = 0;
    next_ = false;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    Model::SetAnimFrame(p->GetModelHandle(), ANIM_FRAME1[0], ANIM_FRAME1[1], 1.0f);

}

void TestWeaponCombo1::OnExit()
{
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}

//---------------------------------------------

TestWeaponCombo2::TestWeaponCombo2(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void TestWeaponCombo2::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    if (time_ < ROTATE_FRAME) {
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(ROTATE_RATIO);
        else if (p->GetCommand()->CmdWalk()) p->Rotate(ROTATE_RATIO);
    }

    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (p->GetCommand()->CmdAtk()) next_ = true;

    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ >= ANIM_ATTACK_FRAME2[0] && time_ <= ANIM_ATTACK_FRAME2[1])
    m->CalcDamage();

    time_++;
    if (time_ >= COMBO_TIME2) {
        GameManager::GetEnemyManager()->ResetAllEnemyCollider();
        if (next_ == true) {
            GameManager::GetEnemyManager()->ResetAllEnemyCollider();
            owner_->ChangeState("Combo3");
        }
        else m->ResetState();
    }
}

void TestWeaponCombo2::OnEnter()
{
    time_ = 0;
    next_ = false;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    Model::SetAnimFrame(p->GetModelHandle(), ANIM_FRAME2[0], ANIM_FRAME2[1], 1.0f);

}

void TestWeaponCombo2::OnExit()
{
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}

//---------------------------------------------

TestWeaponCombo3::TestWeaponCombo3(StateManager* owner) : StateBase(owner), time_(0), next_(false)
{
}

void TestWeaponCombo3::Update()
{
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    if (time_ < ROTATE_FRAME) {
        if (p->GetAim()->IsTarget()) p->AimTargetRotate(ROTATE_RATIO);
        else if (p->GetCommand()->CmdWalk()) p->Rotate(ROTATE_RATIO);
    }
    
    p->CalcNoMove();
    p->FrontMove(MOVE_SPEED);
    if (p->GetCommand()->CmdAtk()) next_ = true;

    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetSegmentCollider()->SetValid(false);
    if (time_ >= ANIM_ATTACK_FRAME3[0] && time_ <= ANIM_ATTACK_FRAME3[1])
    m->CalcDamage();

    time_++;
    if (time_ >= COMBO_TIME3) {
        GameManager::GetEnemyManager()->ResetAllEnemyCollider();
        if (next_ == true) {
            GameManager::GetEnemyManager()->ResetAllEnemyCollider();
            owner_->ChangeState("Combo1");
        }
        else m->ResetState();
    }
}

void TestWeaponCombo3::OnEnter()
{
    time_ = 0;
    next_ = false;
    Player* p = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    Model::SetAnimFrame(p->GetModelHandle(), ANIM_FRAME3[0], ANIM_FRAME3[1], 1.0f);

}

void TestWeaponCombo3::OnExit()
{
    TestWeaponMain* m = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    m->GetPolyLine()->ResetPosition();

}