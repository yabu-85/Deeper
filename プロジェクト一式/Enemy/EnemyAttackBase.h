#pragma once
#include <vector>
#include <array>
using std::vector;
using std::pair;
class EnemyBase;

class EnemyFrame {
	pair<int, int> frame;
	bool inFrame_;
protected:
	EnemyBase* pParent_;

public:
	EnemyFrame(int start, int end);
	void SetParent(EnemyBase* enemy) { pParent_ = enemy; }
	bool IsTargetFrame(int time) { return frame.first >= time && frame.second <= time; }
	bool IsEndFrame(int time) { return frame.second == time; }
	bool GetInFrame() { return inFrame_; }
	void SetInFrame(bool b) { inFrame_ = b; }

	virtual void Update() {};
	virtual void OnEnter() {};
	virtual void OnExit() {};
};

class EnemyAttackController {
	std::vector<EnemyFrame*> actions;

public:
	~EnemyAttackController();
	void AddAction(EnemyFrame* action) { actions.push_back(action); }

	//登録されたアクションが判定フレームだったらAction呼ぶ
	void Update(int currentTime);
};