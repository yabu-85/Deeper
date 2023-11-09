#pragma once
#include <vector>
#include <string>

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
        INVALID,    //無効
        SUCCESS,    //成功
        FAILURE,    //失敗
        RUNNING,    //動作中
        //IDLE,       //アイドル
        //TERMINATE     //終了
    };

private:
    std::string name_;

protected:
    Status status_;
    NodeType type_;

public:
    TreeNode(std::string name);
    virtual ~TreeNode();

    virtual void Initialize() {}
    Status Tick();
    virtual Status Update() = 0;
    virtual void Terminate() {};

    virtual void ResetState() { status_ = Status::INVALID; }
    void SetStatus(Status newState) { status_ = newState; }
    Status GetStatus() { return status_; }
    NodeType GetType() { return type_; }
};

//--------------------------Composite--------------------------
//SequenceとSelectorを実装するための抽象クラス
//TreeNodeを継承したタイプを持つことができる

class CompositeNode : public TreeNode
{
public:
    CompositeNode(std::string name);
    virtual ~CompositeNode();
    virtual Status Update() override = 0;
    virtual void ResetState() override;

    void TerminateChildren(unsigned num);
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
    Sequence(std::string name);
    virtual ~Sequence();
    void Initialize() override;
    Status Update() override;
};

//--------------------------Selector--------------------------

class Selector final : public CompositeNode
{
public:
    Selector(std::string name);
    virtual ~Selector();
    void Initialize() override;
    Status Update() override final;
};

//--------------------------Condition--------------------------

class Condition : public TreeNode
{
public:
    Condition(std::string name);
    ~Condition();
    virtual Status Update() override = 0;
};

//--------------------------Root--------------------------

class Root : public TreeNode
{
    TreeNode* root_;
public:
    Root(std::string name);
    ~Root() override;
    Status Update() override;
    void SetRootNode(TreeNode* root) { root_ = root; }
};

//--------------------------Action--------------------------

class Action : public TreeNode
{
public:
    Action(std::string name);
    virtual ~Action();
    virtual Status Update() override = 0;
};