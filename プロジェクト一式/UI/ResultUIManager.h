#pragma once
#include "UIManager.h"

class ResultUIManager : public UIManager
{
	int hPict_[2];

public:
	ResultUIManager(SceneBase* parent);
	~ResultUIManager() override;
 	void Draw() override;

};

