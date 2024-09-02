#pragma once
#include "../Engine/GameObject.h"
#include <vector>
class Character;

//�_���[�W�̏��
struct DamageInfo {
    Character* owner;   //�_���[�W��^����L����
    std::string name;   //�U���̖��O
    int damage;         //�_���[�W

    DamageInfo() : owner(nullptr), name(""), damage(0) {};
    DamageInfo(int _damage) : owner(nullptr), name(""), damage(_damage) {};
    DamageInfo(Character* _owner, std::string _name, int _damage ) : owner(_owner), name(_name), damage(_damage) {};
};

//�m�b�N�o�b�N�̎��
enum class KNOCK_TYPE {
    SMALL = 0,
    MEDIUM,
    LARGE,
    MAX,
};

//�m�b�N�o�b�N�̏��
struct KnockBackInfo {
    KNOCK_TYPE type;    //�m�b�N�o�b�N�̎��
    int time;           //����
    float power;        //�p���[
    XMFLOAT3 pos;       //�m�b�N�o�b�N�������W

    KnockBackInfo() : type(KNOCK_TYPE::MEDIUM), time(0), power(0.0f), pos({ 0.f, 0.f, 0.f }) {};
    KnockBackInfo(KNOCK_TYPE _type, int _time, float _power, XMFLOAT3 _pos) : type(_type), time(_time), power(_power), pos(_pos) {};
};

/// <summary>
/// Health�E�_���[�W��^������E�L�������m�̔����E�m�b�N�o�b�N�̋@�\���܂Ƃ߂��N���X
/// �p�����Ďg��
/// </summary>
class Character : public GameObject
{
    int hp_;            //HP
    int maxHp_;         //�ő�HP
    float bodyWeight_;  //�߂荞�ݎ��Ɏg���d��
    float bodyRange_;   //�߂荞�ݎ��Ɏg���͈�
    XMFLOAT3 movement_; //�ړ��ʂ�ۑ����邽�߂̂��

    int knockBackTime_;             //�m�b�N�o�b�N�̎���
    int knockBackTimeMax_;          //�m�b�N�o�b�N�̎��ԕۑ��p
    XMFLOAT3 knockBackDirection_;   //�m�b�N�o�b�N���������

    virtual void SmallKnockBack() {};   //�m�b�N�o�b�N���ꂽ�Ƃ�Smal�̏ꍇ�Ă΂��֐�
    virtual void MediumKnockBack() {};  //�m�b�N�o�b�N���ꂽ�Ƃ�Medium�̏ꍇ�Ă΂��֐�
    virtual void LargeKnockBack() {};   //�m�b�N�o�b�N���ꂽ�Ƃ�Large�̏ꍇ�Ă΂��֐�
    void KnockBack(float speed);        //���ۂɃm�b�N�o�b�N������֐�

    std::vector<DamageInfo> damageInfoList_;    //�_���[�W��^����ꂽ���̃��X�g
    virtual void Damage() {};                   //�_���[�W��^����ꂽ�Ƃ��ɌĂ΂��
    virtual void Dead() {};                     //���S�����ꍇ�ɌĂ΂��

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

/// <summary>
/// ������U����������Ȃ��悤�ɂ���N���X�A�U��������Character�Ɏ������Ďg��
/// attackList�ɏ���Ă���Character�ɂ͍U����������Ȃ�
/// </summary>
class DamageController {
    DamageInfo currentDamage;           //���ݒ肳��Ă���_���[�W�̏��
    KnockBackInfo currentKnockBack;     //���ݒ肳��Ă���m�b�N�o�b�N�̏��
    std::vector<Character*> attackList; //�U����^����Character�̃��X�g
    
public:
    void AddAttackList(Character* chara);       //�L���������X�g�ɓo�^
    void ResetAttackList();                     //���X�g���Z�b�g

    //�_���[�W�E�m�b�N�o�b�N�A�N�Z�T
    void SetCurrentDamage(const DamageInfo& info) { currentDamage = info; }
    DamageInfo& GetCurrentDamage() { return currentDamage; }
    void SetCurrentKnockBackInfo(const KnockBackInfo& info) { currentKnockBack = info; }
    KnockBackInfo& GetCurrentKnockBackInfo() { return currentKnockBack; }
};
