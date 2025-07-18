#include "defs.h"

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));

    // You may change this code; this is for demonstration purposes
    BuildingType building;
    initBuilding(&building);
    populateRooms(&building);

    pthread_t h1, h2, h3, h4, g1;

    GhostType *ghost;

    initGhost(&building, &ghost);

    char hunterName;
    while (building.numberOfHunters < 4){
        printf("Enter name of hunter: ");
        scanf("%s", &hunterName);
        int evidenceType = building.numberOfHunters;
        HunterType *hunter;
        initHunter(&hunter, building.rooms, &hunterName, evidenceType, &building, building.numberOfHunters);
        building.hunters[building.numberOfHunters] = hunter;
    }

    if (building.ghost->ghostType == 0){
        printf("The ghost is POLTERGEIST\n");
    }
    if (building.ghost->ghostType == 1){
        printf("The ghost is BANSHEE\n");
    }
    if (building.ghost->ghostType == 2){
        printf("The ghost is BULLIES\n");
    }
    if (building.ghost->ghostType == 3){
        printf("The ghost is PHANTOM\n");
    }

    pthread_create(&g1, NULL, haunt, building.ghost);
    pthread_create(&h1, NULL, hunt, building.hunters[0]);
    pthread_create(&h2, NULL, hunt, building.hunters[1]);
    pthread_create(&h3, NULL, hunt, building.hunters[2]);
    pthread_create(&h4, NULL, hunt, building.hunters[3]);


    pthread_join(g1, NULL);
    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
    pthread_join(h3, NULL);
    pthread_join(h4, NULL);

    printf("\n\nRESULTS\n\n");
    for (int i = 0; i < MAX_HUNTERS; i++){
        if (building.hunters[i]->numOfGhostEv == 3){
            if (building.hunters[i]->checkEmf == 1 && building.hunters[i]->checkTemp == 1 && building.hunters[i]->checkFing == 1){
                printf("\nThe hunters found the ghost was a Poltergeist!\nHUNTERS WIN!\n");
            }
            if (building.hunters[i]->checkEmf == 1 && building.hunters[i]->checkTemp == 1 && building.hunters[i]->checkSound == 1){
                printf("\nThe hunters found the ghost was a Banshee!\nHUNTERS WIN!\n");
            }
            if (building.hunters[i]->checkEmf == 1 && building.hunters[i]->checkFing == 1 && building.hunters[i]->checkSound == 1){
                printf("\nThe hunters found the ghost was just bullies...\nHUNTERS WIN!\n");
            }
            if (building.hunters[i]->checkTemp == 1 && building.hunters[i]->checkFing == 1 && building.hunters[i]->checkSound == 1){
                printf("\nThe hunters found the ghost was a Phantom!\nHUNTERS WIN!\n");
            }
            break;
        }
        if (building.hunters[i]->fear == MAX_FEAR){
            printf("%10s %s has run away in fear!\n","*",building.hunters[i]->name);
            if (i == MAX_HUNTERS -1 ){
                printf("\nTHE GHOST HAS WON!\n");
            }
        }
        if (building.hunters[i]->timer == 0){
            printf("%10s %s got bored and left.\n","*",building.hunters[i]->name);
            if (i == MAX_HUNTERS -1 ){
                printf("\nTHE GHOST HAS WON!\n");
            }
        }
    }

    cleanup(&building);
    return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number,
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1)
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number,
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1)
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}

void cleanup(BuildingType *building){
    free(building->ghost);
    for (int i = 0; i < MAX_HUNTERS; i++){
        EvidenceType *evNode;
        evNode = building->hunters[i]->journal.head;

        EvidenceType* nextEvNode;
        nextEvNode = NULL;

        while (evNode != NULL){
            nextEvNode = evNode->next;
            free(evNode);
            evNode = nextEvNode;

        }
        free(building->hunters[i]);
    }
//    RoomNodeType* roomNode;
//    roomNode = building->rooms->head;
//
//    RoomNodeType* nextRoomNode;
//    nextRoomNode = NULL;
//
//    while (roomNode != NULL){
//        nextRoomNode = roomNode->next;
//        EvidenceType *evNode;
//        evNode = roomNode->room->ghostEvidence->head;
//
//        EvidenceType* nextEvNode;
//        nextEvNode = NULL;
//
//        while (evNode != NULL){
//            nextEvNode = evNode->next;
//            free(evNode);
//            evNode = nextEvNode;
//
//        }
//        //free(roomNode);
//        roomNode = nextRoomNode;
//
//    }

}
