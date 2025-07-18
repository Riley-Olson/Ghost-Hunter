#include "defs.h"

/*  Function:  initNotebook
    Purpose:   Initialize the notebook linked list.
        in:   a pointer to the notebook.
*/
void initNotebook(NotebookType *notebook){

    notebook->head = NULL;
    notebook->tail = NULL;

}
