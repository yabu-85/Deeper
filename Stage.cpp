#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include <vector>

namespace {
    bool drawCell = true;

}

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), startPos_(0,0,0), goalPos_(0,0,0)
{
    for (int i = 0; i < MAX + RMAX; i++) hModel_[i] = -1;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    std::string fileName[] = { "StageT1", "StageT2", "RayStageT1", "RayStageT2" };
    for (int i = 0; i < MAX + RMAX; i++) {
        hModel_[i] = Model::Load("Model/Stage/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }
    
    startPos_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    goalPos_ = XMFLOAT3( 50.0f * (float)(rand() % 2 - 1) , 0.0f, 50.0f + (float)(rand() % 50));

    CreatStage();
}

void Stage::Update()
{
    if (Input::IsKeyDown(DIK_E)) drawCell = !drawCell;

}

void Stage::Draw()
{
    for (int i = 0; i < intersectDatas_.size(); i++) {
        int handle = intersectDatas_.at(i).hModelNum - MAX;
        Transform trans;
        trans.position_ = intersectDatas_.at(i).position;
        Model::SetTransform(handle, trans);
        Model::Draw(handle, 0);
    }

    //Ray�p�̃��C���[�\��
    if (drawCell) {
        for (int i = 0; i < intersectDatas_.size(); i++) {
            int handle = intersectDatas_.at(i).hModelNum;
            Transform trans;
            trans.position_ = intersectDatas_.at(i).position;
            Model::SetTransform(handle, trans);
            Model::Draw(handle, 2);
        }
    }

}

void Stage::Release()
{
}

void Stage::ResetStage()
{
}

//�E�H�[�t���[���̃}�b�v����������Č��悤���Ǝv���i�J�[�r�B�f�B�X�J�o���[�̃}�b�v��������肽����ˁF�ނ����������ǋL�������ς�����j
void Stage::CreatStage()
{
    //�G�̏o���ʒu�̃R���g���[���͈ȉ��̂悤�ɍs���܂��B
    //(1)�e�p�[�c�Ƀv���C���[���ړ��\�ȏꏊ���w�肳��A�}�b�v���g�ݏオ�������ɁA1�{�̃��C���Ńv���C���[�������\�������郋�[�g���C�����`�����
    //(2)�Q�[�����̓v���C���[������͈͂Ń��C���ɉ����ĉe���̓}�b�v����������A���͈͓̔��œG����������v���C���[���P��������

    //�G�̏o���̓^�C�����C���ɂ���Ɓu���[���[�R�[�X�^�[�̂悤�Ɂv���䂳��Ă��āA�ŏ���1�̂̓G�������̂��A���X�ɓG�̏P�����������Ȃ�
    //������s�[�N�ɒB���A�������|����Ǝb���͕��a���K���c�c�Ƃ������ɂȂ�܂��B
    //1�̃X�e�[�W�̏I���܂łɓ|���ׂ��G�̐������߂��Ă��āA����̕��𒴂���ƃv���C���[�̓S�[���ɒH�蒅�������o���܂��B

    
    //��邱�Ɓ[���܂��}�b�v��������[�����[�g���Ȃ��Ł[�����̏ꏊ�̉e���͂ɂ���ēG���X�|�[�������
    

    //�Ȃ񂩌������č�����X�^�[�g����S�[���܂Œ����Ă���
    XMVECTOR vFront{ 0,0,1,0 };
    XMVECTOR vStart = XMLoadFloat3(&startPos_);
    XMVECTOR vGoal = XMLoadFloat3(&goalPos_);
    XMVECTOR vDot = XMVector3Dot(vFront, XMVector3Normalize(vStart - vGoal));
    float angle = (float)acos(XMVectorGetX(vDot));
    XMVECTOR vCross = XMVector3Cross(vFront, vStart - vGoal);
    if (XMVectorGetY(vCross) < 0) {
        angle *= -1;
    }

    XMVECTOR vMove = { 0.0f, 0.0f, -XMVectorGetX(XMVector3Length(vStart - vGoal)) / 10, 0.0f };
    XMMATRIX mRotY = XMMatrixRotationY(angle);
    vMove = XMVector3TransformCoord(vMove, mRotY);

    XMVECTOR vRight = { 10, 0, 0, 0 };
    XMVECTOR direction = XMVector3TransformNormal(vRight, mRotY);

    XMFLOAT3 dataPos1 = XMFLOAT3(0.0f, 0.0f, 0.0f);
    XMFLOAT3 dataPos2 = XMFLOAT3(0.0f, 0.0f, 0.0f);
    for (int i = 1; i <= 10; i++) {
        XMStoreFloat3(&dataPos1, vStart + vMove * i + direction);
        XMStoreFloat3(&dataPos2, vStart + vMove * i - direction);
        intersectDatas_.push_back({ hModel_[MAX + RT2], dataPos1 });
        intersectDatas_.push_back({ hModel_[MAX + RT2], dataPos2 });
    }
    intersectDatas_.push_back({ hModel_[MAX + RT1], startPos_ });
    intersectDatas_.push_back({ hModel_[MAX + RT1], goalPos_ });

}
