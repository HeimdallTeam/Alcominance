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

    start();
    while(hasNext()){

        void* actualValue = next();
        item* actual = getActual();
        if(actualValue==value){
            if(previous != NULL) previous->next=actual->next;
            else head=actual->next;

            delete(actual);
        }
        previous=actual;
    }
}

CRR* CRR::this_ = NULL;

/**
 * Zalozi registr zdroju pro urcity pocet typu zdroju.
 * @param resCount - pocet typu zdroju
 */
CRR::CRR(int resCount){
    
    this->resCount = resCount;
    resLists = new LinkedList[resCount];
    this_=this;
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
 * Vrati zdroje (suroviny) daneho typu podle kriteria.
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
    return NULL;
}

/**
 * Vybere zdroje podle kriteria (priority/distance).
 * @param list seznam zdroju ze kterych se vybira
 */
LinkedList* CRR::getResources(LinkedList* list, ESurCriterion criterion){
    
    LinkedList* resList=new LinkedList();
    
    if(list==NULL || list->isEmpty()) return resList;

    list->start();
    
    ResourceItem* resource = (ResourceItem*) list->next();
    int minValue;
    if(criterion==EBSC_Priority){
        minValue=resource->GetPriority();
    }
    /*
    if(criterion==EBSC_Distance){
        minValue=resource->GetDistance(pos (troll));
    }
    */

    int value;
    while(list->hasNext()){

        resource = (ResourceItem*) list->next();
        if(criterion==EBSC_Priority) value=resource->GetPriority();
//      if(criterion==EBSC_Distance) value=resource->GetDistance();

        if(value<minValue){
            minValue=value;
        }            
    }

    list->start();
    while(list->hasNext()){

        resource = (ResourceItem*) list->next();
        if(criterion==EBSC_Priority) value=resource->GetPriority();
//      if(criterion==EBSC_Distance) value=resource->GetDistance();

        if(value==minValue) resList->add(resource);
    }

    return resList;
}

/**
 * Vrati zdroje (suroviny) daneho typu podle kriteria.
 * @param ESurType typ - typ suroviny (int)
 * @param ESurCriterion criterion1 - primarni kriterium vyberu suroviny - distance/priority (int)
 * @param ESurCriterion criterion2 - sekundarni kriterium vyberu suroviny - distance/priority (int)
 */
int CRR::l_getResources(lua_State * L){

	assert(this_);
    HoeGame::LuaParam lp(L);

    ESurType type;
    ESurCriterion criterion1, criterion2;

    if(lp.GetNumParam()!=2 && lp.GetNumParam()!=3) lp.Error("nespravny pocet parametru (2 nebo 3)");
    if(!lp.IsNum(-1)) lp.Error("nespravny typ parametru 1 (ESurType)");
    if(!lp.IsNum(-2)) lp.Error("nespravny typ parametru 2 (ESurCriterion)");

    type = (ESurType) lp.GetNum(-1);
    criterion1 = (ESurCriterion) lp.GetNum(-2);
    if(lp.GetNumParam()==3){
        if(!lp.IsNum(-3)) lp.Error("nespravny typ parametru 3 (ESurCriterion)");
        criterion2 = (ESurCriterion) lp.GetNum(-3);
    }
    
    LinkedList* res1 = this_->getResources(type);
    
    LinkedList* res2 = this_->getResources(res1, criterion1);
    LinkedList* res3 = this_->getResources(res2, criterion2);

    //@todo vrati prvni nalezeny (muze jich byt vice)
    res3->start();
    lp.PushPointer(res3->next());
    return 1;
}