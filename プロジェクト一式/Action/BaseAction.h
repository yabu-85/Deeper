#pragma once

class Character;

/// <summary>
/// Moveやらのアクションの基底クラス
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