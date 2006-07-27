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




CRR::CRR(int resCount){
    
    this->resCount = resCount;
    resLists = new LinkedList[resCount];
}

CRR::~CRR(){
    
    SAFE_DELETE_ARRAY(resLists);
}

/**
 * Prida resource item do registru. Pokud je to novy typ, vytvori dalsi "slot".
 */
void CRR::addResource(ResourceItem* item){

    for(int n=0; n<resCount; n++){
        if(resLists[n].getType() == item.getType()){
            //typ existuje
        }
    }
    if(n==resCount){
        //typ neexistuje, vytvori se dalsi "slot"
    }
}

void CRR::getResource(){


}