#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

typedef struct {
    char name[MAX_STR];
    struct HunterType* hunters[MAX_HUNTERS];
    struct ConnectorList* connectedRooms;
    struct NotebookType *ghostEvidence;
    struct GhostType *ghost;
    int numberOfConnections;
} RoomType;

typedef struct RoomNode {
    RoomType* room;
    struct RoomNode *next;
} RoomNodeType;

typedef struct ConnectorNode {
    RoomType* room;
    struct ConnectorNode *next;
} ConnectorNodeType;

typedef struct ConnectorList {
    ConnectorNodeType *head;
    ConnectorNodeType *tail;
} ConnectorListType;

typedef struct GhostType{
    GhostClassType ghostType;
    RoomType *room;
    int boredomTimer;
    EvidenceClassType possibleEvidence[3];
    int quit;
} GhostType;

typedef struct RoomListType {
    RoomNodeType *head;
    RoomNodeType *tail;
} RoomListType;

typedef struct Building {
    GhostType* ghost;
    struct HunterType* hunters[MAX_HUNTERS];
    RoomListType* rooms;
    int numberOfHunters;
} BuildingType;

typedef struct EvidenceType{
    EvidenceClassType evidence;
    float value;
    struct EvidenceType *next;
    struct EvidenceType *prev;
} EvidenceType;

typedef struct NotebookType{
    EvidenceType *head;
    EvidenceType *tail;
} NotebookType;

typedef struct HunterType{
    char name[MAX_STR];
    RoomType *room;
    EvidenceClassType evidenceType;
    int fear;
    int timer;
    NotebookType journal;
    int checkEmf;
    int checkTemp;
    int checkFing;
    int checkSound;
    int hunterID;
    int numOfGhostEv;
    int quit;
    BuildingType *building;
} HunterType;

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

void populateRooms(BuildingType*);   // Populates the building with sample data for rooms

void initRoom(RoomType*, char*);

void initRoomList(RoomListType*);

void initConnectorList(ConnectorListType*);

void initHunter(HunterType**, RoomListType*, char*, EvidenceClassType, BuildingType*, int);

void initGhost(BuildingType*, GhostType**);

void initBuilding(BuildingType*);

void initNotebook(NotebookType*);

void connectRooms(RoomNodeType*, RoomNodeType*);

void appendRoom(RoomListType*, RoomNodeType*);

void leaveEvidence(GhostType*);

void collectEvidence(HunterType*, NotebookType*);

void* hunt(void*);

void* haunt(void*);

void moveHunter(HunterType*);

void moveGhost(GhostType*);

void shareEvidence(HunterType*);

void cleanup(BuildingType*);
