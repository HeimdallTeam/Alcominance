#ifndef _CRR_H_
#define _CRR_H_

class LinkedList{

private:
    struct item{
        item* next;
        void* value;
    } Item;
    
    Item* head, tail;
    
public:
    LinkedList();
    ~LinkedList();

    add(void* value);
    remove(Item* item);
};

/**
 * Central Register of Resource
 */
class CRR{

private:
    /** TODO predelat pole spojaku na spojak spojaku */
    LinkedList* resLists;
    int resCount;

public:
    CRR();
    ~CRR();
    void addResource(ResourceItem* item);
    LinkedList getResource(resourceType);
};


#endif