#include "InteractionUI.h"
#include "Interaction.h"
#include "../Engine/GameObject.h"

InteractionUI::InteractionUI(GameObject* parent)
	: isValid_(true), offset_{ 0.0f, 1.0f, 0.0f }, pParent_(parent)
{
}

InteractionUI::~InteractionUI()
{
}

void InteractionUI::Initialize()
{
	Interaction::AddInteractData(this);

}

void InteractionUI::Release()
{
	Interaction::RemoveInteractData(this);

}
