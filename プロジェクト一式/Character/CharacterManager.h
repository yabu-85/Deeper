#pragma once
#include <vector>
class Character;

//�L�����N�^�[�Ǘ��N���X
namespace CharacterManager
{
	std::vector<Character*> GetCharacterList();
	void AddCharacter(Character* c);
	void RemoveCharacter(Character* c);

	//�S�ẴL������Update������
	void SetAllCharacterEnter();
	
	//�S�ẴL������Update�����ۂ�
	void SetAllCharacterLeave();

};

