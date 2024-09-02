#pragma once

class Character;

/// <summary>
/// Move���̃A�N�V�����̊��N���X
/// </summary>
class BaseAction {
protected:
    Character* pCharacter_;

public:
    BaseAction(Character* obj);
    virtual ~BaseAction() {};
    virtual void Update() = 0;
    virtual void Initialize() {}
    virtual void Terminate() {}
};