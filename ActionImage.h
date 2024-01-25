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
    bool isDraw_;       //描画するかどうか
    bool isDrawAction_; //アクションボタンの表示をするかどうか
    ACTION_UI_TABLE type_;

    //修正箇所
    int hPict_[10];

public:
    ActionImage(GameObject* parent);
    ~ActionImage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //名前の初期化
    void SetName();
    
    //どのテーブルを表示するか設定する
    void SetActionTable(ACTION_UI_TABLE type) { type_ = type; }
    
    //表示するかどうか
    void SetActionIsDraw(bool b) { isDraw_ = b; }
    
    //ActionButtonの描画するかどうか
    void SetActionUI(bool b) { isDrawAction_ = b; }

};

