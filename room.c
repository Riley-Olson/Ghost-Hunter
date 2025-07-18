#include "defs.h"

/*  Function:  initRoomList
    Purpose:   Initialize the rooms linked list.
        in:   a pointer to the linked list.
*/
void initRoomList(RoomListType *roomList){

    roomList->head = NULL;
    roomList->tail = NULL;

}

/*  Function:  initConnectorList
    Purpose:   Initialize the linked list of connected rooms.
        in:   a pointer to the linked list.
*/
void initConnectorList(ConnectorListType *roomList){

    roomList->head = NULL;
    roomList->tail = NULL;

}

/*  Function:  initRoom
    Purpose:   Initialize a room
        in:   a pointer to the room.
        in:   the name of the room.
*/
void initRoom(RoomType *room, char *name){
    strcpy(room->name,name);

    // Create notebook to hold evidence ghost has left in the room.
    NotebookType *ghostEvidence = (NotebookType *) calloc(1, sizeof(NotebookType));
    initNotebook(ghostEvidence);
    room->ghostEvidence = ghostEvidence;

    // Create a linked list of rooms that connect to this room.
    ConnectorListType *roomList = (ConnectorListType *) calloc(1, sizeof(ConnectorListType));
    initConnectorList(roomList);
    room->connectedRooms = roomList;
    room->numberOfConnections = 0;
}

/*  Function:  connectRooms
    Purpose:   connect two rooms together
        in:   one room to connect
        in:   second room to connect
*/
void connectRooms(RoomNodeType *roomOne, RoomNodeType *roomTwo){
    ConnectorNodeType *roomConnect = (ConnectorNodeType *) calloc(1, sizeof(ConnectorNodeType));
    roomConnect->room = roomTwo->room;

    ConnectorNodeType *roomConnectBack = (ConnectorNodeType *) calloc(1, sizeof(ConnectorNodeType));
    roomConnectBack->room = roomOne->room;

    if (roomOne->room->connectedRooms->head == NULL){
        roomOne->room->connectedRooms->head = roomConnect;
        roomOne->room->connectedRooms->tail = roomConnect;
    }
    else{
        roomOne->room->connectedRooms->tail->next = roomConnect;
        roomOne->room->connectedRooms->tail = roomConnect;
    }

    if (roomTwo->room->connectedRooms->head == NULL){
        roomTwo->room->connectedRooms->head = roomConnectBack;
        roomTwo->room->connectedRooms->tail = roomConnectBack;
    }
    else{
        roomTwo->room->connectedRooms->tail->next = roomConnectBack;
        roomTwo->room->connectedRooms->tail = roomConnectBack;
    }


    roomOne->room->numberOfConnections += 1;
    roomTwo->room->numberOfConnections += 1;
}

/*  Function:  appendRoom
    Purpose:   add a room to a linked list of rooms
        in:   linked list of rooms
        in:   room node to add
*/
void appendRoom(RoomListType *roomList, RoomNodeType *roomNode){

    if (roomList->head == NULL){
        roomList->head = roomNode;
        roomList->tail = roomNode;
    }
    else{
        roomList->tail->next = roomNode;
        roomList->tail = roomNode;
    }

}
