#include "BehaviourNode.h"
using namespace BT;

//--------------------------Base--------------------------

TreeNode::TreeNode(std::string name)
    : status_(INVALID), type_(ACTION_NODE)
{
}

TreeNode::~TreeNode()
{
}

Status TreeNode::Tick()
{
    if (status_ == INVALID)
    {
        Initialize();
    }

    status_ = Update();

    if (status_ != RUNNING)
    {
        Abort();
    }
    return status_;
}

//--------------------------Composite--------------------------

CompositeNode::CompositeNode(std::string name)
    : TreeNode(name), currentIndex_(0)
{
    type_ = CONTROL_NODE;
}

CompositeNode::~CompositeNode()
{
}

Status CompositeNode::Update()
{
    return Status();
}

void CompositeNode::ResetState()
{
    status_ = IDLE;
    for (unsigned i = 0; i < childlenNodes_.size(); i++)
    {
        childlenNodes_[i]->ResetState();
    }
}

void CompositeNode::AvortChildren(unsigned num)
{
    for (unsigned j = num; j < childlenNodes_.size(); j++)
    {
        if (childlenNodes_[j]->GetType() == CONDITION_NODE)
        {
            childlenNodes_[num]->ResetState();
        }
        else
        {
            if (childlenNodes_[j]->GetStatus() == RUNNING)
            {
                //SENDING HALT TO CHILD " << childNodes_[j]->get_name());
                childlenNodes_[j]->Abort();
            }
            else
            {
                //"NO NEED TO HALT " << childNodes_[j]->get_name() "STATUS" << childNodes_[j]->get_status());
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

Status Sequence::Update()
{
    Status s;
    for (auto& node : this->GetChildren()) {
        s = node->Tick();
        if (s != SUCCESS) {
            return s;
        }
    }

    //ç≈å„ÇÃÉmÅ[ÉhÇ‹Ç≈ê¨å˜ÇµÇƒÇ»Ç¢èÍçá
    return SUCCESS;
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

Status Selector::Update()
{
    Status s;
    for (auto& node : this->GetChildren()) {
        s = node->Tick();
        if (s != FAILURE) {
            return s;
        }
    }

    //ç≈å„ÇÃÉmÅ[ÉhÇ‹Ç≈é∏îsÇµÇƒÇ»Ç¢èÍçá
    return FAILURE;
    
}

//--------------------------Selector--------------------------

Action::Action(std::string name)
    : TreeNode(name)
{
    type_ = ACTION_NODE;
}

Action::~Action()
{
}

void BT::Action::ResetState()
{
    status_ = IDLE;
}
