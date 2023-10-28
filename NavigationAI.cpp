#include "NavigationAI.h"
#include "Stage.h"

NavigationAI::NavigationAI(Stage* s)
{
	pStage_ = s;
	mapData_ = pStage_->GetMapData();

}

void NavigationAI::Navi(XMFLOAT3 target, XMFLOAT3 pos)
{

}

void NavigationAI::Navi(XMFLOAT3& target)
{
	target = XMFLOAT3(target.x + (float)(rand() % 20 -10 ), 0.0f, target.z + (float)(rand() % 20 - 10));

}

