#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

class SelectAttackInfo
{
    int priority_;   //複数攻撃可能だった時の優先度
public:
    SelectAttackInfo() : priority_(0) {}
    virtual bool CanUseAttack(EnemyBase* enemy) { return false; }
    int GetPriority() { return priority_; }
    void SetPriority(int p) { priority_ = p; }
};

#include <map>

class SelectAttack {
    int selectAttack_;
    std::vector<int> comboHistory_;
    std::vector<SelectAttackInfo*> attacks_;

    void SelectNoCombo(EnemyBase* e, std::vector<int>& list);
    void SelectCombo(EnemyBase* e, std::vector<int>& list);


    int currentAttackType_; // 現在の攻撃タイプ
    std::map<int, std::vector<SelectAttackInfo*>> attackMap_; // 攻撃タイプに対する派生可能な攻撃のマップ
    void Test() {
        // 現在の攻撃タイプに対する派生可能な攻撃を取得
        const auto& availableAttacks = attackMap_[currentAttackType_];


    }
    void AddSelectAttack(int attackType, SelectAttackInfo* info) {
        attackMap_[attackType].push_back(info);
    }


public:
    SelectAttack();

    //攻撃情報の追加
    void AddSelectAttack(SelectAttackInfo* info);
    
    //選べるかを計算するだけ（取得はGetSelectAttack）
    bool Selector(EnemyBase* enemy);

    //攻撃の番号取得
    int GetSelectAttack() { return selectAttack_; }

    //コンボ情報を入れる
    void AddToComboHistory(int attackType) { comboHistory_.push_back(attackType); }
    
    //コンボ情報クリア
    void ClearComboHistory() { comboHistory_.clear(); }
};
