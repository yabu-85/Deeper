#pragma once
#include "UIManager.h"

class PlayUIManager : public UIManager
{
public:
	PlayUIManager(SceneBase* parent);
	~PlayUIManager() override;
 	void Update() override;
 	void Draw() override;

};

