/*************************************************************************/
/*  File Name: fsm_stub.c                                                */
/*  Purpose: Basic queue implementation. These files can be removed from */
/*           project when integrating a different event queue.           */
/*************************************************************************/

/*************************************************************************/
/*  Includes                                                             */
/*************************************************************************/
#include "fsm.h"
#include "fsm_stub.h"

/*************************************************************************/
/*  Definitions                                                          */
/*************************************************************************/
#define FSM_QUEUE_ROLLOVER_MASK   (0xFFFFFFFF >> (32 - 3))

/*************************************************************************/
/*  Private Data Types                                                   */
/*************************************************************************/


/*************************************************************************/
/*  Static Global Variables, Constants                                   */
/*************************************************************************/
static U4 u4_s_queuePutIndex = 0;
static U4 u4_s_queueGetIndex = 0;

static FSM_EVT evt_sa_eventQueue[FSM_EVT_QUEUE_LENGTH];

/*************************************************************************/
/*************************************************************************/
U1 u1_fsm_evtQueuePutStub(FSM_EVT evt)
{
  U1 u1_t_rtnSts;
  U4 u4_t_nextIndex;

  u4_t_nextIndex = (u4_s_queuePutIndex + 1) & FSM_QUEUE_ROLLOVER_MASK;

  if(u4_t_nextIndex == u4_s_queueGetIndex)
  {
    u1_t_rtnSts = 1;
  }
  else
  {
    evt_sa_eventQueue[u4_s_queuePutIndex] = evt;
    u4_s_queuePutIndex = u4_t_nextIndex;

    u1_t_rtnSts = 0;
  }

  return (u1_t_rtnSts);
}


FSM_EVT evt_fsm_evtQueueGetStub(void)
{
  U1 u1_t_rtn;

  if(u4_s_queuePutIndex == u4_s_queueGetIndex)
  {
    u1_t_rtn = FSM_EVT_NULL;
  }
  else
  {
    u1_t_rtn = evt_sa_eventQueue[u4_s_queueGetIndex];

    u4_s_queueGetIndex = (u4_s_queueGetIndex + 1) & FSM_QUEUE_ROLLOVER_MASK;
  }

  return (u1_t_rtn);
}
