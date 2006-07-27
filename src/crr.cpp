#include "StdAfx.h"
#include "utils.h"
#include "becher.h"
#include "crr.h"

LinkedList::LinkedList(){
    head=actual=tail=NULL;
}

LinkedList::~LinkedList(){

    item* actual = head;
    item* next;
    
    while(actual != NULL){
        next = actual->next;
        delete(actual);
        actual=next;
    }
}

/**
 * 
 */
bool LinkedList::isEmpty(){
    return head==NULL ? true : false;
}

/**
 * Nastavi prochazeni(pomoci next() a hasNext()) na pocatek.
 */
void LinkedList::start(){
    actual = head;
}

/**
 * Test na existenci nasledujiciho prvku. Pouziti pri prochazeni.
 */
bool LinkedList::hasNext(){

    if(isEmpty()) return false;
    if(actual==head) return true;
    return actual->next == NULL ? false : true;
}

/**
 * Vrati hodnotu dalsiho prvku. Pouziti pri prochazeni.
 */
void* LinkedList::next(){
    
    if(actual != head) actual = actual->next;
    return actual->value;
}

/**
 * Vrati hlavicku (pocatek) spojoveho seznamu.
 */
LinkedList::item* LinkedList::getHead(){
    return head;
}

/**
 * Vrati hlavicku (pocatek) spojoveho seznamu.
 */
LinkedList::item* LinkedList::getActual(){
    return actual;
}

/**
 * Prida hodnotu do spojoveho seznamu.
 */
void LinkedList::add(void* value){

    if(head == NULL){
        head = new item;
        head->value=value;
        head->next=NULL;
        tail=head;
    }else{
        tail->next = new item;
        tail=tail->next;
        tail->value=value;
        tail->next=NULL;
    }
}

/** TODO odebirani prvku (podle prvku nebo jeho hodnoty? opakovani) */
void LinkedList::remove(void* value){
    
    item* previous = NULL;
    item* next;

    start();
    while(hasNext()){

        void* actualValue = next();
        item* actual = getActual();
        if(actualValue==value){
            if(previous != NULL) previous->next=next;
            else head=next;

            delete(actual);
        }
        previous=actual;
    }
}



/**
 * Zalozi registr zdroju pro urcity pocet typu zdroju.
 * @param resCount - pocet typu zdroju
 */
CRR::CRR(int resCount){
    
    this->resCount = resCount;
    resLists = new LinkedList[resCount];
}

CRR::~CRR(){
    
    SAFE_DELETE_ARRAY(resLists);
}

/**
 * Prida resource item do registru podle jeho typu. Pokud je to novy typ, vytvori dalsi "slot".
 */
void CRR::addResource(ResourceItem* item){

    int n;
    for(n=0; n<resCount; n++){

        if(resLists[n].isEmpty()) break;        
        resLists[n].start();
        if(((ResourceItem*) resLists[n].next())->GetType() == item->GetType()){
            //typ existuje
            resLists[n].add(item);
            return;
        }
    }

    //typ neexistuje, vytvori se dalsi "slot"
    resLists[n].add(item);
    
}

/**
 * Vrati zdroje (suroviny) daneho typu.
 */
LinkedList* CRR::getResources(ESurType resourceType){

    int n;
    for(n=0; n<resCount; n++){

        if(resLists[n].isEmpty()) break;
        resLists[n].start();
        if(((ResourceItem*) resLists[n].next())->GetType() == resourceType){
            return &resLists[n];
        }
    }
    
    //typ neexistuje
    return NULL;
}