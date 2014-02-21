#ifndef N_LIST_H
#define N_LIST_H

#include "nnode.h"

//------------------------------------------------------------------------------
class nList 
{
public:
    /// constructor
    nList();
    /// destructor (list must be empty!)
    ~nList();
    /// checks if list is empty
    bool IsEmpty() const;
    /// returns first node in list, 0 if empty
    nNode* GetHead() const;
    /// returns last node in list, 0 if empty
    nNode* GetTail() const;
    /// add node to head of list
    void AddHead(nNode* n);
    /// add node to end of list
    void AddTail(nNode* n);
    /// remove node from head of list
    nNode* RemHead();
    /// remove node from tail of list
    nNode* RemTail();

private:
    nNode* head;
    nNode* tail;
    nNode* tailpred;
};
    
//-----------------------------------------------------------------------------
/**
*/
inline 
nList::nList()
{
    this->head     = (nNode *) &(this->tail);
    this->tail     = 0;
    this->tailpred = (nNode *) &(this->head);
}

//-----------------------------------------------------------------------------
/**
    Destroy list object. The list has to be empty, otherwise an assertion
    will be thrown.
*/
inline nList::~nList()
{
    assert(0 == this->head->succ);
}

//-----------------------------------------------------------------------------
/**
    @return true if list is empty.
*/
inline 
bool 
nList::IsEmpty() const
{
    return (this->head->succ == 0);
}

//-----------------------------------------------------------------------------
/**
    @return return first node in list, or 0 if list is empty
*/
inline 
nNode*
nList::GetHead() const
{
    if (this->head->succ) 
    {
        return this->head;
    }
    else                  
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------
/**
    @return return last node in list, or 0 if list is empty
*/
inline 
nNode*
nList::GetTail() const
{
    if (this->tailpred->pred)
    {
        return this->tailpred;
    }
    else                      
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------
/**
    @param n the node to be added
*/
inline 
void 
nList::AddHead(nNode *n)
{
    n->InsertAfter((nNode *) &(this->head));
}

//-----------------------------------------------------------------------------
/**
    @param n the node to be added
*/
inline 
void 
nList::AddTail(nNode *n)
{
    n->InsertBefore((nNode *) &(this->tail));
}

//-----------------------------------------------------------------------------
/**
    Remove node at head of list, and return pointer to it. Returns 0 if
    list is empty.

    @return the removed node or 0 if list is empty
*/
inline nNode *nList::RemHead()
{
    nNode *n = this->head;
    if (n->succ) 
    {
        n->Remove();
        return n;
    } 
    else 
    {
        return 0;
    }
}

//-----------------------------------------------------------------------------
/**
    Remove node at tail of list, and return pointer to it. Returns 0 if
    list is empty.

    @return the removed node or 0 if list is empty
*/
inline nNode *nList::RemTail()
{
    nNode *n = this->tailpred;
    if (n->pred) 
    {
        n->Remove();
        return n;
    } 
    else 
    {
        return 0;
    }
}
//--------------------------------------------------------------------
#endif
