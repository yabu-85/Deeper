#pragma once
#include <vector>
#include "../Engine/Singleton.h"
class EnemyBase;

/// <summary>
/// 攻撃の情報をまとめるクラス
/// 優先度は数が低いほうが優先度高い
/// 登録されたComboリストから次の攻撃に移ることが出来る
/// </summary>
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
    void AddComboList(int i) { comboList_.push_back(i); }
    void SetComboList(std::vector<int> list) { comboList_ = list; }
};

/// <summary>
/// AddSelectAttackで攻撃情報を初期登録
/// Selectorで登録されたリストから選択
/// Selectorで攻撃がかぶることはない
/// コンボ履歴消すのにClearComboHistoryを使う
/// </summary>
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
    
    //選べるかを計算してCurrentAttackに番号保存（取得はGetSelectAttack）
    bool Selector(EnemyBase* enemy);

    //攻撃の番号取得
    int GetSelectAttack() { return currentAttack_; }

    //コンボ中かどうか
    bool IsCombo() { return !comboHistory_.empty(); }

    //コンボ情報を入れる
    void AddComboHistory(int attackType) { comboHistory_.push_back(attackType); }
    
    //コンボ情報クリア
    void ClearComboHistory() { comboHistory_.clear(); }
};
