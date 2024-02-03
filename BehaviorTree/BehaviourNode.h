#pragma once
#include <vector>

//--------------------------Base--------------------------

class TreeNode
{
public:
    enum class NodeType {
        ACTION_NODE,
        CONDITION_NODE,
        CONTROL_NODE
    };

    enum class Status {
        INVALID,
        SUCCESS,
        FAILURE,
        RUNNING,
    };

protected:
    Status status_;
    NodeType type_;

public:
    TreeNode();
    virtual ~TreeNode();
    Status Tick();

    virtual Status Update() = 0;    //�{�̏���
    virtual void Initialize() {}    //Invalid�̎��������s
    virtual void Terminate() {};    //Running�ȊO�̎����s

    virtual void ResetState() { status_ = Status::INVALID; }
    void SetStatus(Status newState) { status_ = newState; }
    Status GetStatus() { return status_; }
    NodeType GetType() { return type_; }
};

//--------------------------Composite--------------------------

//Sequence��Selector���������邽�߂̒��ۃN���X
//TreeNode���p�������^�C�v�������Ƃ��ł���
class CompositeNode : public TreeNode
{
public:
    CompositeNode();
    virtual ~CompositeNode() override;
    virtual Status Update() override = 0;
    virtual void ResetState() override;

    void AddChildren(TreeNode* child) { childlenNodes_.push_back(child); }
    unsigned GetChildrenNumber() { return static_cast<unsigned>(childlenNodes_.size()); }
    std::vector<TreeNode*> GetChildren() { return childlenNodes_; }
    TreeNode* GetChild(int index) { return childlenNodes_.at(index); }

protected:
    std::vector<TreeNode*> childlenNodes_;
    unsigned currentIndex_;
};

//--------------------------Sequence--------------------------

class Sequence : public CompositeNode
{
public:
    Sequence();
    virtual ~Sequence() override;
    void Initialize() override;
    Status Update() override;
};

//--------------------------Selector--------------------------

class Selector : public CompositeNode
{
public:
    Selector();
    virtual ~Selector() override;
    void Initialize() override;
    Status Update() override;
};

//--------------------------Condition--------------------------

//Decoletar�̖���
class Condition : public TreeNode
{
protected:
    TreeNode* child_;
public:
    Condition(TreeNode* child);
    virtual ~Condition() override;
    virtual Status Update() override = 0;
};

//�K��SUCCESS��Ԃ�
class Succeeder : public Condition
{
public:
    Succeeder(TreeNode* child);
    Status Update() override;
};

//�K��FAILURE��Ԃ�
class Failer : public Condition
{
public:
    Failer(TreeNode* child);
    Status Update() override;
};

//�t�̌��ʂ�Ԃ��iSucces��������Failure�j
class Inverter : public Condition
{
public:
    Inverter(TreeNode* child);
    Status Update() override;
};

//--------------------------Root--------------------------

class Root : public TreeNode
{
    TreeNode* root_;
public:
    Root();
    ~Root() override;
    Status Update() override;
    void SetRootNode(TreeNode* root) { root_ = root; }
};

//--------------------------Action--------------------------

class Action : public TreeNode
{
public:
    Action();
    virtual ~Action();
    virtual Status Update() override = 0;
};