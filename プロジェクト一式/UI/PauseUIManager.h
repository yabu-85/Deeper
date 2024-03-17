#pragma once
#include "UIManager.h"

class PauseUIManager : public UIManager
{
	int hPict_[2];
	Transform pauseTrans_;

public:
	PauseUIManager(SceneBase* parent);
	~PauseUIManager() override;
 	//void Update() override;
 	void Draw() override;

};

