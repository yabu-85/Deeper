#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"

#include "Engine/Text.h"

namespace {
    float gradually = 0.1f;                 //�ړ��X�s�[�h�̉����̒l
    const float stopGradually = 0.25f;      //�ړ��X�s�[�h�̉����̒l�~�܂�Ƃ�
    const float moveGradually = 0.15f;      //�ړ��X�s�[�h�̉����̒l�ړ���
    const float maxMoveSpeed = 1.0f;        //�ő�ړ��X�s�[�h

    Text* pText = new Text;

}

Player::Player(GameObject* parent)
    : GameObject(parent, "Player"), hModel_(-1), pAim_(nullptr), playerMovement_{0,0,0}, moveVec_(0,0,0)
{
    moveSpeed_ = 0.15f;
}

Player::~Player()
{
}

void Player::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Model/SphereCollider.fbx");
    assert(hModel_ >= 0);

    transform_.scale_ = { 8.0f, 8.0f, 8.0f };

    pAim_ = Instantiate<Aim>(this);
    
    pText->Initialize();

}

void Player::Update()
{
    CalcMove();

    XMFLOAT3 prePos = transform_.position_;
    if (playerMovement_.x != 0.0f || playerMovement_.z != 0.0f) {

        //�����蔻���肽��
        //���̎O�p�`�Ɖ~�̏Փ˔���炵���E�Q�[���ɂ͍���Ȃ���

        /*        �@
        ���̏��ɔ��肵�܂��B
        �O�p�`���܂ޕ��ʂƋ��̒��S�Ƃ̋��������̔��a���z���Ă����� ������Ɣ��肵�܂��B
        ���̒��S����O�p�`���܂ޕ��ʂ։��낵������ p+t(-n)�A �O�p�`�� (1-u-v)q0+uq1+vq2 �ŕ\���܂� (t, u, v �͔}��ϐ��An �͎O�p�`�̖@��)�B
        Tomas Moller �̃A���S���Y��
        [n (q1-q0) (q2-q0)]	|t|
        |u|
        |v|
        =	(p-q0)
        �� Cramer �̌����ŉ����܂��B 0.0��u, 0.0��v, u+v��1.0 �Ȃ�����Ɣ��肵�܂��B
        ���̒��S�ƎO�p�`�̍ŒZ���������߂āA���̔��a������������Ό����Ɣ��肵�܂��B
        �ŒZ�������Ƃ��āA�O�p�`��3���_�Ƌ��̒��S�Ƃ̋����̍ŏ��l�A �������͎O�p�`�̊e�ӂƋ��̒��S�Ƃ̋����̍ŏ��l������܂��B
        u<0.0, 1.0<v, 1.0<u+v �̎� q2 �Ƌ��̒��S�Ƃ̋���
        v<0.0, 1.0<u, 1.0<u+v �̎� q1 �Ƌ��̒��S�Ƃ̋���
        u<0.0, v<0.0, u+v<1.0 �̎� q0 �Ƌ��̒��S�Ƃ̋���
        ���ŒZ�����ɂȂ�܂��B ������ł��Ȃ����A
        u<0.0 �Ȃ����q0q1 �Ƌ��̒��S�Ƃ̋����A
        v<0.0 �Ȃ����q0q2�Ƌ��̒��S�Ƃ̋����A
        1.0<u+v �Ȃ����q1q2�Ƌ��̒��S�Ƃ̋���
        ���ŒZ�����ɂȂ�܂��B �������A���� u, v �̒l�͈̔͂ɂ��ꍇ�����́A
        �O�p�`�ɓ݊p���܂܂�Ă���Ɣj�]���邽�߁A�{���m�C�̈�������ɋ��߂邩�A �S�ċ��߂čŏ��l������������ǂ��ł��B
        
        �����������������߁A�v�Z�����������A�Q�[���Ȃǂł̎g�p�͔���p�I�ł��B
        */

    }

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
    Model::Draw(hModel_, 2);

    pText->Draw(30, 30, transform_.position_.x);
    pText->Draw(30, 70, transform_.position_.y);
    pText->Draw(30, 110, transform_.position_.z);


}

void Player::Release()
{
}

bool Player::GetMoveVec(XMVECTOR& vec)
{
    vec = XMLoadFloat3(&moveVec_);
    return XMVector3NotEqual(vec, XMVectorZero());
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
