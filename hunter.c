#include "defs.h"

/*  Function:  initHunter
    Purpose:   initialize a hunter
       in:   double pointer to hunter declared in main.
       in:   An array list of building rooms
       in:   The type of evidence for the hunter to collect
       in:   the building the hunter will be in
       in:   An id to identify the hunter
*/
void initHunter(HunterType **hunter, RoomListType *map, char *name, EvidenceClassType evidenceCollector, BuildingType *building, int id){

    HunterType *newHunter = (HunterType *) calloc(1, sizeof(HunterType));

    newHunter->room = map->head->room;
    strcpy(newHunter->name,name);
    newHunter->evidenceType = evidenceCollector;
    newHunter->fear = 0;
    newHunter->timer = BOREDOM_MAX;
    newHunter->hunterID = id;
    newHunter->quit = 0;
    newHunter->building = building;

    NotebookType *journal = (NotebookType *) calloc(1, sizeof(NotebookType));
    initNotebook(journal);
    newHunter->journal = *journal;

    *hunter = newHunter;

    building->hunters[building->numberOfHunters] = *hunter;
    building->numberOfHunters += 1;

}

/*  Function:  hunt
    Purpose:   Choose what actions a hunter should perform
        in:   a pointer to the hunter.
*/
void* hunt(void *hunter1){
    HunterType* hunter = (HunterType*)hunter1;
    while (hunter->fear != MAX_FEAR && hunter->timer != 0 && hunter->quit != 1){

        int action = randInt(0,3);

        if (hunter->room->ghost != NULL){
            hunter->fear += FEAR_RATE;
            hunter->room->ghost->boredomTimer = BOREDOM_MAX;
        }

        if (action == 0){
            collectEvidence(hunter, &hunter->journal);
        }
        else if (action == 1){
            moveHunter(hunter);
        }
        else if (action == 2){
            printf("%s is sharing evidence...\n", hunter->name);
            shareEvidence(hunter);
        }

        if (hunter->numOfGhostEv == 3){
            hunter->building->ghost->quit = 1;
            for (int i = 0; i < MAX_HUNTERS; i++){
                hunter->building->hunters[i]->quit = 1;
            }
            break;
        }
        usleep(USLEEP_TIME);
    }

    if (hunter->fear == MAX_FEAR){
        printf("%s got too scared and left.\n", hunter->name);

    }
    else if (hunter->timer == 0){
        printf("%s got bored and left.\n", hunter->name);
    }
}

/*  Function:  collectEvidence
    Purpose:   Collect evidence from the hunters current room
        in:   a pointer to the hunter.
        in:   a pointer to the hunter's journal (evidence collection).
*/
void collectEvidence(HunterType *hunter, NotebookType *journal){
    sem_t mutex;
    sem_init(&mutex, 0, 1);
    sem_wait(&mutex);
    if (hunter->room->ghostEvidence->head == NULL){
        printf("%s found nothing in their search for evidence.\n", hunter->name);
    }
    else{
        EvidenceType* iteratorNode;
        EvidenceType* tempEv;
        iteratorNode = hunter->room->ghostEvidence->head;

        while (iteratorNode != NULL){
            tempEv = iteratorNode->next;
            if (iteratorNode->evidence == hunter->evidenceType){
                hunter->timer = BOREDOM_MAX;
                if (iteratorNode == hunter->room->ghostEvidence->head){
                    hunter->room->ghostEvidence->head = iteratorNode->next;
                    if (iteratorNode->next != NULL){
                        iteratorNode->next->prev = NULL;
                    }
                }
                else if (iteratorNode->prev != NULL){
                    iteratorNode->prev->next = iteratorNode->next;
                    if (iteratorNode->next != NULL){
                        iteratorNode->next->prev = iteratorNode->prev;
                    }
                    else{
                        hunter->room->ghostEvidence->tail = iteratorNode->prev;
                    }
                }

                else if(iteratorNode->next == NULL){
                    iteratorNode->next->prev = NULL;
                }
                iteratorNode->prev = NULL;
                iteratorNode->next = NULL;

                if (hunter->evidenceType == 0 && iteratorNode->value > 4.90 && hunter->checkEmf != 1){
                    hunter->checkEmf = 1;
                    hunter->numOfGhostEv += 1;
                }
                if (hunter->evidenceType == 1 && iteratorNode->value < 0 && hunter->checkTemp != 1){
                    hunter->checkTemp = 1;
                    hunter->numOfGhostEv += 1;
                }
                if (hunter->evidenceType == 2 && iteratorNode->value == 1 && hunter->checkFing != 1){
                    hunter->checkFing = 1;
                    hunter->numOfGhostEv += 1;
                }
                if (hunter->evidenceType == 3 && iteratorNode->value > 70 && hunter->checkSound != 1){
                    hunter->checkSound = 1;
                    hunter->numOfGhostEv += 1;
                }

                if (hunter->journal.head == NULL){
                    printf("%s collected some evidence.\n", hunter->name);
                    hunter->journal.head = iteratorNode;
                    hunter->journal.tail = iteratorNode;
                }
                else{
                    printf("%s collected some evidence.\n", hunter->name);
                    hunter->journal.tail->next = iteratorNode;
                    hunter->journal.tail = iteratorNode;
                }
            }
            iteratorNode = tempEv;
        }
    }
    sem_post(&mutex);
}

/*  Function:  moveHunter
    Purpose:   Move hunter to an adjacent room.
        in:   a pointer to the hunter.
*/
void moveHunter(HunterType *hunter){
    hunter->timer -= 1;
    int moveIndex = randInt(0,hunter->room->numberOfConnections);

    ConnectorNodeType * iteratorNode;
    iteratorNode = hunter->room->connectedRooms->head;
    for (int i = 0; i <= moveIndex; i++){
        if (i == moveIndex){
            printf("%s moved from %s to %s.\n",hunter->name,hunter->room->name,iteratorNode->room->name);
            hunter->room->hunters[hunter->hunterID] = NULL;
            hunter->room = iteratorNode->room;
            hunter->room->hunters[hunter->hunterID] = hunter;
            break;
        }
        iteratorNode = iteratorNode->next;
    }
}

/*  Function:  shareEvidence
    Purpose:   Share important evidence with other hunters in the room
        in:   a pointer to the hunter.
*/
void shareEvidence(HunterType *hunter){
    hunter->timer -= 1;
    for (int i = 0; i < MAX_HUNTERS; i++){
        if (hunter->room->hunters[i] != NULL && hunter->room->hunters[i] != hunter){
            if (hunter->checkEmf == 1 && hunter->room->hunters[i]->checkEmf != 1){
                hunter->room->hunters[i]->checkEmf = 1;
                hunter->room->hunters[i]->numOfGhostEv += 1;
            }
            if (hunter->checkFing == 1 && hunter->room->hunters[i]->checkFing != 1){
                hunter->room->hunters[i]->checkFing = 1;
                hunter->room->hunters[i]->numOfGhostEv += 1;
            }
            if (hunter->checkTemp == 1 && hunter->room->hunters[i]->checkTemp != 1){
                hunter->room->hunters[i]->checkTemp = 1;
                hunter->room->hunters[i]->numOfGhostEv += 1;
            }
            if (hunter->checkSound == 1 && hunter->room->hunters[i]->checkSound != 1){
                hunter->room->hunters[i]->checkSound = 1;
                hunter->room->hunters[i]->numOfGhostEv += 1;
            }
        }
    }
}
