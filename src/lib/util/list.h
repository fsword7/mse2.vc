// list.h - linked list package
//
// Date:    Apr 30, 2023
// Author:  Tim Stark

#pragma once
template <typename T>
class List
{
public:
    List() { list.clear(); }
    virtual ~List() = default;
    
    inline void addNode(T *node)    { list.push_back(node); }

    inline bool hasChildren() const { return list.size() > 0; }
    inline int getSize() const      { return list.size(); }

    inline T *getFirst() const  { cidx = 0; return getNext(); }
    inline T *getNext() const   { return cidx < list.size() ? list[cidx++] : nullptr; }
    
    inline void reset() const   { cidx = 0; }

    const std::vector<T *> &getList() const { return list; }

private:
    std::vector<T *> list;
    mutable int cidx = 0;
};