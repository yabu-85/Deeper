#pragma once

namespace CalcFrame
{

};

#include <vector>
using std::vector;
class EnemyBase;

class OneFrame {
protected:
    EnemyBase* pParent_;
    int time;
public:
    OneFrame(int t) : time(t), pParent_(nullptr) {};
    void SetParent(EnemyBase* enemy) { pParent_ = enemy; }
    virtual void CalcFrame(int t);
    virtual void OnFrame() {};
};

class DoubleFrame : public OneFrame {
protected:
    int endTime;
public:
    DoubleFrame(int first, int end) : OneFrame(first), endTime(end) {};
    virtual void CalcFrame(int t) override;
};

class BoundaryDoubleFrame : public DoubleFrame {
public:
    BoundaryDoubleFrame(int first, int end) : DoubleFrame(first, end) {};
    void CalcFrame(int t) override;

    virtual void OnEnter() {};
    virtual void OnExit() {};
};

class EnemyAttackController {
    std::vector<OneFrame*> frameList;
public:
    ~EnemyAttackController();
    void AddFrame(OneFrame* action) { frameList.push_back(action); }

    //登録されたアクションが判定フレームなら実行
    void Update(int currentTime);
};