#include "defs.h"

/*  Function:  initGhost
    Purpose:   Initialize a ghost.
        in:   a pointer to the building the ghost will be in.
        in:   a double pointer to a GhostType declared in main.
*/
void initGhost(BuildingType *building, GhostType **ghost){

    GhostType *newGhost = (GhostType *) calloc(1, sizeof(GhostType));
    newGhost->boredomTimer = BOREDOM_MAX;
    newGhost->ghostType = randInt(0,4);
    newGhost->quit = 0;

    RoomNodeType* iteratorNode;
    iteratorNode = building->rooms->head;

    int moveIndex = randInt(1,13);

    for (int i = 0; i <= moveIndex; i++){
        if (i == moveIndex){
            break;
        }
        iteratorNode = iteratorNode->next;
    }

    if (newGhost->ghostType == 0){
        newGhost->possibleEvidence[0] = 0;
        newGhost->possibleEvidence[1] = 1;
        newGhost->possibleEvidence[2] = 2;
    }
    else if (newGhost->ghostType == 1){
        newGhost->possibleEvidence[0] = 0;
        newGhost->possibleEvidence[1] = 1;
        newGhost->possibleEvidence[2] = 3;
    }
    else if (newGhost->ghostType == 2){
        newGhost->possibleEvidence[0] = 0;
        newGhost->possibleEvidence[1] = 2;
        newGhost->possibleEvidence[2] = 3;
    }
    else if (newGhost->ghostType == 3){
        newGhost->possibleEvidence[0] = 1;
        newGhost->possibleEvidence[1] = 2;
        newGhost->possibleEvidence[2] = 3;
    }

    newGhost->room = iteratorNode->room;

    *ghost = newGhost;

    building->ghost = *ghost;
}

/*  Function:  haunt
    Purpose:   Choose the actions for the ghost to take.
        in:   a pointer to the ghost.
*/
void *haunt(void *ghost1){
    GhostType* ghost = (GhostType*)ghost1;
    while (ghost->boredomTimer != 0 && ghost->quit != 1){

        int action = randInt(0,3);

        if (action == 0){
            leaveEvidence(ghost);
        }
        else if (action == 1){
            moveGhost(ghost);
        }
        else if (action == 2){
          // do nothing
        }

        usleep(USLEEP_TIME);
    }

    if (ghost->boredomTimer == 0){
        printf("The ghost got bored and left.\n");
    }
}

/*  Function:  leaveEvidence
    Purpose:   Leave ghostly evidence in the ghosts current room.
        in:   a pointer to the ghost.
*/
void leaveEvidence(GhostType* ghost){
    int randomEv = randInt(0,3);
    EvidenceType *ghostEvidence = (EvidenceType *) calloc(1, sizeof(EvidenceType));
    ghostEvidence->evidence = ghost->possibleEvidence[randomEv];

    if (ghost->possibleEvidence[randomEv] == 0){
        ghostEvidence->value = randFloat(4.7,5);
    }
    else if (ghost->possibleEvidence[randomEv] == 1){
        ghostEvidence->value = randFloat(-10,1);
    }
    else if (ghost->possibleEvidence[randomEv] == 2){
        ghostEvidence->value = 1;
    }
    else if (ghost->possibleEvidence[randomEv] == 3){
        ghostEvidence->value = randFloat(65,75);
    }

    if (ghost->room->ghostEvidence->head == NULL){
        ghost->room->ghostEvidence->head = ghostEvidence;
        ghost->room->ghostEvidence->tail = ghostEvidence;
    }
    else{
        ghostEvidence->prev = ghost->room->ghostEvidence->tail;
        ghost->room->ghostEvidence->tail->next = ghostEvidence;
        ghost->room->ghostEvidence->tail = ghostEvidence;
    }
}

/*  Function:  moveGhost
    Purpose:   Move ghost to an adjacent room.
        in:   a pointer to the ghost.
*/
void moveGhost(GhostType *ghost){
    ghost->boredomTimer -= 1;
    int moveIndex = randInt(0,ghost->room->numberOfConnections);

    ConnectorNodeType * iteratorNode;
    iteratorNode = ghost->room->connectedRooms->head;
    for (int i = 0; i <= moveIndex; i++){
        if (i == moveIndex){
            ghost->room->ghost = NULL;
            ghost->room = iteratorNode->room;
            ghost->room->ghost = ghost;
            printf("The ghost moved to: %s...\n", ghost->room->name);
            break;
        }
        iteratorNode = iteratorNode->next;
    }
}
