#pragma once
#include <array>
#include <vector>
#include <utility> //std::pair

class PlayerCommand
{
public:
	enum COMMAND {
		ATK = 0,
		SUB_ATK,
		CENTER,
		CENTER_UP,
		CENTER_DOWN,
		AVO,
		TARGET,

		LEFT,
		RIGHT,
		UP,
		DOWN,
		
		CMD_MAX
	};

private:
	std::vector<std::pair<int, COMMAND>> pushKeyCommand_;
	std::vector<std::pair<int, COMMAND>> downKeyCommand_;
	std::vector<std::pair<int, COMMAND>> pushMouseCommand_;
	std::vector<std::pair<int, COMMAND>> downMouseCommand_;
	std::vector<std::pair<int, COMMAND>> scrollCommand_;

	std::array<bool, CMD_MAX> commandFlags = { false };

	bool walk_;

public:
	PlayerCommand();
	void Update();

	//コマンドの変更
	//Cmdの番号（DIK_A)：0 = key / 1 = mouse：Cmdの名前
	void ChangeCmd(COMMAND number, int type);

	bool CmdAtk() { return commandFlags[ATK]; }
	bool CmdSubAtk() { return commandFlags[SUB_ATK]; }
	bool CmdAvo() { return commandFlags[AVO]; }
	bool CmdTarget() { return commandFlags[TARGET]; }

	bool CmdCenter() { return commandFlags[CENTER]; }
	bool CmdCenterUp() { return commandFlags[CENTER_UP]; }
	bool CmdCenterDown() { return commandFlags[CENTER_DOWN]; }

	bool CmdLeft() { return commandFlags[LEFT]; }
	bool CmdRight() { return commandFlags[RIGHT]; }
	bool CmdUp() { return commandFlags[UP]; }
	bool CmdDown() { return commandFlags[DOWN]; }
	bool CmdWalk() { return walk_; }

};

