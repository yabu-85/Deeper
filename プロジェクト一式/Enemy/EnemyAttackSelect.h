#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

class SelectAttackInfo
{
    int priority_;                  //複数攻撃可能だった時の優先度
    std::vector<int> comboList_;    //派生可能攻撃番号
public:
    SelectAttackInfo() : priority_(0) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }
    std::vector<int>& GetComboList() { return comboList_; }
};

class SelectAttack {
    int currentAttack_;                 //現在選んでる攻撃
    std::vector<int> comboHistory_;     //今までのコンボ履歴
    void SelectNoCombo(EnemyBase* e, std::vector<int>& list);
    void SelectCombo(EnemyBase* e, std::vector<int>& list);
    
    //攻撃情報リスト
    std::vector<SelectAttackInfo*> attacks_;

public:
    SelectAttack();

    //攻撃情報の追加
    void AddSelectAttack(SelectAttackInfo* info);
    
    //選べるかを計算するだけ（取得はGetSelectAttack）
    bool Selector(EnemyBase* enemy);

    //攻撃の番号取得
    int GetSelectAttack() { return currentAttack_; }

    //コンボ情報を入れる
    void AddToComboHistory(int attackType) { comboHistory_.push_back(attackType); }
    
    //コンボ情報クリア
    void ClearComboHistory() { comboHistory_.clear(); }
};
