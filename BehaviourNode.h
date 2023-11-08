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
        virtual ~CompositeNode() = 0;
        void AvortChild(unsigned int num);
        void ResetState() override;

        void AddChild(TreeNode* child) { childNodes_.push_back(child); }
        unsigned int GetChildrenNumber() { return static_cast<unsigned int>(childNodes_.size()); }
        std::vector<TreeNode*> GetChildren() { return childNodes_; }

    private:
        std::vector<TreeNode*> childNodes_;
        unsigned int childCount_;
    };

    //--------------------------Selector--------------------------

    class Selector final : public CompositeNode
    {
    public:
        virtual ~Selector() = default;
        Status Update() override final;

    };

    //--------------------------Sequence--------------------------

    class Sequence : public CompositeNode
    {
    public:
        virtual ~Sequence() = default;
        Status Update() override;
    };

    //--------------------------Root--------------------------

    class Root : public Sequence
    {
    public:
        virtual ~Root() = default;
        Status Update() override final;
    };

}


