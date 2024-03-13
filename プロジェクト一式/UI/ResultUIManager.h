#pragma once
#include "UIManager.h"

class ResultUIManager : public UIManager
{
	int hPict_;
	Transform resultTrans_;

public:
	ResultUIManager(SceneBase* parent);
	~ResultUIManager() override;
 	void Draw() override;

};

