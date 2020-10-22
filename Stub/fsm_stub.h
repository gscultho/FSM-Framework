/*************************************************************************/
/*  File Name: fsm_stub.h                                                */
/*  Purpose: Basic queue implementation. These files can be removed from */
/*           project when integrating a different event queue.           */
/*************************************************************************/

#ifndef fsm_stub_h
#define fsm_stub_h

#include <stdio.h>
#include <stdlib.h>

/*************************************************************************/
/*  Definitions                                                          */
/*************************************************************************/
#define FSM_EVT_QUEUE_LENGTH         8

/*************************************************************************/
/*  Public Data Types                                                    */
/*************************************************************************/


/*************************************************************************/
/*  Public Functions                                                     */
/*************************************************************************/
U1 u1_fsm_evtQueuePutStub(FSM_EVT evt);
FSM_EVT evt_fsm_evtQueueGetStub(void);

#endif
