# FSM-Framework
Lightweight FSM framework easily integrated with any RTOS or queue library.

# Features Summary
* Plug APIs from RTOS or library of choice into #define statements in fsm.h in order to define mutual exclusion behavior or the event queue to be used to process events. 
* Configurable:
  * Define state entry/exit functions for any state by passing function pointer to FSM module at startup.
  * Hook functions in place for events such as invalid event detection and "marked" state entry.

# Pending Work
* Writing script to autogenerate state table using discrete event systems tool. State machine can be generated from DES automata after supervisor/diagnoser/etc. design.
* Support for multiple state machines.
