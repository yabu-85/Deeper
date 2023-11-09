#pragma once
#include <vector>
#include <string>

namespace BT {

    enum NodeType {
        ACTION_NODE,
        CONDITION_NODE,
        CONTROL_NODE
    };

    enum Status {
        INVALID,
        SUCCESS,
        FAILURE,
        IDLE,
        RUNNING,
        ABORTED
    };

    //--------------------------Base--------------------------

    class TreeNode
    {
        std::string name_;

    protected:
        NodeType type_;
        Status status_;

    public:
        TreeNode(std::string name);
        virtual ~TreeNode();
        virtual void Initialize() {}

        Status Tick();
        virtual Status Update() = 0;            //実行
        virtual void Abort() = 0;  //ノードの実行を中断

        virtual void ResetState() = 0;
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
        virtual Status Update() override;

        void AvortChildren(unsigned num);
        void ResetState() override;
        void Abort() override {};

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

    //--------------------------Action--------------------------

    class Action : public TreeNode
    {
    public:
        Action(std::string name);
        virtual ~Action();
        void ResetState() override;
        void Abort() override {};

    };

}


