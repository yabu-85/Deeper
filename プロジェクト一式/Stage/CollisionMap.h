#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/Fbx.h"
#include <vector>

class Triangle;
class Cell;

/// <summary>
/// Cellをまとめたクラスで
/// 当たり判定やRayCastなどをする
/// </summary>
class CollisionMap : public GameObject
{
    //ヒープポインタのエラー出るから仕方なくここにも
    std::vector<Triangle*> triangles_;
   
    Cell*** cells_ = 0;

    //指定したCellのポインタを取得 / 範囲外ならnullptr
    Cell* GetCell(XMFLOAT3 pos);

public:
    CollisionMap(GameObject* parent);
    ~CollisionMap();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void CreatIntersectDataTriangle();
    void ResetCellTriangle();

    bool GetRayCastMinDist(XMFLOAT3 camPos, XMFLOAT3 plaPos, RayCastData* _data, float& minDist);

    //線分上のCellのTriangleとRayの判定当たったら即終了
    void RaySelectCellVsSegment(RayCastData& _data, XMFLOAT3 target);

    void CalcMapWall(XMFLOAT3& _pos, float speed, float radius);

    //CollisionMapとの判定時間内からのやつ
    bool IsWall(int x, int z);

};

