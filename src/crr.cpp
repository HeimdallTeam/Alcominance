#include "StdAfx.h"
#include "crr.h"

LinkedList::LinkedList(){
    head=tail=NULL;
}

LinkedList::~LinkedList(){

    Item* actual = head;
    Item* next;
    
    while(actual != NULL){
        next = actual->next;
        delete(actual);
        actual=next;
    }
}

/**
 * Prida hodnotu do spojoveho seznamu.
 */
void LinkedList::add(void* value){

    if(head == NULL){
        head = new Item;
        head->value=value;
        head->next=NULL;
        tail=head;
    }else{
        tail->next = new Item;
        tail=tail->next;
        tail->value=value;
        tail->next=NULL;
    }
}


/** TODO odebirani prvku (podle prvku nebo jeho hodnoty? opakovani) */
void LinkedList::remove(Item* item){
    
}

/**
 * 
 */
bool LinkedList::isEmpty(){
    return head==null ? true : false;
}

/**
 * Vrati hlavicku (pocatek) spojoveho seznamu.
 */
Item* LinkedList::getHead(){
    return head;
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

    for(int n=0; n<resCount; n++){

        if(resLists[n].isEmpty()) break;
        if(resLists[n].getHead().getValue().getType() == item.getType()){
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
LinkedList* CRR::getResources(resourceType){

    for(int n=0; n<resCount; n++){
        if(resLists[n].getHead().getValue().getType() == resourceType){
            return resLists[n];
        }
    }
    if(n==resCount){
        //typ neexistuje
        retun NULL;
    }
}