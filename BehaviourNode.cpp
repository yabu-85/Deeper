#include "BehaviourNode.h"
#include <algorithm>  // for_each

//--------------------------Base--------------------------

TreeNode::TreeNode() : status_(Status::INVALID), type_(NodeType::ACTION_NODE)
{
}

TreeNode::~TreeNode()
{
}

TreeNode::Status TreeNode::Tick()
{
    if (status_ == Status::INVALID)
    {
        Initialize();
    }

    status_ = Update();

    if (status_ != Status::RUNNING)
    {
        Terminate();
    }
    return status_;
}

//--------------------------Composite--------------------------

CompositeNode::CompositeNode() : TreeNode(), currentIndex_(0)
{
    type_ = NodeType::CONTROL_NODE;
}

CompositeNode::~CompositeNode()
{
    for_each(childlenNodes_.begin(), childlenNodes_.end(), [](TreeNode* node) {
        delete node;
    });
}

void CompositeNode::ResetState()
{
    status_ = Status::INVALID;
    for (unsigned i = 0; i < childlenNodes_.size(); i++)
    {
        childlenNodes_[i]->ResetState();
    }
}

void CompositeNode::TerminateChildren(unsigned num)
{
    for (unsigned j = num; j < childlenNodes_.size(); j++)
    {
        if (childlenNodes_[j]->GetType() == NodeType::CONDITION_NODE)
        {
            childlenNodes_[num]->ResetState();
        }
        else
        {
            if (childlenNodes_[j]->GetStatus() == Status::RUNNING)
            {
                childlenNodes_[j]->Terminate();
            }
        }
    }
}

//--------------------------Sequence--------------------------

Sequence::Sequence() : CompositeNode()
{
}

Sequence::~Sequence()
{
}

void Sequence::Initialize()
{
    currentIndex_ = 0;
}

Sequence::Status Sequence::Update()
{
    Status s;
    for (auto& node : this->GetChildren()) {
        s = node->Tick();
        if (s != Status::SUCCESS) {
            return s;
        }
    }

    //�Ō�̃m�[�h�܂Ő������ĂȂ��ꍇ
    return Status::SUCCESS;
}

//--------------------------Selector--------------------------

Selector::Selector() : CompositeNode()
{
}

Selector::~Selector()
{
}

void Selector::Initialize()
{
    currentIndex_ = 0;
}

Selector::Status Selector::Update()
{
    Status s;
    for (auto& node : this->GetChildren()) {
        s = node->Tick();
        if (s != Status::FAILURE) {
            return s;
        }
    }

    //�Ō�̃m�[�h�܂Ŏ��s���ĂȂ��ꍇ
    return Status::FAILURE;
    
}

//--------------------------Condition--------------------------

Condition::Condition() : TreeNode()
{
    type_ = NodeType::CONDITION_NODE;
}

Condition::~Condition()
{
}

//--------------------------Root--------------------------

Root::Root() : TreeNode(), root_(nullptr)
{
}

Root::~Root()
{
}

Root::Status Root::Update()
{
    return root_->Tick();
}

//--------------------------Action--------------------------

Action::Action() : TreeNode()
{
    type_ = NodeType::ACTION_NODE;
}

Action::~Action()
{
}
