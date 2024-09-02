#pragma once
#include "../Engine/GameObject.h"
#include <vector>
class Character;

//ダメージの情報
struct DamageInfo {
    Character* owner;   //ダメージを与えるキャラ
    std::string name;   //攻撃の名前
    int damage;         //ダメージ

    DamageInfo() : owner(nullptr), name(""), damage(0) {};
    DamageInfo(int _damage) : owner(nullptr), name(""), damage(_damage) {};
    DamageInfo(Character* _owner, std::string _name, int _damage ) : owner(_owner), name(_name), damage(_damage) {};
};

//ノックバックの種類
enum class KNOCK_TYPE {
    SMALL = 0,
    MEDIUM,
    LARGE,
    MAX,
};

//ノックバックの情報
struct KnockBackInfo {
    KNOCK_TYPE type;    //ノックバックの種類
    int time;           //時間
    float power;        //パワー
    XMFLOAT3 pos;       //ノックバック発生座標

    KnockBackInfo() : type(KNOCK_TYPE::MEDIUM), time(0), power(0.0f), pos({ 0.f, 0.f, 0.f }) {};
    KnockBackInfo(KNOCK_TYPE _type, int _time, float _power, XMFLOAT3 _pos) : type(_type), time(_time), power(_power), pos(_pos) {};
};

/// <summary>
/// Health・ダメージを与えられる・キャラ同士の反発・ノックバックの機能をまとめたクラス
/// 継承して使う
/// </summary>
class Character : public GameObject
{
    int hp_;            //HP
    int maxHp_;         //最大HP
    float bodyWeight_;  //めり込み時に使う重さ
    float bodyRange_;   //めり込み時に使う範囲
    XMFLOAT3 movement_; //移動量を保存するためのやつ

    int knockBackTime_;             //ノックバックの時間
    int knockBackTimeMax_;          //ノックバックの時間保存用
    XMFLOAT3 knockBackDirection_;   //ノックバックさせる方向

    virtual void SmallKnockBack() {};   //ノックバックされたときSmalの場合呼ばれる関数
    virtual void MediumKnockBack() {};  //ノックバックされたときMediumの場合呼ばれる関数
    virtual void LargeKnockBack() {};   //ノックバックされたときLargeの場合呼ばれる関数
    void KnockBack(float speed);        //実際にノックバックさせる関数

    std::vector<DamageInfo> damageInfoList_;    //ダメージを与えられた情報のリスト
    virtual void Damage() {};                   //ダメージを与えられたときに呼ばれる
    virtual void Dead() {};                     //死亡した場合に呼ばれる

public:
    Character(GameObject* parent, std::string name);
    virtual ~Character();
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0;
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    //キャラクター同士の衝突の際めり込みを避ける
    void ReflectCharacter();
    
    //----------------------------------------------------------------

    //DamageInfoを考慮しないApplyDamage関数
    void ApplyDamageDirectly(const DamageInfo& daamgeinfo);
    void ApplyDamageDirectly(const DamageInfo& daamgeinfo, const KnockBackInfo& knockinfo);

    /// <summary>
    /// DamageInfoListを考慮するApplyDamage関数
    /// 戻り値：Listになく、攻撃が成功したならtrueが返ってくる
    /// </summary>
    bool ApplyDamageWithList(const DamageInfo& daamgeinfo);
    bool ApplyDamageWithList(const DamageInfo& daamgeinfo, const KnockBackInfo& knockinfo);

    //ダメージ情報
    void AddDamageInfo(const DamageInfo& damageInfo);
    void RemoveDamageInfo(const DamageInfo& damageInfo);

    //----------------------------------------------------------------
    
    void SetHP(int i) { hp_ = i; }
    void SetMaxHP(int i) { maxHp_ = i; }
    int GetHP() { return hp_; }
    int GetMaxHP() { return maxHp_; }

    float GetBodyRange() { return bodyRange_; }
    float GetBodyWeight() { return bodyWeight_; }
    void SetBodyWeight(float f) { bodyWeight_ = f; }
    void SetBodyRange(float f) { bodyRange_ = f; }
    
    int GetKnockBackTime() { return knockBackTime_; }
    int GetKnockBackTimeMax() { return knockBackTimeMax_; }

    //----------------------------------------------------------------

    void SetMovement(XMFLOAT3 move) { movement_ = move; }                   //プレイヤーだけ追加処理ある
    void SetMovement(XMVECTOR move) { XMStoreFloat3(&movement_, move); }    //・・
    XMFLOAT3 GetMovement() { return movement_; }
    XMVECTOR GetMovementVector() { return XMLoadFloat3(&movement_); }
    virtual void ResetMovement() { movement_ = { 0.0f, 0.0f, 0.0f }; };
};

/// <summary>
/// 複数回攻撃が当たらないようにするクラス、攻撃をするCharacterに持たせて使う
/// attackListに乗っているCharacterには攻撃が当たらない
/// </summary>
class DamageController {
    DamageInfo currentDamage;           //今設定されているダメージの情報
    KnockBackInfo currentKnockBack;     //今設定されているノックバックの情報
    std::vector<Character*> attackList; //攻撃を与えたCharacterのリスト
    
public:
    void AddAttackList(Character* chara);       //キャラをリストに登録
    void ResetAttackList();                     //リストリセット

    //ダメージ・ノックバックアクセサ
    void SetCurrentDamage(const DamageInfo& info) { currentDamage = info; }
    DamageInfo& GetCurrentDamage() { return currentDamage; }
    void SetCurrentKnockBackInfo(const KnockBackInfo& info) { currentKnockBack = info; }
    KnockBackInfo& GetCurrentKnockBackInfo() { return currentKnockBack; }
};
