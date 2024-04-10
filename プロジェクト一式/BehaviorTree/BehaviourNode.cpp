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
    
    return status_ = Update();
    
    status_ = Update();

    return status_;
}

//--------------------------Composite--------------------------

CompositeNode::CompositeNode() : TreeNode()
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

//--------------------------Sequence--------------------------

Sequence::Sequence() : CompositeNode()
{
}

Sequence::~Sequence()
{
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

Selector::Selector() : CompositeNode()
{
}

Selector::~Selector()
{
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

Condition::Condition(TreeNode* child) : TreeNode(), child_(child)
{
    type_ = NodeType::CONDITION_NODE;
}

Condition::~Condition()
{
}

//----------Succedder----------
Succeeder::Succeeder(TreeNode* child) : Condition(child)
{
}

Succeeder::Status Succeeder::Update()
{
    child_->Tick();
    return Status::SUCCESS;
}

//----------Failer----------
Failer::Failer(TreeNode* child) : Condition(child)
{
}

Failer::Status Failer::Update()
{
    child_->Tick();
    return Status::FAILURE;
}

//----------Inverter----------
Inverter::Inverter(TreeNode* child) : Condition(child)
{
}

Inverter::Status Inverter::Update()
{
    auto s = child_->Tick();

    if (s == Status::SUCCESS) return Status::FAILURE;
    else if (s == Status::FAILURE) return Status::SUCCESS;

    return s;
}

//--------------------------Root--------------------------

Root::Root() : TreeNode(), root_(nullptr)
{
}

Root::~Root()
{
    delete root_;
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
