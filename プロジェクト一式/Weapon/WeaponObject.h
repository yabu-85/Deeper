#pragma once
#include "../Engine/GameObject.h"

class InteractionUI;

class WeaponObject : public GameObject
{
    int hModel_;
    int type_;      //weapon‚Ìtype
    InteractionUI* pInteractionUI_;

public:
    WeaponObject(GameObject* parent);
    ~WeaponObject() override;
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void LoadModel(std::string _name);
    void SetType(int t) { type_ = t; }
    int GetType() { return type_; }
    InteractionUI* GetInteractionUI() { return pInteractionUI_; }

};

