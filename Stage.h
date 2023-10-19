#pragma once
#include "Engine/GameObject.h"
#include <vector>

enum StageNum {
    T1 = 0,
    T2,
    MAX,
};

enum RayStageNum {
    RT1 = 0,
    RT2,
    RMAX,
};

struct IntersectData {
    int hModelNum;
    XMFLOAT3 position;
};

class Stage :
    public GameObject
{
    int hModel_[MAX + RMAX];
    std::vector<IntersectData> intersectDatas_; //ローポリのデータ集

    enum MAP {
        NONE = 0,
        FLOAR,
        MAX,
    };

    int mapSizeX_;
    int mapSizeZ_;
    XMFLOAT3 startPos_;
    std::vector<std::vector<int>> mapData_;
    void CreatSquare(XMFLOAT3 start, int x, int z);

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void ResetStage();
    void CreatStage();
    XMFLOAT3 GetPlayerStartPos();

   std::vector<IntersectData> GetIntersectDatas() { return intersectDatas_; }; 

};