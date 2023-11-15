#pragma once

class BaseAction {
public:
    virtual ~BaseAction() = default;
    virtual void Update() = 0;
    virtual void Initialize() {}
    virtual void Terminate() {}
};