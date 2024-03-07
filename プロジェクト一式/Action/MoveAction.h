#pragma once
#include "BaseAction.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class Player;

class MoveAction : public BaseAction
{
protected:
    bool isInRange_;    //Targetについていたらtrue
    float moveSpeed_;   //移動スピード
    float moveRange_;   //移動終了判定の範囲
    XMFLOAT3 targetPos_;//ターゲットの場所

public:
    MoveAction(Character* obj, float speed, float range);
    virtual ~MoveAction() override {};
    virtual void Update() override;
    virtual void Initialize() override {};
    virtual void Terminate() override {};

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    
    //移動が終わった、Targetの位置についたか移動終わった
    bool IsInRange() { return isInRange_; }

    //Astarの場合Update呼ばないと移動しない・IsOutEndTargetに使う
    virtual void SetTarget(XMFLOAT3 target) { targetPos_ = target; }
};

class AstarMoveAction : public MoveAction {
    std::vector<XMFLOAT3> targetList_;  //今の経路
    XMFLOAT3 lastTarget_;               //今のTargetCharaのポジション

public:
    AstarMoveAction(Character* obj, float speed, float range);
    ~AstarMoveAction() override {};

    void Update() override;

    //今の移動目標リストを取得
    std::vector<XMFLOAT3> GetTarget() { return targetList_; }

    //移動やめさせる
    void StopMove() { targetList_.clear(); }

    //今の目標の位置が遠くなりすぎたらtrue
    bool IsOutTarget(float range);

    //新しく経路を求める
    void UpdatePath(XMFLOAT3 target);

};

class OrientedMoveAction : public MoveAction {
    XMVECTOR direction_;
    XMVECTOR move_;

public:
    OrientedMoveAction(Character* obj, float speed);
    ~OrientedMoveAction() override {};
    void Update() override;
    void SetDirection(XMVECTOR vec) { direction_ = vec; }

    //countの数移動した時、壁に接触するかどうか：壁の場合true
    bool CheckWallCollision(int count);

    //エネミーの位置とターゲットの位置から方向を計算してセット
    void CalcOptimalDirection();

    //４方向確率を入れてDirectionを取得０はなし、高ければ高いほど確率上がる
    void SelectProbabilityDirection(int f, int b, int r, int l);

    //Directionを逆に
    void InverseDirection();

};