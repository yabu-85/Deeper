#include "CreateStage.h"
#include "CollisionMap.h"
#include "NavigationAI.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/CsvReader.h"
#include "../Engine/Global.h"
#include "../GameManager/GameManager.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/EnemyBase.h"
#include "../Scene/StageBase.h"
#include "../Player/Player.h"
#include "../Stage/Warp.h"
#include <vector>

CreateStage::CreateStage()
    : mapSizeX_(0), mapSizeZ_(0)
{
    for (int i = 0; i < MAX; i++) hModel_[i] = -1;
}

CreateStage::~CreateStage()
{
}

void CreateStage::Initialize()
{
    std::string fileName[MAX] = { "StageT1", "StageT2", "StageT3", "RayStageT1", "RayStageT2", "RayStageT3" };
    for (int i = 0; i < MAX; i++) {
        if (fileName[i] != "none") {
            hModel_[i] = Model::Load("Model/Stage/" + fileName[i] + ".fbx");
            assert(hModel_[i] >= 0);
        }
    }

}

void CreateStage::Draw()
{
    for (int i = 0; i < intersectDatas_.size(); i++) {
        int handle = intersectDatas_.at(i).hModelNum;
        Transform trans;
        trans.position_ = intersectDatas_.at(i).position;
        trans.scale_ = intersectDatas_.at(i).scale;
        Model::SetTransform(handle, trans);
        Model::Draw(handle, Direct3D::SHADER_SKYBOX);
    }
}

XMFLOAT3 CreateStage::GetRandomFloarPosition(float size)
{
    std::vector<Cell> posList;
    std::vector<std::vector<float>> mapS = GameManager::GetNavigationAI()->GetMapSize();
    
    for (int x = 0; x < mapSizeX_; x++) {
        for (int z = 0; z < mapSizeZ_; z++) {
            if (mapData_[z][x] == MAP::M_FLOAR && mapS[x][z] > size) {
                Cell cell;
                cell.x = x;
                cell.z = z;
                posList.push_back(cell);
            }
        }
    }

    if (posList.empty()) return XMFLOAT3((float)mapSizeX_ / 2.0f + 0.5f, (float)mapSizeZ_ / 2.0f + 0.5f, 0.0f);

    int index = rand() % posList.size();
    return XMFLOAT3((float)posList.at(index).x + 0.5f, 0.0f, (float)posList.at(index).z + 0.5f);
}

XMFLOAT3 CreateStage::GetPositionPlayerDirection(XMFLOAT3 position, float range)
{
    XMFLOAT3 pPos = GameManager::GetPlayer()->GetPosition();
    XMFLOAT3 dir = Float3Multiply(Float3Normalize(Float3Sub(position, pPos)), range);
    pPos = Float3Add(pPos, dir);

    int pos[2] = { 0,0 };
    pos[0] = static_cast<int>(pPos.x);
    pos[1] = static_cast<int>(pPos.z);
    int size = 0;
    int max = 5;
    int maxCount = 0;

    while (maxCount < max) {
        std::vector<Cell> cellList;

        //size�͈̔͂ŏ�������΃��X�g�ɒǉ�
        for (int i = pos[0] - size; i <= pos[0] + size; ++i) {
            for (int j = pos[1] - size; j <= pos[1] + size; ++j) {
                if (i < 0 || i >= (int)mapData_[0].size() || j < 0 || j >= (int)mapData_.size())
                    continue;

                if (mapData_[j][i] == MAP::M_FLOAR) {
                    Cell c(i, j);
                    cellList.push_back(c);
                }
            }
        }

        //���X�g���炶��Ȃ���΂��̒����烉���_���őI�ѕԂ�
        if (!cellList.empty()) {
            int randamIndex = rand() % cellList.size();
            XMFLOAT3 out = XMFLOAT3((float)cellList.at(randamIndex).x, 0.0f, (float)cellList.at(randamIndex).z);
            out = XMFLOAT3(out.x + 0.5f, out.y, out.z + 0.5f);
            return out;
        }

        size++;
        maxCount++;
    }

    return position;
}

void CreateStage::CreateStageData(std::string name)
{
    Initialize();

    //CSV�t�@�C���ǂݍ���
    CsvReader csv;
    csv.Load(name);

    //�X�e�[�W�̕��ƍ�����ݒ�
    mapSizeX_ = (int)csv.GetWidth();
    mapSizeZ_ = (int)csv.GetHeight();
    intersectDatas_.clear();

    //�T�C�Y���}�b�v�̑傫���ɂ���
    mapData_.resize(mapSizeZ_);
    for (int i = 0; i < mapSizeZ_; i++) mapData_[i].resize(mapSizeX_, FLOAR);

    //���̃T�C�Y���}�b�v�T�C�Y�ɍ��킹�Đ���
    intersectDatas_.push_back({ hModel_[FLOAR], hModel_[R_FLOAR], XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3((float)mapSizeX_, 1.0f, (float)mapSizeZ_) });

    //CSV�f�[�^���e�[�u���Ɋi�[
    for (int x = 0; x < mapSizeX_; x++) {
        for (int z = 0; z < mapSizeZ_; z++) {
            int data = csv.GetValue(x, z);
            mapData_[z][x] = FLOAR;

            //Wall
            if (data == 2)
            {
                mapData_[z][x] = WALL;
                intersectDatas_.push_back({ hModel_[WALL], hModel_[R_WALL], XMFLOAT3((float)x, 0.0f, (float)z), XMFLOAT3(1.0f, 1.0f, 1.0f) });
            }
            
            //SmallWall
            if (data == 3)
            {
                mapData_[z][x] = WALL;
                intersectDatas_.push_back({ hModel_[SMALL_WALL], hModel_[R_SMALL_WALL], XMFLOAT3((float)x, 0.0f, (float)z), XMFLOAT3(1.0f, 1.0f, 1.0f) });
            }

            //PlayerStartPoint
            if (data == 10)
            {
                StageBase* stage = static_cast<StageBase*>(GameManager::GetStage());
                if(stage) stage->SetStartPosition( {(float)x + 0.5f, 0.0f, (float)z + 0.5f } );
            }

            //WarpPoint
            if (data == 11)
            {
                StageBase* stage = static_cast<StageBase*>(GameManager::GetStage());
                if (!stage) continue;

                Warp* warp = Instantiate<Warp>(stage);
                warp->SetPosition({ (float)x, 0.0f, (float)z });
                stage->SetWarp(warp);
            }
            
            //BossStartPos
            if (data == 12)
            {
                StageBase* stage = static_cast<StageBase*>(GameManager::GetStage());
                if (!stage) continue;

                stage->SetBossPosition({ (float)x + 0.5f, 0.0f, (float)z + 0.5f });
            }
        }
    }
    
    GameManager::GetNavigationAI()->SetMapData();
}