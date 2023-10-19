#pragma once
#include "Engine/GameObject.h"
#include <vector>

enum StageNum {
    T1 = 0,
    T1R,
    T2,
    MAX,
};

enum RayStageNum {
    RT1 = 0,
    RT1R,
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
    std::vector<IntersectData> intersectDatas_; //���[�|���̃f�[�^�W

    enum MAP {
        NONE = 0,
        FLOAR,
    };

    int mapSizeX_;
    int mapSizeZ_;
    XMFLOAT3 startPos_;
    std::vector<std::vector<int>> mapData_;

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    std::vector<IntersectData> GetIntersectDatas() { return intersectDatas_; };
    XMFLOAT3 GetPlayerStartPos();

    void ResetStage();
    void CreatStage();


};