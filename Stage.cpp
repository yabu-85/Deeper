#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

//Ray�̕����p�̂Ă��Ƃ悤�Ł[��
#include "Player.h"

namespace {
    const float playerRadius = 1.0f;
    const float boxSize = 10.0f;
    const int maxCount = 8;
    
    //�����W�ȏ�͂���Ȃ�����
    int boxCount[maxCount] = { 0,0,0,0,0,0,0,0 };


}


Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
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
        hModel_[i] = Model::Load("Model/" + fileName[i] + ".fbx");
        assert(hModel_[i] >= 0);
    }

    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(25.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT1], XMFLOAT3(-25.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(10.0f, 0.0f, 0.0f)} );
    intersectDatas_.push_back( { hModel_[MAX + RT2], XMFLOAT3(-10.0f, 0.0f, 0.0f)} );

}

void Stage::Update()
{
    //�Ȃ񂩂����Ŏl�p�̒��̃|���S���擾�����肵����
    if (!Input::IsKeyDown(DIK_SPACE)) return;


    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();

    for (int i = 0; i < maxCount; i++) {

    }



    for (int i = 0; i < intersectDatas_.size(); i++) {
        Model::GetFbx(intersectDatas_[i].hModelNum);
    }

    //�v���C���[�̈ʒu���擾���āA���苗�����ɓ����������u���b�N���擾

    //�Ƃ���Fbx�f�[�^�̎w��|���S��������RayCast����֐��Ƃ�����Ĕ����������



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
    if (true) {
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