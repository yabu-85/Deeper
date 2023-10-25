#pragma once
#include "UIManager.h"

class ExitUIManager : public UIManager
{
	int hPict_;

public:
	ExitUIManager();
	~ExitUIManager() override;
	void Draw() override;

};

