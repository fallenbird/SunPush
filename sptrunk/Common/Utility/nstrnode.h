#ifndef N_STRNODE_H
#define N_STRNODE_H

//------------------------------------------------------------------------------
/**
    @brief A node in a nStrList.

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/

#include <string.h>
#include "nnode.h"
#include "nstring.h"

//------------------------------------------------------------------------------
class nStrNode: public nNode 
{
public:
    /// default constructor
    nStrNode();
    /// constructor providing custom data pointer
    nStrNode(void* ptr);
    /// constructor providing node name
    nStrNode(const char* str);
    /// constructor providing custom data pointer and node name
    nStrNode(const char* str, void* ptr);
    /// set the name of this node
    void SetName(const char* str);
    /// get the name of this node
    const char* GetName() const;
    /// get next node in list
    nStrNode* GetSucc() const;
    /// get previous node in list
    nStrNode* GetPred() const;

private:
    nString name;
};

//------------------------------------------------------------------------------
/**
*/
inline
nStrNode::nStrNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nStrNode::nStrNode(void* ptr) :
    nNode(ptr)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nStrNode::nStrNode(const char* str) :
    name(str)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nStrNode::nStrNode(const char* str, void* ptr) :
    nNode(ptr),
    name(str)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nStrNode::SetName(const char* str)
{
    this->name.Set(str);
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
nStrNode::GetName() const
{
    return this->name.IsEmpty() ? 0 : this->name.Get();
}

//------------------------------------------------------------------------------
/**
*/
inline
nStrNode*
nStrNode::GetSucc() const
{
    return (nStrNode *) nNode::GetSucc();
}

//------------------------------------------------------------------------------
/**
*/
inline
nStrNode*
nStrNode::GetPred() const
{
    return (nStrNode *) nNode::GetPred();
}

//------------------------------------------------------------------------------
#endif
