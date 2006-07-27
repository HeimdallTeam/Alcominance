#ifndef _CRR_H_
#define _CRR_H_

class LinkedList{

private:
    struct item{
        item* next;
        void* value;        
    } Item;
    
    Item *head, *tail;
    
public:
    LinkedList();
    ~LinkedList();

    void add(void* value);
    void remove(Item* item);
    bool isEmpty();
    Item* getHead();
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
    LinkedList* getResources(resourceType);
};


#endif