#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "GameManager.h"
#include "EnemyBase.h"

#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //�ړ��X�s�[�h�̉����̒l
    const float stopGradually = 0.25f;      //�ړ��X�s�[�h�̉����̒l�~�܂�Ƃ�
    const float moveGradually = 0.15f;      //�ړ��X�s�[�h�̉����̒l�ړ���
    const float maxMoveSpeed = 1.0f;        //�ő�ړ��X�s�[�h

    Text* pText = new Text;

}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), pAim_(nullptr), playerMovement_{0,0,0}, moveVec_(0,0,0), isAvo_(false)
{
    vDirection_ = XMVectorZero();
    moveSpeed_ = 0.2f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Model/PlayerTest.fbx");
    assert(hModel_ >= 0);
    Model::SetAnimFrame(hModel_, 0, 40, 1);

    transform_.scale_ = { 8.0f, 8.0f, 8.0f };
    transform_.scale_ = { 1.0f, 1.0f, 1.0f };

   // transform_.scale_ = { 0.2f, 0.2f, 0.2f };

    pAim_ = Instantiate<Aim>(this);
    
    pText->Initialize();

}

void Player::Update()
{
    //�G�C���^�[�Q�b�g
    if (Input::IsKeyDown(DIK_Q)) pAim_->SetTargetEnemy();

    //�������Ă�
    if (Input::IsKeyDown(DIK_SPACE)) {
        isAvo_ = true;
        if (IsMoveKeyPushed()) {

        }
        else {

        }

    }
    if (isAvo_) {

    }

    CalcMove();         //�ړ��ʌv�Z

    transform_.position_.x += ((playerMovement_.x * moveSpeed_));
    transform_.position_.z += ((playerMovement_.z * moveSpeed_));



    if (Input::IsKey(DIK_UPARROW)) transform_.position_.y += 0.1f;
    if (Input::IsKey(DIK_DOWNARROW)) transform_.position_.y -= 0.1f;
    if (Input::IsKeyDown(DIK_LEFTARROW)) transform_.position_.y = 0.0f;
    if (Input::IsKeyDown(DIK_RIGHTARROW)) transform_.position_.y += 10.0f;
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    pText->Draw(30, 30, (int)transform_.position_.x);
    pText->Draw(30, 70, (int)transform_.position_.y);
    pText->Draw(30, 110, (int)transform_.position_.z);

}

void Player::Release()
{
}

bool Player::IsMoveKeyPushed()
{
    if (Input::IsKey(DIK_W) || Input::IsKey(DIK_A) || Input::IsKey(DIK_S) || Input::IsKey(DIK_D)) return true;

    return false;
}

bool Player::IsMove()
{
    return XMVector3NotEqual(XMLoadFloat3(&playerMovement_), XMVectorZero());
}

void Player::CalcMove()
{
    XMFLOAT3 fMove = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();
    moveVec_ = { 0,0,0 };
    gradually = stopGradually;

    // Player�N���X��Move�֐����̈ꕔ
    if (Input::IsKey(DIK_W)) {
        fMove.x += aimDirection.x;
        fMove.z += aimDirection.z;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_A)) {
        fMove.x -= aimDirection.z;
        fMove.z += aimDirection.x;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_S)) {
        fMove.x -= aimDirection.x;
        fMove.z -= aimDirection.z;
        gradually = moveGradually;
    }
    if (Input::IsKey(DIK_D)) {
        fMove.x += aimDirection.z;
        fMove.z -= aimDirection.x;
        gradually = moveGradually;
    }

    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    XMStoreFloat3(&fMove, vMove);
    moveVec_ = fMove;

    fMove = { ((fMove.x - playerMovement_.x) * gradually) , 0.0f , ((fMove.z - playerMovement_.z) * gradually) };
    playerMovement_ = { playerMovement_.x + fMove.x , 0.0f , playerMovement_.z + fMove.z };

    float currentSpeed = XMVectorGetX(XMVector3Length(XMLoadFloat3(&playerMovement_)));

    //MaxSpeed�����Ă����琳�K���EMaxSpeed�̒l�ɂ���
    if (currentSpeed > maxMoveSpeed) {
        XMVECTOR vMove;
        vMove = XMLoadFloat3(&playerMovement_);
        vMove = XMVector3Normalize(vMove);
        vMove *= maxMoveSpeed;

        XMStoreFloat3(&playerMovement_, vMove);
    }

}
