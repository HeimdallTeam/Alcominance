#ifndef _CRR_H_
#define _CRR_H_

#include "buildings.h"

/**
 * Spojovy seznam
 */
class LinkedList{

private:

    struct item{
            item* next;
            void* value;
/*
        public:
            // prochazeni pomoci start(), next() a hasNext()
            item* getNext(){
                return next;
            }
            void setNext(item* next){
                this->next=next;
            }
            void* getValue(){
                return value;
            }
            void setValue(void* value){
                this->value=value;
            }
*/
    };
    
    item* getHead();
    item* getActual();

    item *head, *tail;
    item* actual;

public:
    
    LinkedList();
    ~LinkedList();

    void add(void* value);
    void remove(void* value);
    bool isEmpty();

    void* next();
    bool hasNext();
    void start();    
};

/**
 * Central Register of Resource
 * Centralni registr zdroju (surovin) slouzi ...
 */
class CRR {

private:
    /** TODO predelat pole spojaku na pole poli */
    LinkedList* resLists;
    int resCount;

public:
    CRR(int resCount);
    ~CRR();
    void addResource(ResourceItem* item);
    LinkedList* getResources(ESurType resourceType);
};


#endif