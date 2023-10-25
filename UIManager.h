#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>
#include<functional>
using namespace DirectX;

class UIBase;

class UIManager
{

protected:
	std::vector<UIBase*> uiList_;

public:
	UIManager();
	~UIManager();

	void Update();
	void Draw();
	void AddUi(std::string name, XMFLOAT2 pos, std::function<void()> onClick);
	void EndDraw();

};

