#include "CreateStage.h"
#include "CollisionMap.h"
#include "NavigationAI.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/CsvReader.h"
#include "../Engine/Global.h"
#include "../GameManager/GameManager.h"
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
    std::string fileName[MAX] = { "StageT1", "StageT2", "RayStageT1", "RayStageT2"};
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

XMFLOAT3 CreateStage::GetRandomFloarPosition()
{
    std::vector<Cell> posList;
    for (int x = 0; x < mapSizeX_; x++) {
        for (int z = 0; z < mapSizeZ_; z++) {
            if (mapData_[z][x] == CreateStage::MAP::M_FLOAR) {
                Cell cell;
                cell.x = x;
                cell.z = z;
                posList.push_back(cell);
            }
        }
    }

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

        //sizeの範囲で床があればリストに追加
        for (int i = pos[0] - size; i <= pos[0] + size; ++i) {
            for (int j = pos[1] - size; j <= pos[1] + size; ++j) {
                if (i < 0 || i >= (int)mapData_[0].size() || j < 0 || j >= (int)mapData_.size())
                    continue;

                if (mapData_[j][i] == CreateStage::MAP::M_FLOAR) {
                    Cell c(i, j);
                    cellList.push_back(c);
                }
            }
        }

        //リストからじゃなければその中からランダムで選び返す
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

    //CSVファイル読み込み
    CsvReader csv;
    csv.Load(name);

    //ステージの幅と高さを設定
    mapSizeX_ = (int)csv.GetWidth();
    mapSizeZ_ = (int)csv.GetHeight();
    intersectDatas_.clear();

    //サイズをマップの大きさにする
    mapData_.resize(mapSizeZ_);
    for (int i = 0; i < mapSizeZ_; i++) mapData_[i].resize(mapSizeX_, FLOAR);

    //床のサイズをマップサイズに合わせて生成
    intersectDatas_.push_back({ hModel_[FLOAR], hModel_[R_FLOAR], XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3((float)mapSizeX_, 1.0f, (float)mapSizeZ_) });

    //CSVデータをテーブルに格納
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
                warp->SetPosition(XMFLOAT3((float)x, 0.0f, (float)z));
                stage->AddWarpList(warp);
            }
        }
    }
    
    GameManager::GetNavigationAI()->SetMapData();
}
