/**
 * This is a file 
 */

#include <led.h>
#include <iap.h>
#include "irControl.h"

enum ApplicationState {
    ApplicationStateIdle = 0,
    ApplicationStateCaptureCommand = 1,
    ApplicationStateRunCommand = 2
};

void testFunc();

uint8 testMode = 0;
uint8 applicationState = ApplicationStateIdle;

int main(void)
{   
    uint32 testVar;
    
    initializeLeds();
    clearAllLeds();
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeUart0(115200);                    // Init the UART
    printfUart0("Welcome to IRemote!\n");    // Send a welcome message
    printfUart0("Id: %i, Version: %i, Serial: %i\n",readIdIap(),readVersionIap(),readSerialIap());
    //printfUart0("Sector 26: %i, Sector 27: %i\n",checkBlankIap(26),checkBlankIap(27));
    testVar = 0;
    __disable_irq();
    //eraseIap(27);
    //writeIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    readIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    printfUart0("Test: %u\n",testVar);
    __enable_irq();
    
    initializeIrControl();
    
    setPinMode(2,10,PinModeNoPullUpDown);       // button3
    setGpioDirection(2,10,GpioDirectionInput);
    enableGpioInterrupt(2,10,GpioInterruptRisingEdge,&testFunc);
    
    setGpioDirection(0,9,GpioDirectionOutput);   // Output pin for testing purposes
    
    for (;;) 
    {
        if (applicationState == ApplicationStateIdle)
        {
        }
        else if (applicationState == ApplicationStateCaptureCommand)
        {
            processData();
        }
        else if (applicationState == ApplicationStateRunCommand)
        {
            
        }
        delayMs(100);
     }

    return 0 ;
}



void testFunc()
{
    if (testMode == 0)
    {
        printfUart0("Start capturing data\n");
        applicationState = ApplicationStateCaptureCommand;
        startIrCapture();
        testMode = 1;
    }
    else
    {
        printfUart0("Start running command\n");
        applicationState = ApplicationStateRunCommand;
        //runIrCommand();
        testMode = 0;
    }
}
