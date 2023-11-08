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
    : TreeNode(name), childCount_(0)
{
    type_ = CONTROL_NODE;
}

void BT::CompositeNode::ResetState()
{
    status_ = IDLE;
    for (unsigned int i = 0; i < childNodes_.size(); i++)
    {
        childNodes_[i]->ResetState();
    }
}

void CompositeNode::AvortChild(unsigned int num)
{
    for (unsigned int j = num; j < childNodes_.size(); j++)
    {
        if (childNodes_[j]->GetType() == BT::CONDITION_NODE)
        {
            childNodes_[num]->ResetState();
        }
        else
        {
            if (childNodes_[j]->GetStatus() == RUNNING)
            {
                //SENDING HALT TO CHILD " << childNodes_[j]->get_name());
                childNodes_[j]->Abort();
            }
            else
            {
                //"NO NEED TO HALT " << childNodes_[j]->get_name() "STATUS" << childNodes_[j]->get_status());
            }
        }
    }
}

//--------------------------Selector--------------------------

Status Selector::Update()
{
    for (auto& node : this->GetChildren()) { if (node->Update() == true) { return SUCCESS; } } //true
    return RUNNING; //false
}

//--------------------------Sequence--------------------------

Status Sequence::Update()
{
    return Status();
}

//--------------------------Root--------------------------

Status Root::Update()
{
    return Status();
}