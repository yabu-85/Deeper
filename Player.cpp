#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Global.h"
#include "Aim.h"
#include "StateManager.h"
#include "PlayerState.h"
#include "PlayerCommand.h"
#include "PlayerWeapon.h"
#include "GameManager.h"
#include "CreateStage.h"
#include "LifeManager.h"
#include "VFXManager.h"

#include "Engine/BoxCollider.h"
#include "Engine/SphereCollider.h"
#include "CollisionMap.h"
#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //�ړ��X�s�[�h�̉����̒l
    const float stopGradually = 0.25f;      //�ړ��X�s�[�h�̉����̒l�~�܂�Ƃ�
    const float moveGradually = 0.15f;      //�ړ��X�s�[�h�̉����̒l�ړ���
    const float maxMoveSpeed = 1.0f;        //�ő�ړ��X�s�[�h
    const float avoRotateRatio = 0.92f;     //�������RotateRatio

    bool isCollider = true; //�����蔻�肷�邩�ǂ����A������悤�ɂ��邽�߂�

    Text* pText = new Text;
    XMFLOAT3 rotateMove = XMFLOAT3(0.0f, 0.0f, 0.0f);
    SphereCollider* collid = nullptr;

    int enterTime;
    XMFLOAT3 enterPos = XMFLOAT3();
}

Player::Player(GameObject* parent)
    : Character(parent, "Player"), hModel_{-1, -1}, pAim_(nullptr), pStateManager_(nullptr), pCommand_(nullptr), pPlayerWeapon_(nullptr),
    moveSpeed_(0.0f), rotateRatio_(0.0f), playerMovement_(0,0,0)
{
}

Player::~Player()
{
    SAFE_DELETE(pStateManager_);
    SAFE_DELETE(pCommand_);

}

void Player::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_[0] = Model::Load("Model/FiterTestUp.fbx");
    assert(hModel_[0] >= 0);

    hModel_[1] = Model::Load("Model/FiterTestDown.fbx");
    assert(hModel_[1] >= 0);
    transform_.rotate_.y += 180.0f;

    SetPosition(GameManager::GetCreateStage()->GetPlayerStartPos());
    moveSpeed_ = 0.08f;
    rotateRatio_ = 0.2f;
    bodyWeight_ = 5.1f;
    enterTime = 60;
    transform_.scale_ = { 0.5f, 0.5f, 0.5f };

    pAim_ = Instantiate<Aim>(this);
    pCommand_ = new PlayerCommand();
    pPlayerWeapon_ = new PlayerWeapon(this);
    pPlayerWeapon_->SetPlayerDataWeapon();

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

    //BoxCollider* collider = new BoxCollider(XMFLOAT3(0.0f, 1.3f, 0.0f), XMFLOAT3(0.5f, 2.6f, 0.5f));
    //AddCollider(collider);
    collid = new SphereCollider(XMFLOAT3(0.0f, 1.2f, 0.0f), 0.25f);
    AddCollider(collid);

    enterPos = transform_.position_;
    transform_.position_.y += (enterTime * 0.5f);

    pText->Initialize();


}

void Player::Update()
{
    //Case�i�Y
    if (enterTime > 0) {
        enterTime--;
        transform_.position_.y -= 0.5f;
        
        XMFLOAT3 cPos = XMFLOAT3(enterPos.x, enterPos.y + 5.0f, enterPos.z + 13.0f);
        pAim_->SetCompulsion(cPos, enterPos);

        if (enterTime == 0) {
            XMFLOAT3 pos = { transform_.position_.x, transform_.position_.y + 0.7f, transform_.position_.z };
            VFXManager::CreatVfxExplode1(pos);
        }

        return;
    }

    pCommand_->Update();
    pStateManager_->Update();

    //�G�C���^�[�Q�b�g
    if (pCommand_->CmdTarget()) pAim_->SetTargetEnemy();
    
    //�f�o�b�O�p
    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;

    if (Input::IsKeyDown(DIK_T)) isCollider = !isCollider;
    if (isCollider) {
        CollisionMap* map = static_cast<CollisionMap*>(FindObject("CollisionMap"));
        map->CalcMapWall(transform_.position_, moveSpeed_);
    }

}

void Player::Draw()
{
    Model::SetTransform(hModel_[0], transform_);
    Model::Draw(hModel_[0]);
    Model::SetTransform(hModel_[1], transform_);
    Model::Draw(hModel_[1]);
    pPlayerWeapon_->DrawWeapon();

    //�f�o�b�O�p
    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);

    CollisionDraw();

}

void Player::Release()
{
}

void Player::OnCollision(GameObject* pTarget)
{
    std::string name = pTarget->GetObjectName();
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        Character* c = static_cast<Character*>(pTarget);
        ReflectCharacter(c);
    }

}

void Player::OnAttackCollision(GameObject* pTarget)
{
    std::string name = pTarget->GetObjectName();
    if (pTarget->GetObjectName().find("Enemy") != std::string::npos) {
        LifeManager::Damage(10);

    }

}

void Player::Rotate()
{
    rotateMove = GetInputMove();

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
    rotateMove = GetInputMove();

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

void Player::AimTargetRotate()
{
    XMFLOAT3 tar = pAim_->GetTargetPos();
    float rotateY = transform_.rotate_.y;

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(rotateY)), cosf(XMConvertToRadians(rotateY)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b(transform_.position_.x - tar.x, transform_.position_.z - tar.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b)) * -1.0f;
    XMStoreFloat2(&a, vA);
    XMStoreFloat2(&b, vB);
    float cross = a.x * b.y - a.y * b.x;
    float dot = a.x * b.x + a.y * b.y;
    transform_.rotate_.y += XMConvertToDegrees(-atan2f(cross, dot) * rotateRatio_);
}

void Player::AimTargetRotate(float ratio)
{
    XMFLOAT3 tar = pAim_->GetTargetPos();
    float rotateY = transform_.rotate_.y;

    XMFLOAT2 a = XMFLOAT2(sinf(XMConvertToRadians(rotateY)), cosf(XMConvertToRadians(rotateY)));
    XMVECTOR vA = XMVector2Normalize(XMLoadFloat2(&a));
    XMFLOAT2 b(transform_.position_.x - tar.x, transform_.position_.z - tar.z);
    XMVECTOR vB = XMVector2Normalize(XMLoadFloat2(&b)) * -1.0f;
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
    SetMovement((XMLoadFloat3(&move) * moveSpeed_) * f);
}

void Player::Move(float f)
{
    transform_.position_.x += ((playerMovement_.x * moveSpeed_) * f);
    transform_.position_.z += ((playerMovement_.z * moveSpeed_) * f);
    SetMovement((XMLoadFloat3(&playerMovement_) * moveSpeed_) * f);
}

XMVECTOR Player::GetDirectionVec()
{
    XMVECTOR vMove = { 0.0, 0.0, 1.0, 0.0 };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove = XMVector3Normalize(vMove);
    return vMove;
}

void Player::ResetMovement()
{
    movement_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    playerMovement_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
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