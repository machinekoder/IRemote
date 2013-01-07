/**
 * @file timeout.h
 * @author Alexander Rössler
 * @brief Timeout Library
 * @date 05-01-2013
 */
#pragma once

#include <types.h>
#include <timer.h>

/** Initializes a free timer to work as counter for timeouts.
 *  @return 0 if successful, -1 on error
 */
int8 initializeTimeout(int8 timerId);
/** Returns current timeout value in msecs
 *  @return timeout value in ms
 */
uint32 timeoutMsecs(void);
/** Resets the timeout timer. */
void resetTimeout(void);
