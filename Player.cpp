#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "StateManager.h"
#include "PlayerState.h"
#include "PlayerCommand.h"
#include "TestWeaponMain.h"
#include "TestWeaponSub.h"
#include "Engine/BoxCollider.h"

#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //�ړ��X�s�[�h�̉����̒l
    const float stopGradually = 0.25f;      //�ړ��X�s�[�h�̉����̒l�~�܂�Ƃ�
    const float moveGradually = 0.15f;      //�ړ��X�s�[�h�̉����̒l�ړ���
    const float maxMoveSpeed = 1.0f;        //�ő�ړ��X�s�[�h
    const float avoRotateRatio = 0.92f;     //�������RotateRatio

    Text* pText = new Text;
    XMFLOAT3 rotateMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
    
}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_{-1, -1}, pAim_(nullptr), playerMovement_{0,0,0}, pStateManager_(nullptr),
    pCommand_(nullptr), pMainWeapon_(nullptr), pSubWeapon_{ nullptr, nullptr }, money_(0), hp_(0), maxHp_(0), currentSubIndex_(0)
    , moveSpeed_(0.0f), rotateRatio_(0.0f)
{
}

Player::~Player()
{
    delete pStateManager_;
    delete pCommand_;
}

void Player::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_[0] = Model::Load("Model/FiterTestUp.fbx");
    assert(hModel_[0] >= 0);

    hModel_[1] = Model::Load("Model/FiterTestDown.fbx");
    assert(hModel_[1] >= 0);
    transform_.rotate_.y += 180.0f;

    pMainWeapon_ = Instantiate<TestWeaponMain>(this);
    pMainWeapon_->SetOffsetScale(XMFLOAT3(0.1f, 1.0f, 0.1f));
    pMainWeapon_->SetOffsetRotate(XMFLOAT3(0.0f, 0.0f, 0.0f));

    pAim_ = Instantiate<Aim>(this);
    pCommand_ = new PlayerCommand();

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new PlayerWait(pStateManager_));
    pStateManager_->AddState(new PlayerWeaponChange(pStateManager_));
    pStateManager_->AddState(new PlayerWalk(pStateManager_));
    pStateManager_->AddState(new PlayerAvo(pStateManager_));
    pStateManager_->AddState(new PlayerAtk(pStateManager_));
    pStateManager_->AddState(new PlayerSubAtk(pStateManager_));
    pStateManager_->AddState(new PlayerDead(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    maxHp_ = 60;
    hp_ = maxHp_;
    moveSpeed_ = 0.15f;
    rotateRatio_ = 0.2f;

    BoxCollider* collider = new BoxCollider(XMFLOAT3(0.0f, 1.5f, 0.0f), XMFLOAT3(0.5f, 2.0f, 0.5f));
    AddCollider(collider);

    pText->Initialize();
}

void Player::Update()
{
    pCommand_->Update();
    pStateManager_->Update();

    //�G�C���^�[�Q�b�g
    if (pCommand_->CmdTarget()) pAim_->SetTargetEnemy();

    //�f�o�b�O�p
    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;
    if (Input::IsKey(DIK_H)) ApplyDamage(1);
}

void Player::Draw()
{
    Model::SetTransform(hModel_[0], transform_);
    Model::Draw(hModel_[0]);
    Model::SetTransform(hModel_[1], transform_);
    Model::Draw(hModel_[1]);

    CollisionDraw();

    //����̕\����\��
    if (currentSubIndex_ == 0) {
        if (pSubWeapon_[0]) pSubWeapon_[0]->Visible();
        if (pSubWeapon_[1]) pSubWeapon_[1]->Invisible();
    }
    else {
        if (pSubWeapon_[1]) pSubWeapon_[1]->Visible();
        if (pSubWeapon_[0]) pSubWeapon_[0]->Invisible();
    }


    //�f�o�b�O�p
    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);
    
    pText->Draw(1100, 30, money_);
    pText->Draw(1100, 70, hp_);
    pText->Draw(1150, 70, " / ");
    pText->Draw(1200, 70, maxHp_);
    
    pText->Draw(1000, 600, currentSubIndex_);
    if (pSubWeapon_[0]) {
        const char* cstr = pSubWeapon_[0]->GetObjectName().c_str();
        pText->Draw(1000, 650, cstr);
    }
    if (pSubWeapon_[1]) {
        const char* cstr = pSubWeapon_[1]->GetObjectName().c_str();
        pText->Draw(1000, 700, cstr);
    }
}

void Player::Release()
{
}

void Player::Rotate()
{
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b = XMFLOAT2(rotateMove.x, rotateMove.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);

    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
}

void Player::Rotate(float ratio)
{
    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(transform_.rotate_.y)), cosf(XMConvertToRadians(transform_.rotate_.y)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b = XMFLOAT2(rotateMove.x, rotateMove.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b));
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);

    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * ratio);
}

XMFLOAT3 Player::GetInputMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    if (pCommand_->CmdWalk()) {
        gradually = moveGradually;

        XMFLOAT3 aimDirection = pAim_->GetAimDirection();
        if (pCommand_->CmdUp()) {
            fMove.x += aimDirection.x;
            fMove.z += aimDirection.z;
        }
        if (pCommand_->CmdLeft()) {
            fMove.x -= aimDirection.z;
            fMove.z += aimDirection.x;
        }
        if (pCommand_->CmdDown()) {
            fMove.x -= aimDirection.x;
            fMove.z -= aimDirection.z;
        }
        if (pCommand_->CmdRight()) {
            fMove.x += aimDirection.z;
            fMove.z -= aimDirection.x;
        }
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);

    return fMove;
}

void Player::FrontMove(float f)
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    vMove = XMVector3Normalize(vMove + XMLoadFloat3(&playerMovement_));
    XMFLOAT3 move{};
    XMStoreFloat3(&move, vMove);

    transform_.position_.x += ((move.x * moveSpeed_) * f);
    transform_.position_.z += ((move.z * moveSpeed_) * f);
}

void Player::Move(float f)
{
    transform_.position_.x += ((playerMovement_.x * moveSpeed_) * f);
    transform_.position_.z += ((playerMovement_.z * moveSpeed_) * f);
}

void Player::ApplyDamage(int da)
{
    hp_ -= da;
    if (hp_ <= 0) {
        pStateManager_->ChangeState("Dead");
    }
}

XMVECTOR Player::GetDirectionVec()
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    return vMove;
}

void Player::CalcRotate()
{
    rotateMove = GetInputMove();
}

void Player::CalcMove()
{
    gradually = stopGradually;

    XMFLOAT3 fMove = GetInputMove();
    rotateMove = fMove;

    XMFLOAT3 move = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + move.x , 0.0f , playerMovement_.z + move.z };

    //MaxSpeed�����Ă����琳�K���EMaxSpeed�̒l�ɂ���
    float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerMovement_)));
    if (currentSpeed > maxMoveSpeed) {
        XMVECTOR vMove;
        vMove = XMLoadFloat3(&playerMovement_);
        vMove = XMVector3Normalize(vMove);
        vMove *= maxMoveSpeed;
        XMStoreFloat3(&playerMovement_, vMove);
    }
}

void Player::CalcNoMove()
{
    XMFLOAT3 move = { 0,0,0 };
    gradually = stopGradually;
    move = { ((move.x - playerMovement_.x) * gradually) , 0.0f , ((move.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + move.x , 0.0f , playerMovement_.z + move.z };
}

void Player::InitAvo()
{
    //�����Ă���ꍇ�͑�̂��̕���
    if (pCommand_->CmdWalk()) {
        CalcMove();
        Rotate(avoRotateRatio);
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
    }
    //�����Ă��Ȃ��āA�^�[�Q�b�g��Ԃ̎��͌����Ă�������̋t��
    else if(pAim_->IsTarget()) {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed * -1.0);
        rotateMove = XMFLOAT3(-rotateMove.x, -rotateMove.y, -rotateMove.z);
    }
    //�����Ă��Ȃ��āA���^�[�Q�b�g�����Ă��Ȃ����͌����Ă��������
    else {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed);
    }

}

void Player::SetWeapon(WeaponBase* weapon)
{
    //�󂢂Ă�����ǉ�����
    if (pSubWeapon_[0] == nullptr) {
        pSubWeapon_[0] = weapon;
        return;
    }
    else if (pSubWeapon_[1] == nullptr) {
        pSubWeapon_[1] = weapon;
        return;
    }
    
    //�㏑������
    pSubWeapon_[currentSubIndex_]->KillMe();
    pSubWeapon_[currentSubIndex_] = weapon;
}

void Player::WeaponChangeIndex()
{
    if (pCommand_->CmdCenterUp()) {
        currentSubIndex_ = 0;
        return;
    }
    if (pCommand_->CmdCenterDown()) {
        currentSubIndex_ = 1;
        return;
    }

}

void Player::SubWeaponRemove()
{
    pSubWeapon_[currentSubIndex_]->KillMe();
    pSubWeapon_[currentSubIndex_] = nullptr;
}
