/* Interrupts.hpp
*
* Copyright (C) 2020 Jan Rogall
*
* This software may be modified and distributed under the terms
* of the MIT license.  See the LICENSE file for details.
*/

#ifndef MICROPIN_INTERRUPTS_INCLUDED
#define MICROPIN_INTERRUPTS_INCLUDED

#include "RegisterDefinitions.hpp"
#include <avr/interrupt.h>

namespace MicroPin
{

// Disables interrupts as long as in scope
class NoInterrupts
{
public:
    __attribute__((always_inline)) NoInterrupts() : oldSREG(rSREG) {
        cli();
    }
    __attribute__((always_inline)) ~NoInterrupts() {
        rSREG = oldSREG;
    }
    NoInterrupts(NoInterrupts&&) = delete;
    NoInterrupts& operator=(NoInterrupts&&) = delete;
private:
    uint8_t oldSREG;
};


}



#endif
