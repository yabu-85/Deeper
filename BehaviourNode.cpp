#include "BehaviourNode.h"

//--------------------------Base--------------------------

TreeNode::TreeNode(std::string name)
    : name_(name), status_(Status::INVALID), type_(NodeType::ACTION_NODE)
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

CompositeNode::CompositeNode(std::string name)
    : TreeNode(name), currentIndex_(0)
{
    type_ = NodeType::CONTROL_NODE;
}

CompositeNode::~CompositeNode()
{
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

Sequence::Sequence(std::string name)
    : CompositeNode(name)
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

    //ÅŒã‚Ìƒm[ƒh‚Ü‚Å¬Œ÷‚µ‚Ä‚È‚¢ê‡
    return Status::SUCCESS;
}

//--------------------------Selector--------------------------

Selector::Selector(std::string name)
    : CompositeNode(name)
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

    //ÅŒã‚Ìƒm[ƒh‚Ü‚ÅŽ¸”s‚µ‚Ä‚È‚¢ê‡
    return Status::FAILURE;
    
}

//--------------------------Condition--------------------------

Condition::Condition(std::string name)
    : TreeNode(name)
{
    type_ = NodeType::CONDITION_NODE;
}

Condition::~Condition()
{
}

//--------------------------Root--------------------------

Root::Root(std::string name)
    : TreeNode(name), root_(nullptr)
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

Action::Action(std::string name)
    : TreeNode(name)
{
    type_ = NodeType::ACTION_NODE;
}

Action::~Action()
{
}
