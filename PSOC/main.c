
#include <project.h>
#include "stdio.h"    
#include "functions.h"

uint16 InterruptCnt;

/*******************************************************************************
* Define Interrupt service routine and allocate an vector to the Interrupt
********************************************************************************/
CY_ISR(InterruptHandler)
{
	/* Read Status register in order to clear the sticky Terminal Count (TC) bit 
	 * in the status register. Note that the function is not called, but rather 
	 * the status is read directly.
	 */
   	Timer_1_STATUS;
    
	/* Increment the Counter to indicate the keep track of the number of 
     * interrupts received */
    InterruptCnt++;    
}


int main()
{
    //Enable the global interrupt
    CyGlobalIntEnable;
    
    //For Loop
    int run=0;
    
    //in pulse
    int in=0;

    //UART Buffer
    uint16 count=0;
    uint8 buffer[USBUART_BUFFER_SIZE];
    
    //Axis counters
    uint32 CuentaX;
    uint32 CuentaY;
    
    
    #if (CY_PSOC3 || CY_PSOC5LP)
        
        // Enable the Interrupt component connected to Timer interrupt
        TimerISR_StartEx(InterruptHandler);
        
        // Start USBFS operation with 5-V operation.
        USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    

        //Components initialization
        LCD_Start();
        
        Quad_Y_Start();
        Quad_Y_SetCounter(0);
        
        Quad_X_Start();
        Quad_X_SetCounter(0);
        
        PWM_X_Start();
        PWM_Y_Start();
        
    #endif


    
    //Wait the push button for Activate Loop
     while(!run){
        if (Push_Read()==0){
        run=1;
    }
    }

    while(run)
    {
        //Counters
         CuentaY = Quad_Y_GetCounter();
         float CY = (CuentaY/64)*0.6;
       
         CuentaX = Quad_X_GetCounter();       
         float CX = (CuentaX/64)*0.6;
                   
        
        //UART______________________________________________________________________________________________________
        UART_Send2Host(CX,CY,InterruptCnt,in,buffer,count);
        
        //Test______________________________________________________________________________________________________
        #ifdef Test 
        
        LCD_Position(1u, 0u);
        LCD_PrintInt16(InterruptCnt);
        
        in=pulseDetect(InterruptCnt);
        run=test(InterruptCnt);
        #endif     
    }
    
        
}

/* [] END OF FILE */
