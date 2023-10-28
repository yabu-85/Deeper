#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "GameManager.h"
#include "EnemyBase.h"
#include "StateManager.h"
#include "PlayerState.h"
#include "PlayerCommand.h"
#include "Feet.h"

#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //�ړ��X�s�[�h�̉����̒l
    const float stopGradually = 0.25f;      //�ړ��X�s�[�h�̉����̒l�~�܂�Ƃ�
    const float moveGradually = 0.15f;      //�ړ��X�s�[�h�̉����̒l�ړ���
    const float maxMoveSpeed = 1.0f;        //�ő�ړ��X�s�[�h

    Text* pText = new Text;
    XMFLOAT3 prePos = XMFLOAT3(0.0f, 0.0f, 0.0f);
    
    float rotateTime = 0.0f;
}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_{-1, -1}, pAim_(nullptr), playerMovement_{0,0,0}, moveVec_(0, 0, 0), pStateManager_(nullptr), pCommand_(nullptr)
{
    moveSpeed_ = 0.1f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_[0] = Model::Load("Model/FiterTest2Up.fbx");
    assert(hModel_[0] >= 0);

    hModel_[1] = Model::Load("Model/FiterTest2Down.fbx");
    assert(hModel_[1] >= 0);
    transform_.rotate_.y += 180.0f;

    pCommand_ = new PlayerCommand();

    pStateManager_ = new StateManager(this);
    pStateManager_->AddState(new PlayerWait(pStateManager_));
    pStateManager_->AddState(new PlayerWalk(pStateManager_));
    pStateManager_->AddState(new PlayerAvo(pStateManager_));
    pStateManager_->ChangeState("Wait");
    pStateManager_->Initialize();

    pAim_ = Instantiate<Aim>(this);
    pText->Initialize();
}

void Player::Update()
{
    pCommand_->Update();
    pStateManager_->Update();

    Feet* pFeet = (Feet*)FindObject("Feet");
    if (pCommand_->CmdAtk() && pFeet) pFeet->ApplyDamage(4);

    //�G�C���^�[�Q�b�g
    if (pCommand_->CmdTarget()) pAim_->SetTargetEnemy();


    //�f�o�b�O�p�R�}���h
    if (Input::IsKeyDown(DIK_G)) {
        Model::SetBlendingAnimFrame(hModel_[1], 0, 120, 160, 1.0f, 0.9f);
    }

    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;
}

void Player::Draw()
{
    transform_.rotate_.y = pAim_->GetRotate().y + 180.0f;
    
    Model::SetTransform(hModel_[0], transform_);
    Model::Draw(hModel_[0]);

    //������]�����ɂ���Ă݂Ă�
    {
        XMVECTOR moveVec = XMLoadFloat3(&playerMovement_);
        XMVECTOR vFront{ 0,0,1,0 };
        XMVECTOR vDot = XMVector3Dot(vFront, moveVec);
        float dot = XMVectorGetX(vDot);
        float angle = (float)acos(dot);
        XMVECTOR vCross = XMVector3Cross(vFront, moveVec);
        if (XMVectorGetY(vCross) < 0) {
            angle *= -1;
        }
        transform_.rotate_.y = XMConvertToDegrees(angle);
    }
    
    Model::SetTransform(hModel_[1], transform_);
    Model::Draw(hModel_[1]);

    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);
}

void Player::Release()
{
}

void Player::Move(float f)
{
    prePos = transform_.position_;
    transform_.position_.x += ((playerMovement_.x * moveSpeed_) * f);
    transform_.position_.z += ((playerMovement_.z * moveSpeed_) * f);

    //�i�s��������������
    if (pCommand_->CmdWalk()) {
        XMFLOAT3 fAimPos = XMFLOAT3(prePos.x - transform_.position_.x, 0.0f, prePos.z - transform_.position_.z);
        XMVECTOR vAimPos = XMLoadFloat3(&fAimPos);  //���K���p�̕ϐ���float�̒l������
        vAimPos = XMVector3Normalize(vAimPos);
        XMVECTOR vFront{ 0,0,1,0 };
        XMVECTOR vDot = XMVector3Dot(vFront, vAimPos);
        float dot = XMVectorGetX(vDot);
        float angle = (float)acos(dot);

        //�O�ϋ��߂Ĕ���]�������� angle �� -1 �|����
        XMVECTOR vCross = XMVector3Cross(vFront, vAimPos); //�O�ϋ��߂��� �O�ς̓x�N�g���^
        if (XMVectorGetY(vCross) < 0) {
            angle *= -1;
        }
        transform_.rotate_.y = XMConvertToDegrees(angle);
        transform_.rotate_.y += 180.0f;
    }
}

bool Player::IsMoveKeyPushed(XMFLOAT3& key)
{
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();

    bool flag = false;
    if (pCommand_->CmdUp()) {
        key.x += aimDirection.x;
        key.z += aimDirection.z;
        flag = true;
    }
    if (pCommand_->CmdLeft()) {
        key.x -= aimDirection.z;
        key.z += aimDirection.x;
        flag = true;
    }
    if (pCommand_->CmdDown()) {
        key.x -= aimDirection.x;
        key.z -= aimDirection.z;
        flag = true;
    }
    if (pCommand_->CmdRight()) {
        key.x += aimDirection.z;
        key.z -= aimDirection.x;
        flag = true;
    }
    return flag;
}

bool Player::IsMove()
{
    return XMVector3NotEqual(XMLoadFloat3(&playerMovement_), XMVectorZero());
}

XMVECTOR Player::GetDirectionVec()
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    return vMove;
}

void Player::CalcMove()
{
    //fMove�̒l����違 gradually��ݒ�
    gradually = stopGradually;
    XMFLOAT3 fMove = { 0,0,0 };
    if (IsMoveKeyPushed(fMove)) gradually = moveGradually;

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);
    moveVec_ = fMove;

    fMove = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };

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
    XMFLOAT3 fMove = { 0,0,0 };
    gradually = stopGradually;
    moveVec_ = fMove;
    fMove = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };
}

void Player::InitAvo()
{
    if (pCommand_->CmdWalk()) {
        CalcMove();
        XMStoreFloat3(&playerMovement_, GetMoveVec());
    }
    else {
        XMStoreFloat3(&playerMovement_, GetDirectionVec() * maxMoveSpeed * -1.0);
    }
}