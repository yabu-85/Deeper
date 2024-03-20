#pragma once
#include "../Engine/GameObject.h"
#include <vector>

enum class KNOCK_TYPE {
    SMALL = 0,
    MEDIUM,
    LARGE,
    MAX,
};

class Character;
struct DamageInfo {
    Character* owner;
    std::string name;
    int damage;

    DamageInfo() : owner(nullptr), name(""), damage(0) {};
    DamageInfo(int _damage) : owner(nullptr), name(""), damage(_damage) {};
    DamageInfo(Character* _owner, std::string _name, int _damage ) : owner(_owner), name(_name), damage(_damage) {};
};

struct KnockBackInfo {
    KNOCK_TYPE type;
    int time;
    float power;
    XMFLOAT3 pos;

    KnockBackInfo() : type(KNOCK_TYPE::MEDIUM), time(0), power(0.0f), pos({ 0.f, 0.f, 0.f }) {};
    KnockBackInfo(KNOCK_TYPE _type, int _time, float _power, XMFLOAT3 _pos) : type(_type), time(_time), power(_power), pos(_pos) {};
};

class Character : public GameObject
{
    int hp_;
    int maxHp_;
    float bodyWeight_;  //�߂荞�ݎ��Ɏg���d��
    float bodyRange_;   //�߂荞�ݎ��Ɏg���͈�
    XMFLOAT3 movement_; //�ړ��ʂ�ۑ����邽�߂̂��

    int knockBackTime_;             //�m�b�N�o�b�N�̎���
    int knockBackTimeMax_;          //�m�b�N�o�b�N�̎��ԕۑ��p
    XMFLOAT3 knockBackDirection_;   //�m�b�N�o�b�N���������

    virtual void SmallKnockBack() {};
    virtual void MediumKnockBack() {};
    virtual void LargetKnockBack() {};

    std::vector<DamageInfo> damageInfoList_;
    virtual void Damage() {};
    virtual void Dead() {};
    void KnockBack(float speed);

public:
    Character(GameObject* parent, std::string name);
    virtual ~Character();
    virtual void Initialize() override = 0 {};
    virtual void Update() override = 0;
    virtual void Draw() override = 0 {};
    virtual void Release() override = 0 {};

    //�L�����N�^�[���m�̏Փ˂̍ۂ߂荞�݂������
    void ReflectCharacter();
    
    //----------------------------------------------------------------

    //DamageInfo���l�����Ȃ�ApplyDamage�֐�
    void ApplyDamageDirectly(const DamageInfo& daamgeinfo);
    void ApplyDamageDirectly(const DamageInfo& daamgeinfo, const KnockBackInfo& knockinfo);

    /// <summary>
    /// DamageInfoList���l������ApplyDamage�֐�
    /// �߂�l�FList�ɂȂ��A�U�������������Ȃ�true���Ԃ��Ă���
    /// </summary>
    bool ApplyDamageWithList(const DamageInfo& daamgeinfo);
    bool ApplyDamageWithList(const DamageInfo& daamgeinfo, const KnockBackInfo& knockinfo);

    //�_���[�W���
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

    void SetMovement(XMFLOAT3 move) { movement_ = move; }                   //�v���C���[�����ǉ���������
    void SetMovement(XMVECTOR move) { XMStoreFloat3(&movement_, move); }    //�E�E
    XMFLOAT3 GetMovement() { return movement_; }
    XMVECTOR GetMovementVector() { return XMLoadFloat3(&movement_); }
    virtual void ResetMovement() { movement_ = { 0.0f, 0.0f, 0.0f }; };
};

class DamageController {
    DamageInfo currentDamage;
    KnockBackInfo currentKnockBack;
    std::vector<Character*> attackList;
public:
    void SetCurrentDamage(const DamageInfo& info) { currentDamage = info; }
    DamageInfo& GetCurrentDamage() { return currentDamage; }
    void SetCurrentKnockBackInfo(const KnockBackInfo& info) { currentKnockBack = info; }
    KnockBackInfo& GetCurrentKnockBackInfo() { return currentKnockBack; }

    void AddAttackList(Character* chara);
    void ResetAttackList();
};
