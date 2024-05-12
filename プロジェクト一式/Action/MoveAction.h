#pragma once
#include "BaseAction.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

//pTargetがnull出ない場合は、pTargetの方向へ移動
//pTargetがnullならtargetPosに方向に移動
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

    //他の敵をよけるようにmoveを計算
    void CalcDodge(XMVECTOR& move);

    void SetMoveSpeed(float speed) { moveSpeed_ = speed; }
    void SetMoveRange(float range) { moveRange_ = range; }
    
    //移動が終わった、Targetの位置についたか移動終わった
    bool IsInRange() { return isInRange_; }

    //Astarの場合Update呼ばないと移動しない
    virtual void SetTarget(XMFLOAT3 target) { targetPos_ = target; }
};

//Astarの移動
//TargetListの後ろのデータが目標地点
//経路の更新はUpdatePathを使い、SetTargetはIsOutTargetで使う
class AstarMoveAction : public MoveAction {
    std::vector<XMFLOAT3> targetList_;  //今の経路
    XMFLOAT3 lastTarget_;               //今のTargetCharaのポジション
    int handle_;

public:
    AstarMoveAction(Character* obj, float speed, float range);
    void Update() override;

    //今の移動目標リストを取得
    std::vector<XMFLOAT3> GetTarget() { return targetList_; }

    //移動やめさせる
    void StopMove() { targetList_.clear(); }

    //今の目標の位置が遠くなりすぎたらtrue
    bool IsOutTarget(float range);

    //新しく経路を求める
    void UpdatePath(XMFLOAT3 target);

    //デバッグ用の経路表示
    void Draw();
};

//Target地点を向き、Directionの方向に移動する
//Targetを目標地点ではなく、方向を求めるために使うため、Targetの地点は通り過ぎる
class OrientedMoveAction : public MoveAction {
    XMVECTOR targetDirection_;  //ターゲットへのベクトル
    XMVECTOR direction_;    //移動の向き
    XMVECTOR move_;         //移動量

public:
    OrientedMoveAction(Character* obj, float speed);
    ~OrientedMoveAction() override {};
    void Update() override;
    void SetDirection(XMVECTOR vec) { direction_ = vec; }
    void SetTarget(XMFLOAT3 target) override;

    //countの数移動した時、壁に接触するかどうか：壁の場合true
    bool CheckWallCollision(int count);

    //４方向確率を入れてDirectionを取得合計で0はダメ、高ければ高いほど確率上がる
    void SelectProbabilityDirection(int f, int b, int r, int l);

    //Directionを逆に
    void InverseDirection();

};