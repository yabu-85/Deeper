#pragma once
#include "UIManager.h"

class TitleUIManager : public UIManager
{
	int hPict_;
	Transform titleTrans_;

public:
	TitleUIManager(SceneBase* parent);
	~TitleUIManager() override;
 	void Draw() override;

};

