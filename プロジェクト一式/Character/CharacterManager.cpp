#include "CharacterManager.h"
#include "Character.h"

namespace CharacterManager {
	std::vector<Character*> characterList_;

}

std::vector<Character*> CharacterManager::GetCharacterList() {
	return characterList_;
}

void CharacterManager::AddCharacter(Character* c) {
	characterList_.push_back(static_cast<Character*>(c));
}

void CharacterManager::RemoveCharacter(Character* c) {
	for (auto it = characterList_.begin(); it != characterList_.end();) {
		if (*it == c) {
			it = characterList_.erase(it);
			break;
		}
		else {
			++it;
		}
	}
}

void CharacterManager::SetAllCharacterEnter()
{
	for (auto e : characterList_) {
		e->Enter();
	}
}

void CharacterManager::SetAllCharacterLeave()
{
	for (auto e : characterList_) {
		e->Leave();
	}
}

