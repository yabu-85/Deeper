#pragma once
#include "UIManager.h"

class ExitUIManager : public UIManager
{
	int hPict_[2];
	Transform exitTrans_;

public:
	ExitUIManager(SceneBase* parent);
	~ExitUIManager() override;
	void Draw() override;

};

