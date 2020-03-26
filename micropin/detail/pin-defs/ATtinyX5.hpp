/* detail/pin-defs/ATtinyX5.h
 *
 * Copyright (C) 2018 Jan Rogall
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */


#ifndef MICROPIN_DETAIL_PINDEFS_ATTINYX5_INCLUDED
#define MICROPIN_DETAIL_PINDEFS_ATTINYX5_INCLUDED

#include "../../ConstexprProgmem.hpp"

namespace MicroPin
{
namespace detail
{
    struct PinBitmaskTag{};
    struct PinTimerTag{};
    using PinBitmask = ConstexprProgmemArray<PinBitmaskTag, uint8_t, 6,
        0x01,
        0x02,
        0x04,
        0x08,
        0x10,
        0x20
    >;
    using PinTimer = ConstexprProgmemArray<PinTimerTag, uint8_t, 6,
        1,
        2,
        0,
        0,
        4,
        0
    >;
}
}

inline uint8_t MicroPin::detail::GetRuntimePinBitmask(uint8_t pin)
{
    return PinBitmask::RuntimeRead(pin);
}
inline uint8_t MicroPin::detail::GetRuntimePinTimer(uint8_t pin)
{
    return PinTimer::RuntimeRead(pin);
}
constexpr uint8_t MicroPin::detail::GetPinPortN(uint8_t pin)
{
    //Only one port
    return 0;
}
constexpr MicroPin::Register8 MicroPin::detail::GetPortDataDirection(uint8_t port)
{
    return rDDRB;
}
constexpr MicroPin::Register8 MicroPin::detail::GetPortData(uint8_t port)
{
    return rPORTB;
}
constexpr MicroPin::Register8 MicroPin::detail::GetPortInput(uint8_t port)
{
    return rPINB;
}
constexpr MicroPin::Register8 MicroPin::detail::GetPinDataDirection(uint8_t pin)
{
    return GetPortDataDirection(GetPinPortN(pin));
}
constexpr MicroPin::Register8 MicroPin::detail::GetPinData(uint8_t pin)
{
    return GetPortData(GetPinPortN(pin));
}
constexpr MicroPin::Register8 MicroPin::detail::GetPinInput(uint8_t pin)
{
    return GetPortInput(GetPinPortN(pin));
}
inline void MicroPin::detail::ClearPWM(uint8_t timer)
{
    switch(timer)
    {
    case 1:
        rTCCR0A &= ~bCOM0A1;
        break;
    case 2:
        rTCCR0A &= ~bCOM0B1;
        break;
    case 3:
        rTCCR1 &= ~bCOM1A1;
        break;
    case 4:
        rGTCCR &= ~bCOM1B1;
        break;
    }
}
inline void MicroPin::detail::AnalogWrite(uint8_t timerNum, uint8_t value)
{
    switch(timerNum)
    {
    case 1:
        rTCCR0A |= bCOM0A1;
        rOCR0A = value;
        break;
    case 2:
        rTCCR0A |= bCOM0B1;
        rOCR0B = value;
        break;
    case 3:
        rTCCR1 |= bCOM1A1;
        rOCR1A = value;
        break;
    case 4:
        rGTCCR |= bCOM1B1;
        rOCR1B = value;
    }
}
//Does not check input range
constexpr uint8_t MicroPin::detail::GetAnalogPort(uint8_t digitalPin)
{
    return digitalPin == 2 ? 1 : (digitalPin == 3 ? 3: (digitalPin == 4 ? 2 : 0));
}

namespace MicroPin
{
namespace detail
{
    template<uint8_t Num, bool Analog = false, bool Digital = true>
    struct PinTraitsHelper
    {
        static constexpr bool exists = true;
        static constexpr bool hasTimer = PinTimer::Get<Num>() != 0;
        static constexpr bool isAnalog = Analog;
        static constexpr bool hasDigital = Digital;
        static constexpr uint8_t bitmask = PinBitmask::Get<Num>();
        static constexpr uint8_t timer = PinTimer::Get<Num>();
    };
    //Port B
    template<> struct PinTraits<0> : PinTraitsHelper<0>{};
    template<> struct PinTraits<1> : PinTraitsHelper<1>{};
    template<> struct PinTraits<2> : PinTraitsHelper<2, true>{};
    template<> struct PinTraits<3> : PinTraitsHelper<3, true>{};
    template<> struct PinTraits<4> : PinTraitsHelper<4, true>{};
    template<> struct PinTraits<5> : PinTraitsHelper<5, true>{};
}
}
#endif