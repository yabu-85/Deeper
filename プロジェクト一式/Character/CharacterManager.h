#pragma once
#include <vector>
class Character;

//キャラクター管理クラス
namespace CharacterManager
{
	std::vector<Character*> GetCharacterList();
	void AddCharacter(Character* c);
	void RemoveCharacter(Character* c);

	//全てのキャラのUpdateを許可に
	void SetAllCharacterEnter();
	
	//全てのキャラのUpdateを拒否に
	void SetAllCharacterLeave();

};

