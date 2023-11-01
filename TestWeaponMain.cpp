#include "TestWeaponMain.h"
#include "StateManager.h"
#include "Engine/Model.h"
#include "DamageCtrl.h"
#include "GameManager.h"
#include "Player.h"
#include "PlayerCommand.h"
#include "Engine/VFX.h"

TestWeaponMain::TestWeaponMain(GameObject* parent)
	:WeaponBase(parent)
{
	objectName_ = "TestWeaponMain";
}

TestWeaponMain::~TestWeaponMain()
{
    Release();
}

void TestWeaponMain::Initialize()
{
    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new TestWeaponWait(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo1(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo2(pStateManager_));
    pStateManager_->AddState(new TestWeaponCombo3(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Model/BlueBox.fbx");
    assert(hModel_ >= 0);

    pPlayer_ = (Player*)GetParent();
    GameManager* pGameManager = (GameManager*)FindObject("GameManager");
    pDamageCtrl_ = pGameManager->GetDamageCtrl();
}

void TestWeaponMain::Update()
{


}

void TestWeaponMain::Draw()
{
    //Draw�̒��O�ɂ��Ȃ��ƂЂƂO�̃t���[���̍��W���Ԃ��Ă���
    transform_.position_ = Model::GetBoneAnimPosition(pPlayer_->GetModelHandle(), "hand.L");
    
    XMMATRIX matrix = Model::GetBoneAnimRotateMatrix(pPlayer_->GetModelHandle(), "hand.L");
    XMFLOAT4X4 matrix4x4;

    // XMMATRIX��XMFLOAT4X4�ɕϊ�
    XMStoreFloat4x4(&matrix4x4, matrix);

    // XMFLOAT4X4�����]�����𒊏o
    XMFLOAT3 rotation;
    rotation.x = XMConvertToDegrees(atan2f(matrix4x4._32, matrix4x4._33)); // ���[��
    rotation.y = XMConvertToDegrees(asinf(-matrix4x4._31));                // �s�b�`
    rotation.z = XMConvertToDegrees(atan2f(matrix4x4._21, matrix4x4._11)); // ���[

    rotation.y -= pPlayer_->GetRotate().y;
    rotation.y *= -1.0f;
    transform_.rotate_ = rotation;

    Transform t = transform_;
    CalcOffset(t);
    Model::SetTransform(hModel_, t);
    Model::Draw(hModel_);

}

void TestWeaponMain::Release()
{
}

void TestWeaponMain::ResetState()
{
    atkEnd_ = true;
    pStateManager_->ChangeState("Wait");
}

void TestWeaponMain::CalcDamage(float range)
{
    XMFLOAT3 tar;
    tar.x = (float)sin(XMConvertToRadians(transform_.rotate_.y + offsetTrans_.rotate_.y));
    tar.y = -(float)tan(XMConvertToRadians(transform_.rotate_.x + offsetTrans_.rotate_.x));
    tar.z = (float)cos(XMConvertToRadians(transform_.rotate_.y + offsetTrans_.rotate_.y));

    XMFLOAT3 vec = tar;
    XMVECTOR vVec = XMLoadFloat3(&vec);
    vVec = XMVector3Normalize(vVec);
    XMStoreFloat3(&vec, vVec);
    pDamageCtrl_->CalcSword(transform_.position_, vec, range);

    EmitterData  data;
    data.position = transform_.position_;
    data.delay = 0;
    data.number = 1;
    data.lifeTime = 10;
    data.positionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data.direction = vec;
    data.directionRnd = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data.speed = data.lifeTime * 15 / 100;
    data.speedRnd = 0.0f;
    data.accel = 1.0f;
    data.size = XMFLOAT2(0.4f, 0.4f);
    data.sizeRnd = XMFLOAT2(0.4f, 0.4f);
    data.scale = XMFLOAT2(1.0f, 1.0f);
    data.color = XMFLOAT4(1.0f, 1.0f, 0.1f, 1.0f);
    data.deltaColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    data.gravity = 0.0f;
    VFX::Start(data);

}

//--------------------state---------------------------------------------------

TestWeaponWait::TestWeaponWait(StateManager* owner)
{
    owner_ = owner;
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
}

void TestWeaponWait::Update()
{
    if (!pTestWeaponMain_->IsAtkEnd()) owner_->ChangeState("Combo1");
}

//---------------------------------------------

TestWeaponCombo1::TestWeaponCombo1(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 40;
}

void TestWeaponCombo1::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if(next_ == true) owner_->ChangeState("Combo2");
        else {
            pTestWeaponMain_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }

    float fff = (float)time_ / (float)comboTime_;
    float rotateY = -30.0f + 60.0f * fff;
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(0.0f, rotateY, 0.0f));
    pTestWeaponMain_->CalcDamage();

}

void TestWeaponCombo1::OnEnter()
{
    if (pTestWeaponMain_ == nullptr) owner_->ChangeState("Wait");
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo1::OnExit()
{
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(0.0f, 0.0f, 0.0f));

}

//---------------------------------------------

TestWeaponCombo2::TestWeaponCombo2(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 25;
}

void TestWeaponCombo2::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo3");
        else {
            pTestWeaponMain_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }

    float fff = (float)time_ / (float)comboTime_;
    pTestWeaponMain_->SetScale(XMFLOAT3(2.0f * fff, 2.0f * fff, 2.0f * fff));
}

void TestWeaponCombo2::OnEnter()
{
    if (pTestWeaponMain_ == nullptr) owner_->ChangeState("Wait");
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo2::OnExit()
{
    pTestWeaponMain_->SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
}

//---------------------------------------------

TestWeaponCombo3::TestWeaponCombo3(StateManager* owner)
    :time_(0), next_(false)
{
    owner_ = owner;
    pTestWeaponMain_ = static_cast<TestWeaponMain*>(owner_->GetGameObject());
    pPlayer_ = static_cast<Player*>(owner_->GetGameObject()->GetParent());
    comboTime_ = 50;
}

void TestWeaponCombo3::Update()
{
    time_--;
    if (pPlayer_->GetCommand()->CmdAtk()) next_ = true;
    
    if (time_ <= 0) {
        if (next_ == true) owner_->ChangeState("Combo1");
        else {
            pTestWeaponMain_->SetAtkEnd(true);
            owner_->ChangeState("Wait");
        }
        return;
    }
    
    float fff = (float)time_ / (float)comboTime_;
    float rotateY = 60.0f - 120.0f * fff;
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(0.0f, rotateY, 0.0f));
    pTestWeaponMain_->CalcDamage(20.0f);

}

void TestWeaponCombo3::OnEnter()
{
    if (pTestWeaponMain_ == nullptr) owner_->ChangeState("Wait");
    time_ = comboTime_;
    next_ = false;
}

void TestWeaponCombo3::OnExit()
{
    pTestWeaponMain_->SetOffsetRotate(XMFLOAT3(0.0f, 0.0f, 0.0f));
}
