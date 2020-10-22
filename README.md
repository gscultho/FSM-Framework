# FSM-Framework
Lightweight FSM framework easily integrated with any RTOS or queue library.

## Features Summary
* Plug APIs from RTOS or library of choice into #define statements in fsm.h in order to define mutual exclusion behavior or the event queue to be used to process events. 
* Configurable:
  * Define state entry/exit functions for any state by passing function pointer to FSM module at startup.
  * Hook functions in place for events such as invalid event detection and "marked" state entry.
* Basic queue implementation included in Stub folder to start.

## Configuration
* Instructions for autogeneration of state table to follow upon feature completion.
* Define behavior of FSM module by setting the four #define statements listed below:
```
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
  ```
* Process event queue with call to:
```
/*************************************************************************/
/*  Function Name: u1_fsm_serveEvtQueue                                  */
/*  Purpose:       Serve waiting events in event queue. Will run until   */
/*                 error or empty. Calls vd_fsm_invalidEvtHandler() hook */
/*                 if event is invalid at current state.                 */
/*  Arguments:     N/A                                                   */
/*  Return:        U1 FSM_EVT_QUEUE_INVALID_EVT         OR               */
/*                    FSM_EVT_QUEUE_ALL_EVT_PROCESSED                    */
/*************************************************************************/
U1 u1_fsm_serveEvtQueue(void)
```

## Pending Work
* Writing script to autogenerate state table using discrete event systems tool. State machine can be generated from DES automata after supervisor/diagnoser/etc. design.
* Support for multiple state machines.
