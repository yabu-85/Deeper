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
        INVALID,    //初期状態
        SUCCESS,    //成功
        FAILURE,    //失敗
        //RUNNING,  //実行中（使い道が今のところない
    };

protected:
    Status status_;
    NodeType type_;

public:
    TreeNode();
    virtual ~TreeNode();
    Status Tick();

    virtual Status Update() = 0;    //本体処理
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
};

//--------------------------Sequence--------------------------

//失敗するまで続ける、最後まで失敗しない場合SUCCESSが帰ってくる
class Sequence : public CompositeNode
{
public:
    Sequence();
    virtual ~Sequence() override;
    Status Update() override;
};

//--------------------------Selector--------------------------

//成功するまで続ける、最後まで成功しない場合FAILUREが帰ってくる
class Selector : public CompositeNode
{
public:
    Selector();
    virtual ~Selector() override;
    Status Update() override;
};

//--------------------------Condition--------------------------

//Decoletarの役割
class Condition : public TreeNode
{
protected:
    TreeNode* child_;
public:
    Condition(TreeNode* child);
    virtual ~Condition() override;
    virtual Status Update() override = 0;
};

//必ずSUCCESSを返す
class Succeeder : public Condition
{
public:
    Succeeder(TreeNode* child);
    Status Update() override;
};

//必ずFAILUREを返す
class Failer : public Condition
{
public:
    Failer(TreeNode* child);
    Status Update() override;
};

//逆の結果を返す（SuccesだったらFailure）
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