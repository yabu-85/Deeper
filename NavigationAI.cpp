#include "NavigationAI.h"
#include <stdlib.h>

void NavigationAI::Navi(XMFLOAT3 target, XMFLOAT3 pos)
{

}

void NavigationAI::Navi(XMFLOAT3& target)
{
	target = XMFLOAT3(target.x + (float)(rand() % 20 -10 ), 0.0f, target.z + (float)(rand() % 20 - 10));

}

