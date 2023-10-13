#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

//Ray�̕����p�̂Ă��Ƃ悤�Ł[��
#include "Player.h"
#include "Engine/Text.h"
#include <vector>
#include "Cell.h"
#include "Engine/FbxParts.h"

namespace {
    const float playerRadius = 1.0f;
    const float boxSize = 10.0f;
    const int maxCount = 8;

    Text *pText = new Text;
    Quad* pQuad = new Quad;
    Cell* pCell = new Cell;

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

    pText->Initialize();
	pQuad->Initialize();
}

void Stage::Update()
{
    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();


    //�v���C���[�̈ʒu���擾���āA���苗�����ɓ����������u���b�N���擾
    //Block�̒ǉ��͂ł������ǁA���a�E�����͂܂�����ĂȂ�
    float fBox[3] = { plaPos.x / boxSize, plaPos.y / boxSize, plaPos.z / boxSize };
    int iBox[3] = { fBox[0], fBox[1], fBox[2] };
    for (int i = 0; i < 3; i++) if (fBox[i] < 0) iBox[i] -= 1;
    for(int i = 0;i < 3;i++) iBox[i] *= 10;
    XMFLOAT3 cellPos = XMFLOAT3((float)iBox[0], 0.0f, (float)iBox[2]);

    pQuad->SetPosition(cellPos);
    pCell->SetPosLeng(cellPos, boxSize);
    pCell->ResetTriangles();

	//Block�͈͓̔��̃|���S�����擾������
    for (int i = 0; i < intersectDatas_.size(); i++) {
        Fbx* pFbx = Model::GetFbx(intersectDatas_[i].hModelNum);
        std::vector<FbxParts*> pFbxParts = pFbx->GetFbxParts();
    
        for (int n = 0; n < pFbxParts.size(); n++) {
            int size = (int)pFbxParts[n]->GetVertexCount();

            for (int j = 0; j < size; j++) {
                XMFLOAT3 verPos = pFbxParts[n]->GetVertexPosition(j);
                XMFLOAT3 interPos = intersectDatas_[i].position;
                verPos = { verPos.x + interPos.x, verPos.y + interPos.y, verPos.z + interPos.z };
                
                //�Ƃ肠�����������Œǉ����Ă݂�->Cell�̂ق��ŏ���������Ă�
                Triangle tri;
                XMFLOAT3* pori;
                tri.SetPosition(&verPos);
                
                pCell->SetTriangle(tri);
            }
        }
    }

    std::vector<Triangle*> triList = pCell->GetTriangles();
    int aaaa = 0;

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

	if(true)
	pQuad->Draw();

    Player* pPlayer = (Player*)FindObject("Player");
    XMFLOAT3 plaPos = pPlayer->GetPosition();
    pText->Draw(30, 30, plaPos.x);
    pText->Draw(30, 70, plaPos.y);
    pText->Draw(30, 110, plaPos.z);

}

void Stage::Release()
{
}