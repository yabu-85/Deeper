#pragma once
#include "UIManager.h"

class ExitUIManager : public UIManager
{
	int hPict_;
	Transform exitTrans_;

public:
	ExitUIManager(SceneBase* parent);
	~ExitUIManager() override;
	void Draw() override;

};

