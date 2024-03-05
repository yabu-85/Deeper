#include "CharacterManager.h"
#include "Character.h"

namespace CharacterManager {
	std::vector<Character*> characterList_;

	std::vector<Character*> GetCharacterList() { 
		return characterList_; 
	}

	void AddCharacter(Character* c) { 
		characterList_.push_back(static_cast<Character*>(c)); 
	}

	void RemoveCharacter(Character* c) {
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

}

