#include "gpio.h"

inline void setGpioDirection(uint8 port, uint8 pin, enum GpioDirection direction)
{
    switch (port)
    {
        case 0: GPIO0_CLEAR_DIR(pin);
                GPIO0_SET_DIR(direction,pin);
                return;
        case 1: GPIO1_CLEAR_DIR(pin);
                GPIO1_SET_DIR(direction,pin);
                return;
        case 2: GPIO2_CLEAR_DIR(pin);
                GPIO2_SET_DIR(direction,pin);
                return;
        case 3: GPIO3_CLEAR_DIR(pin);
                GPIO3_SET_DIR(direction,pin);
                return;
        default: return;
    }
    return;
}

inline uint8 readGpio(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: return GPIO0_READ(pin);
        case 1: return GPIO1_READ(pin);
        case 2: return GPIO2_READ(pin);
        case 3: return GPIO3_READ(pin);
        default: return 0;
    }
    return 0;
}

inline void writeGpio(uint8 port, uint8 pin, uint8 value)
{
    switch (port)
    {
        case 0: (value ? (GPIO0_SET(pin)) : (GPIO0_CLEAR(pin)));
                return;
        case 1: (value ? (GPIO1_SET(pin)) : (GPIO1_CLEAR(pin)));
                return;
        case 2: (value ? (GPIO2_SET(pin)) : (GPIO2_CLEAR(pin)));
                return;
        case 3: (value ? (GPIO3_SET(pin)) : (GPIO3_CLEAR(pin)));
                return;
        default: return;
    }
    return;
}

inline void setGpio(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: GPIO0_SET(pin);
                return;
        case 1: GPIO1_SET(pin);
                return;
        case 2: GPIO2_SET(pin);
                return;
        case 3: GPIO3_SET(pin);
                return;
        default: return;
    }
    return;
}

inline void clearGpio(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: GPIO0_CLEAR(pin);
                return;
        case 1: GPIO1_CLEAR(pin);
                return;
        case 2: GPIO2_CLEAR(pin);
                return;
        case 3: GPIO3_CLEAR(pin);
                return;
        default: return;
    }
    return;
}

inline void toggleGpio(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: GPIO0_SET((GPIO0_READ(pin) ? (GPIO0_CLEAR(pin)) : (GPIO0_SET(pin))));
                return;
        case 1: GPIO1_SET((GPIO1_READ(pin) ? (GPIO1_CLEAR(pin)) : (GPIO1_SET(pin))));
                return;
        case 2: GPIO2_SET((GPIO2_READ(pin) ? (GPIO2_CLEAR(pin)) : (GPIO2_SET(pin))));
                return;
        case 3: GPIO3_SET((GPIO3_READ(pin) ? (GPIO3_CLEAR(pin)) : (GPIO3_SET(pin))));
                return;
        default: return;
    }
    return;
}

void enableGpioInterrupt(uint8 port, uint8 pin, enum GpioInterruptType type, void (* func)(void))
{
    switch (port)
    {
        case 0: if ((type == GpioInterruptRisingEdge) || (type == GpioInterruptFallingAndRisingEdge))
                {
                    LPC_GPIOINT->IO0IntEnR |= (1 << pin);
                }
                if ((type == GpioInterruptFallingEdge) || (type == GpioInterruptFallingAndRisingEdge))
                {
                    LPC_GPIOINT->IO0IntEnF |= (1 << pin);
                }
                functionPointers0[pin] = func;
                break;
        case 2: if ((type == GpioInterruptRisingEdge) || (type == GpioInterruptFallingAndRisingEdge))
                {
                    LPC_GPIOINT->IO2IntEnR |= (1 << pin);
                }
                if ((type == GpioInterruptFallingEdge) || (type == GpioInterruptFallingAndRisingEdge))
                {
                    LPC_GPIOINT->IO2IntEnF |= (1 << pin);
                }
                functionPointers2[pin] = func;
                break;
        default: return;
    }
    NVIC_EnableIRQ(EINT3_IRQn);
    return;
}

void disableGpioInterrupt(uint8 port, uint8 pin)
{
    switch (port)
    {
        case 0: LPC_GPIOINT->IO0IntEnR &= ~(1 << pin);
                LPC_GPIOINT->IO0IntEnF &= ~(1 << pin);
                return;
        case 2: LPC_GPIOINT->IO2IntEnR &= ~(1 << pin);
                LPC_GPIOINT->IO2IntEnF &= ~(1 << pin);
                return;
        default: return;
    }
    return;
}

void EINT3_IRQHandler()
{
    uint8 i;
    //toggleLed(1);
    
    if (LPC_GPIOINT->IntStatus & (1 << 0))
    {
        for (i = 0; i < GPIO0_INT_PIN_COUNT; i++)
        {
            if ((LPC_GPIOINT->IO0IntStatR & (1 << i)) || (LPC_GPIOINT->IO0IntStatF & (1 << i)))
            {
                (*functionPointers0[i])();
                LPC_GPIOINT->IO0IntClr |= (1 << i);
            }
        }

    }
    else if (LPC_GPIOINT->IntStatus & (1 << 2))
    {
        for (i = 0; i < GPIO2_INT_PIN_COUNT; i++)
        {
            if ((LPC_GPIOINT->IO2IntStatR & (1 << i)) || (LPC_GPIOINT->IO2IntStatF & (1 << i)))
            {
                (*functionPointers2[i])();
                LPC_GPIOINT->IO2IntClr |= (1 << i);
            }
        }
    }
}
