#include "InputManager.h"
#include "../Engine/Input.h"
#include <array>

namespace InputManager {
	std::array<std::pair<TYPE, int>, CMD_MAX> commandList;

	//enum‚Ì‡”Ô‚Å
	const std::array<std::pair<TYPE, int>, CMD_MAX> DEFAULT_COMMAND {
		std::make_pair(KEY, DIK_A),
		std::make_pair(KEY, DIK_D),
		std::make_pair(KEY, DIK_W),
		std::make_pair(KEY, DIK_S),

		std::make_pair(MOUSE, 0),
		std::make_pair(MOUSE, 1),
		std::make_pair(SCROLL_UP, 0),
		std::make_pair(SCROLL_DOWN, 0),

		std::make_pair(KEY, DIK_SPACE),
		std::make_pair(MOUSE, 2),
		std::make_pair(KEY, DIK_E),
	};
	
}

void InputManager::SetDefaultKeyConfig()
{
	//DEFAULT_COMMAND‚Éİ’è
	for (int i = 0; i < CMD_MAX; i++) {
		commandList[i].first = DEFAULT_COMMAND[i].first;
		commandList[i].second = DEFAULT_COMMAND[i].second;
	}

}

void InputManager::ChangeCmd(COMMAND cmd, TYPE type, int num)
{
	//‚·‚Å‚É‚ ‚éKey‚ª‚ ‚éê‡‚Í‘ÎÛ‚ğ‰Šú‰»
	for (int i = 0; i < CMD_MAX; i++) {
		if (commandList[i].first == type && commandList[i].second == num) {
			commandList[i].first = KEY;
			commandList[i].second = 0;
			break;
		}
	}

	commandList[cmd].first = type;
	commandList[cmd].second = num;

}

bool InputManager::IsCmd(COMMAND cmd)
{
	if (commandList[cmd].first == KEY) return Input::IsKey(commandList[cmd].second);
	else if (commandList[cmd].first == MOUSE) return Input::IsMouseButton(commandList[cmd].second);
	else if (commandList[cmd].first == SCROLL_UP) return Input::IsUpScroll();
	else if (commandList[cmd].first == SCROLL_DOWN) return Input::IsDownScroll();
	return false;
}

bool InputManager::IsCmdUp(COMMAND cmd)
{
	if (commandList[cmd].first == KEY) return Input::IsKeyUp(commandList[cmd].second);
	else if (commandList[cmd].first == MOUSE) return Input::IsMouseButtonUp(commandList[cmd].second);
	else if (commandList[cmd].first == SCROLL_UP) return Input::IsUpScroll();
	else if (commandList[cmd].first == SCROLL_DOWN) return Input::IsDownScroll();
	return false;
}

bool InputManager::IsCmdDown(COMMAND cmd)
{
	if (commandList[cmd].first == KEY) return Input::IsKeyDown(commandList[cmd].second);
	else if (commandList[cmd].first == MOUSE) return Input::IsMouseButtonDown(commandList[cmd].second);
	else if (commandList[cmd].first == SCROLL_UP) return Input::IsUpScroll();
	else if (commandList[cmd].first == SCROLL_DOWN) return Input::IsDownScroll();
	return false;
}

bool InputManager::CmdWalk() {
	bool up = IsCmd(MOVE_UP);
	bool down = IsCmd(MOVE_DOWN);
	bool right = IsCmd(MOVE_RIGHT);
	bool left = IsCmd(MOVE_LEFT);

	bool walk = up || down || right || left;
	if (!walk) return false;

	//•¡”‰Ÿ‚µ‚Ä‚¢‚éê‡‚Ìˆ—
	if (up && down && !left && !right) return false;
	else if (left && right && !up && !down) return false;
	else if (up && down && left && right) return false;
	
	return true;
}