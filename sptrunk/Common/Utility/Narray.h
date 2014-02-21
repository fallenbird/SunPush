#ifndef N_ARRAY_H
#define N_ARRAY_H
#include <assert.h>
//------------------------------------------------------------------------------
template<class TYPE> class nArray
{
public:
    typedef TYPE* iterator;
    enum
    {
        DoubleGrowSize = (1<<0),    // when set, grow size doubles each turn
    };

    /// constructor with default parameters
    nArray();
    /// constuctor with initial size and grow size
    nArray(int initialSize, int initialGrow);
    /// constructor with initial size, grow size and initial values
    nArray(int initialSize, int initialGrow, TYPE initialValue);
    /// copy constructor
    nArray(const nArray<TYPE>& rhs);
    /// destructor
    ~nArray();
    /// assignment operator
    nArray<TYPE>& operator=(const nArray<TYPE>& rhs);
    /// [] operator
    TYPE& operator[](int index) const;

    /// set behaviour flags
    void SetFlags(int f);
    /// get behaviour flags
    int GetFlags() const;
    /// clear contents and set a fixed size
    void SetFixedSize(int size);
    /// push element to back of array
    TYPE& PushBack(const TYPE& elm);
    /// append element to array (synonym for PushBack())
    void Append(const TYPE& elm);
    /// reserve 'num' elements at end of array and return pointer to first element
    iterator Reserve(int num);
    /// get number of elements in array
    int Size() const;
    /// get overall allocated size of array in number of elements
    int AllocSize() const;
    /// set element at index, grow array if necessary
    TYPE& Set(int index, const TYPE& elm);
    /// return reference to nth element in array
    TYPE& At(int index);
    /// return reference to first element
    TYPE& Front() const;
    /// return reference to last element
    TYPE& Back() const;
    /// return true if array empty
    bool Empty() const;
    /// erase element at index
    void Erase(int index);
    /// quick erase, does not call operator= or destructor
    void EraseQuick(int index);
    /// erase element pointed to by iterator
    iterator Erase(iterator iter);
    /// quick erase, does not call operator= or destructor
    iterator EraseQuick(iterator iter);
    /// insert element at index
    void Insert(int index, const TYPE& elm);
    /// clear array (calls destructors)
    void Clear();
    /// reset array (does NOT call destructors)
    void Reset();
    /// return iterator to beginning of array
    iterator Begin() const;
    /// return iterator to end of array
    iterator End() const;
    /// find identical element in array, return iterator
    iterator Find(const TYPE& elm) const;
    /// find identical element in array, return index
    int FindIndex(const TYPE& elm) const;
    /// find array range with element
    void Fill(int first, int num, const TYPE& elm);
    /// clear contents and preallocate with new attributes
    void Reallocate(int initialSize, int grow);

private:
    /// check if index is in valid range, and grow array if necessary
    void CheckIndex(int);
    /// destroy an element (call destructor without freeing memory)
    void Destroy(TYPE* elm);
    /// copy content
    void Copy(const nArray<TYPE>& src);
    /// delete content
    void Delete();
    /// grow array
    void Grow();
    /// grow array to target size
    void GrowTo(int newAllocSize);
    /// move elements, grows array if needed
    void Move(int fromIndex, int toIndex);
    /// unsafe quick move, does not call operator= or destructor
    void MoveQuick(int fromIndex, int toIndex);

    int growSize;           // grow by this number of elements if array exhausted
    int allocSize;          // number of elements allocated
    int numElements;        // number of elements in array
    int flags;
    TYPE* elements;         // pointer to element array
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nArray<TYPE>::nArray() :
    growSize(16),
    allocSize(0),
    numElements(0),
    flags(0)
{
    this->elements = 0;
}

//------------------------------------------------------------------------------
/**
    Note: 'grow' can be zero to create a static preallocated array.
*/
template<class TYPE>
nArray<TYPE>::nArray(int initialSize, int grow) :
    growSize(grow),
    allocSize(initialSize),
    numElements(0),
    flags(0)
{
    assert(initialSize >= 0);
    if (initialSize > 0)
    {
        this->elements = new TYPE[this->allocSize];
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
    Note: 'grow' can be zero to create a static preallocated array.
*/
template<class TYPE>
nArray<TYPE>::nArray(int initialSize, int grow, TYPE initialValue) :
    growSize(grow),
    allocSize(initialSize),
    numElements(initialSize),
    flags(0)
{
    assert(initialSize >= 0);
    if (initialSize > 0)
    {
        this->elements = new TYPE[this->allocSize];
        int i;
        for (i = 0; i < initialSize; i++)
        {
            this->elements[i] = initialValue;
        }
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Copy(const nArray<TYPE>& src)
{
    assert(0 == this->elements);

    this->growSize    = src.growSize;
    this->allocSize   = src.allocSize;
    this->numElements = src.numElements;
    this->flags       = src.flags;
    if (this->allocSize > 0)
    {
        this->elements = new TYPE[this->allocSize];
        int i;
        for (i = 0; i < this->numElements; i++)
        {
            this->elements[i] = src.elements[i];
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Delete()
{
    this->growSize = 0;
    this->allocSize = 0;
    this->numElements = 0;
    this->flags = 0;
    if (this->elements)
    {
        delete[] this->elements;
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Destroy(TYPE* elm)
{
	elm;// 消除一个警告 [2009-12-16 13:25:03  ]
    elm->~TYPE();//当T是内建类型比如int时，因为没有析构函数，所以此行代码被优化掉，编译器认为elm没有被使用过
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nArray<TYPE>::nArray(const nArray<TYPE>& rhs) :
    growSize(0),
    allocSize(0),
    numElements(0),
    elements(0),
    flags(0)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nArray<TYPE>::~nArray()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::SetFlags(int f)
{
    this->flags = f;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
int
nArray<TYPE>::GetFlags() const
{
    return this->flags;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Reallocate(int initialSize, int grow)
{
    this->Delete();
    this->growSize    = grow;
    this->allocSize   = initialSize;
    this->numElements = 0;
    if (initialSize > 0)
    {
        this->elements = new TYPE[initialSize];
    }
    else
    {
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
    Set a new fixed size. This will throw away the current content, and
    create preallocate a new array which cannot grow. All elements in
    the array will be valid.
*/
template<class TYPE>
void
nArray<TYPE>::SetFixedSize(int size)
{
    this->Reallocate(size, 0);
    this->numElements = size;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nArray<TYPE>&
nArray<TYPE>::operator=(const nArray<TYPE>& rhs)
{
    this->Delete();
    this->Copy(rhs);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::GrowTo(int newAllocSize)
{
    TYPE* newArray = new TYPE[newAllocSize];

    if (this->elements)
    {
        // copy over contents
        int i;
        for (i = 0; i < this->numElements; i++)
        {
            newArray[i] = this->elements[i];
        }

        // discard old array and update contents
        delete[] this->elements;
    }
    this->elements  = newArray;
    this->allocSize = newAllocSize;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Grow()
{
    assert(this->growSize > 0);
    int growToSize;
    if ((DoubleGrowSize & this->flags) != 0)
    {
        // double growth behaviour
        if (0 == this->allocSize)
        {
            growToSize = growSize;
        }
        else
        {
            growToSize = 2 * this->allocSize;
        }
    }
    else
    {
        // classic linear growth behaviour
        growToSize = this->allocSize + this->growSize;
    }
    this->GrowTo(growToSize);
}

//------------------------------------------------------------------------------
/**
    30-Jan-03   floh    serious bugfixes!
*/
template<class TYPE>
void
nArray<TYPE>::Move(int fromIndex, int toIndex)
{
    assert(0!=this->elements);
    assert(fromIndex < this->numElements);

    // nothing to move?
    if (fromIndex == toIndex)
    {
        return;
    }

    // compute number of elements to move
    int num = this->numElements - fromIndex;

    // check if array needs to grow
    int neededSize = toIndex + num;
    while (neededSize >= this->allocSize)
    {
        this->Grow();
    }

    if (fromIndex > toIndex)
    {
        // this is a backward move
        int i;
        for (i = 0; i < num; i++)
        {
            this->elements[toIndex + i] = this->elements[fromIndex + i];
        }

        // destroy remaining elements
        for (i = (fromIndex + i) - 1; i < this->numElements; i++)
        {
            this->Destroy(&(this->elements[i]));
        }
    }
    else
    {
        // this is a forward move
        int i;
        for (i = num - 1; i >= 0; --i)
        {
            this->elements[toIndex + i] = this->elements[fromIndex + i];
        }

        // destroy freed elements
        for (i = fromIndex; i < toIndex; i++)
        {
            this->Destroy(&(this->elements[i]));
        }
    }

    // adjust array size
    this->numElements = toIndex + num;
}

//------------------------------------------------------------------------------
/**
    Very fast move which does not call assignment operators or destructors,
    so you better know what you do!
*/
template<class TYPE>
void
nArray<TYPE>::MoveQuick(int fromIndex, int toIndex)
{
    assert(0 != this->elements);
    assert(fromIndex < this->numElements);

    // compute number of elements to move
    int num = this->numElements - fromIndex;

    // nothing to move?
    if (fromIndex == toIndex)
    {
        return;
    }

    // do a direct memory move
    memmove(&(this->elements[toIndex]), &(this->elements[fromIndex]), num * sizeof(TYPE));

    // adjust array size
    this->numElements = toIndex + num;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
nArray<TYPE>::PushBack(const TYPE& elm)
{
    // grow allocated space if exhausted
    if (this->numElements == this->allocSize)
    {
        this->Grow();
    }
    assert(0 != this->elements);
    this->elements[this->numElements] = elm;
    return this->elements[this->numElements++];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Append(const TYPE& elm)
{
    // grow allocated space if exhausted
    if (this->numElements == this->allocSize)
    {
        this->Grow();
    }
    assert(0!=this->elements);
    this->elements[this->numElements++] = elm;
}

//------------------------------------------------------------------------------
/**
    Make room for N new elements at the end of the array, and return a pointer 
    to the start of the reserved area. This can be (carefully!) used as a fast 
    shortcut to fill the array directly with data.
*/
template<class TYPE>
typename nArray<TYPE>::iterator
nArray<TYPE>::Reserve(int num)
{
    assert(num > 0);
    int maxElement = this->numElements + num;
    while (maxElement >= this->allocSize)
    {
        this->Grow();
    }
    assert(this->elements);
    iterator iter = this->elements + this->numElements;
    this->numElements += num;
    return iter;
}

//------------------------------------------------------------------------------
/**
    This will check if the provided index is in the valid range. If it is
    not the array will be grown to that index.
*/
template<class TYPE>
void
nArray<TYPE>::CheckIndex(int index)
{
    if (index >= this->numElements)
    {
        // grow array if necessary
        if (index >= this->allocSize)
        {
            assert(this->growSize > 0);
            this->GrowTo(index + this->growSize);
        }
        // update number of contained elements
        this->numElements = index + 1;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
nArray<TYPE>::Set(int index, const TYPE& elm)
{
    assert(index < this->numElements);
    this->CheckIndex(index);
    this->elements[index] = elm;
    return this->elements[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
int
nArray<TYPE>::Size() const
{
    return this->numElements;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
int
nArray<TYPE>::AllocSize() const
{
    return this->allocSize;
}

//------------------------------------------------------------------------------
/**
    Access an element. This method may grow the array if the index is
    outside the array range.
*/
template<class TYPE>
TYPE&
nArray<TYPE>::At(int index)
{
    this->CheckIndex(index);
    return this->elements[index];
}

//------------------------------------------------------------------------------
/**
    Access an element. This method will NOT grow the array, and instead do
    a range check, which may throw an assertion.
*/
template<class TYPE>
TYPE&
nArray<TYPE>::operator[](int index) const
{
    assert(this->elements && (index >= 0) && (index < this->numElements));
    return this->elements[index];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
nArray<TYPE>::Front() const
{
    assert(this->elements && (this->numElements > 0));
    return this->elements[0];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
nArray<TYPE>::Back() const
{
    assert(this->elements && (this->numElements > 0));
    return this->elements[this->numElements - 1];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
nArray<TYPE>::Empty() const
{
    return (this->numElements == 0);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Erase(int index)
{
    assert(this->elements && (index >= 0) && (index < this->numElements));
    if (index == (this->numElements - 1))
    {
        // special case: last element
        this->Destroy(&(this->elements[index]));
        this->numElements--;
    }
    else
    {
        this->Move(index + 1, index);
    }
}

//------------------------------------------------------------------------------
/**
    Quick erase, uses memmove() and does not call assignment operators
    or destructor, so be careful about that!
*/
template<class TYPE>
void
nArray<TYPE>::EraseQuick(int index)
{
    assert(this->elements && (index >= 0) && (index < this->numElements));
    if (index == (this->numElements - 1))
    {
        // special case: last element
        this->numElements--;
    }
    else
    {
        this->MoveQuick(index + 1, index);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
typename nArray<TYPE>::iterator
nArray<TYPE>::Erase(typename nArray<TYPE>::iterator iter)
{
    assert(this->elements && (iter >= this->elements) && (iter < (this->elements + this->numElements)));
    this->Erase(iter - this->elements);
    return iter;
}

//------------------------------------------------------------------------------
/**
    Quick erase, uses memmove() and does not call assignment operators
    or destructor, so be careful about that!
*/
template<class TYPE>
typename nArray<TYPE>::iterator
nArray<TYPE>::EraseQuick(typename nArray<TYPE>::iterator iter)
{
    assert(this->elements && (iter >= this->elements) && (iter < (this->elements + this->numElements)));
    this->EraseQuick(iter - this->elements);
    return iter;
}


//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nArray<TYPE>::Insert(int index, const TYPE& elm)
{
    assert((index >= 0) && (index <= this->numElements));
    if (index == this->numElements)
    {
        // special case: append element to back
        this->PushBack(elm);
    }
    else
    {
        this->Move(index, index + 1);
        this->elements[index] = elm;
    }
}

//------------------------------------------------------------------------------
/**
    The current implementation of this method does not shrink the
    preallocated space. It simply sets the array size to 0.
*/
template<class TYPE>
void
nArray<TYPE>::Clear()
{
    int i;
    for (i = 0; i < this->numElements; i++)
    {
        this->Destroy(&(this->elements[i]));
    }
    this->numElements = 0;
}

//------------------------------------------------------------------------------
/**
    This is identical with Clear(), but does NOT call destructors (it just
    resets the numElements member. USE WITH CARE!
*/
template<class TYPE>
void
nArray<TYPE>::Reset()
{
    this->numElements = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
typename nArray<TYPE>::iterator
nArray<TYPE>::Begin() const
{
    return this->elements;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
typename nArray<TYPE>::iterator
nArray<TYPE>::End() const
{
    return this->elements + this->numElements;
}

//------------------------------------------------------------------------------
/**
    Find element in array, return iterator, or 0 if element not
    found.

    @param elm          element to find
    @return             element iterator, or 0 if not found
*/
template<class TYPE>
typename nArray<TYPE>::iterator
nArray<TYPE>::Find(const TYPE& elm) const
{
    int index;
    for (index = 0; index < this->numElements; index++)
    {
        if (this->elements[index] == elm)
        {
            return &(this->elements[index]);
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    Find element in array, return element index, or -1 if element not
    found.

    @param  elm     element to find
    @return         index to element, or -1 if not found
*/
template<class TYPE>
int
nArray<TYPE>::FindIndex(const TYPE& elm) const
{
    int index;
    for (index = 0; index < this->numElements; index++)
    {
        if (this->elements[index] == elm)
        {
            return index;
        }
    }
    return -1;
}

//------------------------------------------------------------------------------
/**
    Fills an array range with the given element value. Will grow the
    array if necessary

    @param  first   index of first element to start fill
    @param  num     num elements to fill
    @param  elm     fill value
*/
template<class TYPE>
void
nArray<TYPE>::Fill(int first, int num, const TYPE& elm)
{
    if ((first + num) > this->numElements)
    {
        this->GrowTo(first + num);
    }
    int i;
    for (i = first; i < (first + num); i++)
    {
        this->elements[i] = elm;
    }
}

//------------------------------------------------------------------------------
#endif
