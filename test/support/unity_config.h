#ifndef UNITY_SUPPORT_H
#define UNITY_SUPPORT_H

#include "unity.h"

void UnityHelperDeadLoop(void);

//Put our exit breakpoint in a place where it won't get sat on: an unused interrupt vector
#define UNITY_OUTPUT_COMPLETE() UnityHelperDeadLoop()

#endif