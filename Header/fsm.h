/*************************************************************************/
/*  File Name: fsm.h                                                     */
/*  Purpose: Header file for FSM module. Module designed to be integrated*/
/*           with any RTOS or used bare metal, only handlers and queue   */
/*           functions need to be defined in fsm.h.                      */
/*  Created by: Garrett Sculthorpe on 10/20/20.                          */
/*  Copyright © 2020 Garrett Sculthorpe.                                 */
/*              All rights reserved.                                     */
/*************************************************************************/

#ifndef fsm_h
#define fsm_h


/*************************************************************************/
/*  Definitions                                                          */
/*************************************************************************/
/* Replace function on right with API to get single event from an event queue,
   or return FSM_EVT_NULL if empty */
#define evt_fsm_evtQueueGet()            evt_fsm_evtQueueGetStub()

/* Replace function on right with API to put single event into an event queue,
   with an argument that can take FSM_EVT */
#define u1_fsm_evtQueuePut(evt)          u1_fsm_evtQueuePutStub(evt)

/* Replace function on left with API for mutual exclusion, lock()   */
#define fsm_enterCritical()              (0)

/* Replace function on left with API for mutual exclusion, unlock() */
#define fsm_exitCritical()               (0)



#define FSM_TRUE                         1
#define FSM_FALSE                        0

#define FSM_EVT_REGISTERED               0
#define FSM_EVT_NOT_REGISTERED           1

#define FSM_STATE_REENTRY                1
#define FSM_STATE_FIRST_ENTRY            0

#define FSM_STATE_SET_ENTRY_FUNC         0
#define FSM_STATE_SET_EXIT_FUNC          1

#define FSM_STATE_SET_FUNC_SUCCESS       0
#define FSM_STATE_SET_FUNC_INV_STATE     1
#define FSM_STATE_SET_FUNC_INV_SET       2

#define FSM_EVT_QUEUE_ALL_EVT_PROCESSED  0
#define FSM_EVT_QUEUE_INVALID_EVT        1

/*************************************************************************/
/*  Public Data Types                                                    */
/*************************************************************************/
#ifndef U1
typedef unsigned char U1;
#endif
#ifndef U4
typedef int U4;
#endif

typedef enum FSM_EVT
{
  FSM_EVT_0 = 0,
  FSM_EVT_1,

  FSM_NUM_EVENTS,
  FSM_EVT_INACTIVE,
  FSM_EVT_NULL
}
FSM_EVT;

typedef enum FSM_STATE
{
  FSM_STATE_0 = 0,
  FSM_STATE_1,

  FSM_NUM_STATES
}
FSM_STATE;

/*************************************************************************/
/*  Public Functions                                                     */
/*************************************************************************/
/*************************************************************************/
/*  Function Name: vd_fsm_init                                           */
/*  Purpose:       Initialize module variables.                          */
/*  Arguments:     N/A                                                   */
/*  Return:        N/A                                                   */
/*************************************************************************/
void vd_fsm_init(void);

/*************************************************************************/
/*  Function Name: u1_fsm_setStateFp                                     */
/*  Purpose:       Set function pointers for state entry/exit.           */
/*  Arguments:     U4 state:                                             */
/*                    State for which functions will be defined.         */
/*                 U1 entryOrExit:                                       */
/*                    FSM_STATE_SET_ENTRY_FUNC   OR                      */
/*                    FSM_STATE_SET_EXIT_FUNC                            */
/*                 void (*fp):                                           */
/*                    Pointer to routine. Entry() has U1 argument.       */
/*  Return:        FSM_STATE_SET_FUNC_INV_SET       OR                   */
/*                 FSM_STATE_SET_FUNC_INV_STATE     OR                   */
/*                 FSM_STATE_SET_FUNC_SUCCESS                            */
/*************************************************************************/
U1 u1_fsm_setStateFp(U4 state, U1 entryOrExit, void (*fp));

/*************************************************************************/
/*  Function Name: state_fsm_getCurrentState                             */
/*  Purpose:       Return current state of FSM.                          */
/*  Arguments:     N/A                                                   */
/*  Return:        enum FSM_STATE                                        */
/*************************************************************************/
FSM_STATE state_fsm_getCurrentState(void);

/*************************************************************************/
/*  Function Name: u1_fsm_registerEvt                                    */
/*  Purpose:       Register an event. Wraps event queue API.             */
/*  Arguments:     FSM_EVT evt:                                          */
/*                    Event number defined in enum FSM_EVT.              */
/*  Return:        Return value from wrapped queue API.                  */
/*************************************************************************/
U1 u1_fsm_registerEvt(FSM_EVT evt);

/*************************************************************************/
/*  Function Name: u1_fsm_serveEvtQueue                                  */
/*  Purpose:       Serve waiting events in event queue. Will run until   */
/*                 error or empty. Calls vd_fsm_invalidEvtHandler() hook */
/*                 if event is invalid at current state.                 */
/*  Arguments:     N/A                                                   */
/*  Return:        U1 FSM_EVT_QUEUE_INVALID_EVT         OR               */
/*                    FSM_EVT_QUEUE_ALL_EVT_PROCESSED                    */
/*************************************************************************/
U1 u1_fsm_serveEvtQueue(void);

#endif
