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