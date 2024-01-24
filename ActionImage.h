#pragma once
#include "Engine/GameObject.h"

enum ACTION_UI_TABLE {
    WAIT = 0,
    WALK,
    WEAPON_CHANGE,
    AVO,
    ATK,
    ATK_SUB,
    MAX,
};

class ActionImage : public GameObject
{
    bool isDraw_;       //�`�悷�邩�ǂ���
    bool isDrawAction_; //�A�N�V�����{�^���̕\�������邩�ǂ���
    ACTION_UI_TABLE type_;

public:
    ActionImage(GameObject* parent);
    ~ActionImage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //���O�̏�����
    void SetName();
    
    //�ǂ̃e�[�u����\�����邩�ݒ肷��
    void SetActionTable(ACTION_UI_TABLE type) { type_ = type; }
    
    //�\�����邩�ǂ���
    void SetActionIsDraw(bool b) { isDraw_ = b; }
    
    //ActionButton�̕`�悷�邩�ǂ���
    void SetActionUI(bool b) { isDrawAction_ = b; }

};

