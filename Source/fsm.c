/*************************************************************************/
/*  File Name: fsm.c                                                     */
/*  Purpose: Source file for FSM module. Module designed to be integrated*/
/*           with any RTOS or used bare metal, only handlers and queue   */
/*           functions need to be defined in fsm.h.                      */
/*  Created by: Garrett Sculthorpe on 10/20/20.                          */
/*  Copyright © 2020 Garrett Sculthorpe.                                 */
/*              All rights reserved.                                     */
/*************************************************************************/

/*************************************************************************/
/*  Includes                                                             */
/*************************************************************************/
#include "fsm.h"
#include "fsm_stub.h"

/*************************************************************************/
/*  Definitions                                                          */
/*************************************************************************/
#define FSM_MARKED_STATE   1
#define FSM_UNMARKED_STATE 0
#define FSM_NULL           0

/*************************************************************************/
/*  Private Data Types                                                   */
/*************************************************************************/
typedef struct FSMStateRoutines
{
  void (*entryFunction)(U1 reentrySts);
  void (*exitFunction)(void);
}
FSMStateRoutines;

typedef struct FSMState
{
  U1                markedState;
  FSMStateRoutines* stateFunctions;
  U4                eventList[FSM_NUM_EVENTS];
}
FSMState;

/*************************************************************************/
/*  Static Global Variables, Constants                                   */
/*************************************************************************/
static FSM_STATE state_s_currentState;
static FSM_STATE state_s_prevState;

static FSMStateRoutines fsm_sas_fp[FSM_NUM_STATES] = {FSM_NULL};


/*************************************************************************/
/*************************************************************************/
/* Auto-generated state table, do not modify                             */
/*************************************************************************/
/*************************************************************************/
const FSMState fsm_sas_fsmTable[FSM_NUM_STATES] =
{   /* Marked status   Pointer to entry/exit APIs  Transition: 0          1
  {
    FSM_MARKED_STATE, &fsm_sas_fp[FSM_STATE_0],   {FSM_EVT_INACTIVE, FSM_STATE_1}
  },
  {
    FSM_UNMARKED_STATE, &fsm_sas_fp[FSM_STATE_1], {FSM_STATE_0,      FSM_EVT_INACTIVE}
  }
};
/*************************************************************************/
/*************************************************************************/


/*************************************************************************/
/*  Private Function Prototypes                                          */
/*************************************************************************/


/*************************************************************************/

/*************************************************************************/
/*  Function Name: vd_fsm_init                                           */
/*  Purpose:       Initialize module variables.                          */
/*  Arguments:     N/A                                                   */
/*  Return:        N/A                                                   */
/*************************************************************************/
void vd_fsm_init(void)
{
  state_s_currentState      = FSM_STATE_0;
  state_s_prevState = FSM_EVT_NULL;
}

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
/*  Return:        U1 FSM_STATE_SET_FUNC_INV_SET       OR                */
/*                    FSM_STATE_SET_FUNC_INV_STATE     OR                */
/*                    FSM_STATE_SET_FUNC_SUCCESS                         */
/*************************************************************************/
U1 u1_fsm_setStateFp(U4 state, U1 entryOrExit, void (*fp))
{
  U1 u1_t_rtnSts;

  /* Check valid state. */
  if(state < FSM_NUM_STATES)
  {
    u1_t_rtnSts = FSM_STATE_SET_FUNC_SUCCESS;

    fsm_enterCritical();

    /* Entry or exit callback? */
    if(entryOrExit == FSM_STATE_SET_ENTRY_FUNC)
    {
      fsm_sas_fsmTable[state].stateFunctions->entryFunction = fp;
    }
    else if(entryOrExit == FSM_STATE_SET_EXIT_FUNC)
    {
      fsm_sas_fsmTable[state].stateFunctions->exitFunction = fp;
    }
    /* Invalid param. */
    else
    {
      u1_t_rtnSts = FSM_STATE_SET_FUNC_INV_SET;
    }

    fsm_exitCritical();
  }
  else
  {
    u1_t_rtnSts = FSM_STATE_SET_FUNC_INV_STATE;
  }

  return (u1_t_rtnSts);
}

/*************************************************************************/
/*  Function Name: state_fsm_getCurrentState                             */
/*  Purpose:       Return current state of FSM.                          */
/*  Arguments:     N/A                                                   */
/*  Return:        enum FSM_STATE                                        */
/*************************************************************************/
FSM_STATE state_fsm_getCurrentState(void)
{
  FSM_STATE state_t_currentState;

  fsm_enterCritical();

  state_t_currentState = state_s_currentState;

  fsm_exitCritical();

  return state_t_currentState;
}

/*************************************************************************/
/*  Function Name: u1_fsm_registerEvt                                    */
/*  Purpose:       Register an event. Wraps event queue API.             */
/*  Arguments:     FSM_EVT evt:                                          */
/*                    Event number defined in enum FSM_EVT.              */
/*  Return:        Return value from wrapped queue API.                  */
/*************************************************************************/
U1 u1_fsm_registerEvt(FSM_EVT evt)
{
  return u1_fsm_evtQueuePut(evt);
}

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
{
  FSM_EVT evt_s_nextEvent;
  U1 u1_t_reentrySts;
  U1 u1_t_rtnSts;
  U1 u1_t_evtProcessFlag;

  evt_s_nextEvent = FSM_EVT_NULL;
  u1_t_evtProcessFlag = FSM_TRUE;

  while(u1_t_evtProcessFlag == FSM_TRUE)
  {
    fsm_enterCritical();

    evt_s_nextEvent = evt_fsm_evtQueueGet();

    /* Check if empty or event returned. */
    if(evt_s_nextEvent == FSM_EVT_NULL)
    {
      u1_t_evtProcessFlag = FSM_FALSE;
      u1_t_rtnSts         = FSM_EVT_QUEUE_ALL_EVT_PROCESSED;
    }
    /* Check valid transition */
    else if(fsm_sas_fsmTable[state_s_currentState].eventList[evt_s_nextEvent] == FSM_EVT_INACTIVE)
    {
      /* Invalid transition */
      u1_t_rtnSts = FSM_EVT_QUEUE_INVALID_EVT;
#ifdef u1_fsm_invalidEvtHandler
      u1_t_evtProcessFlag = u1_fsm_invalidEvtHandler();
#endif
    }
    else
    {
      state_s_prevState = state_s_currentState;

      /* Call state exit function if defined. */
      if(fsm_sas_fsmTable[state_s_currentState].stateFunctions->exitFunction != (void*)FSM_NULL)
      {
        fsm_sas_fsmTable[state_s_currentState].stateFunctions->exitFunction();
      }
      else
      {

      }

      state_s_currentState = fsm_sas_fsmTable[state_s_currentState].eventList[evt_s_nextEvent];

      /* Is the transition a self-loop?*/
      u1_t_reentrySts = (state_s_prevState == state_s_currentState) ?
                        FSM_STATE_REENTRY : FSM_STATE_FIRST_ENTRY;

      /* Call state entry function if defined. */
      if(fsm_sas_fsmTable[state_s_currentState].stateFunctions->entryFunction != (void*)FSM_NULL)
      {
        fsm_sas_fsmTable[state_s_currentState].stateFunctions->entryFunction(u1_t_reentrySts);
      }
      else
      {

      }

      /* Call marked state handler if state is marked. */
      if(fsm_sas_fsmTable[state_s_currentState].markedState == FSM_MARKED_STATE)
      {
#ifdef fsm_markedStateHandler
        fsm_markedStateHandler(state_s_currentState);
#endif
      }
      else
      {

      }
    }

    fsm_exitCritical();
  }

  return (u1_t_rtnSts);
}

/*************************************************************************/
/* History                                                               */
/*************************************************************************/
/* 10/20/2020      First implementation.                                 */
/*                                                                       */
