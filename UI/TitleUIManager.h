#pragma once
#include "UIManager.h"

class TitleUIManager : public UIManager
{
	int hPict_[2];

public:
	TitleUIManager();
	~TitleUIManager() override;
 	void Draw() override;

};

