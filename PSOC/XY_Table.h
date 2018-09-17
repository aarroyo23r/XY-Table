/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "stdio.h"  

#define Test

//PWM values
#define PWM_X_Value             200
#define PWM_Y_Value             200

//#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow usage of the floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
//    asm (".global _printf_float");
//#endif

#define USBFS_DEVICE    (0u)

// The buffer size is equal to the maximum packet size of the IN and OUT bulk endpoints.

#define USBUART_BUFFER_SIZE (64u)
#define LINE_STR_LENGTH     (20u)

int pulseWidth=1000;


//Functions___________________________________________________________________________________________________________

void Send( uint16 time,int in,uint16 CX, uint16 CY){                   
    char Msg[50] = {};
    sprintf(Msg,"%hu,%i,%d,%d \r\n",time,in,CX,CY);
    USBUART_PutString(Msg);        
}

void UART_Send2Host ( uint16 CX, uint16 CY,uint8 InterruptCnt,int in,uint8 *buffer ,uint16 count) {
    /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data 
                 * from host. */
                USBUART_CDC_Init();
            }
        }

        /* Service USB CDC when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Check for input data from host. */
            //if (0u != USBUART_DataIsReady())
            if(1)
            {
                /* Read received data and re-enable OUT endpoint. */
                count = USBUART_GetAll(buffer);

                if (0u != count)
                {
                    /* Wait until component is ready to send data to host. */
                    while (0u == USBUART_CDCIsReady())
                    {
                    }
                    
                    /* Send data back to host. */
                    Send(InterruptCnt,in,CX,CY);

                    if (USBUART_BUFFER_SIZE == count)
                    {
                        /* Wait until component is ready to send data to PC. */
                        while (0u == USBUART_CDCIsReady())
                        {
                        }                        
                    }
                }
            }
        }
}


int test(uint16 InterruptCnt ){
    int run=1;
    Timer_1_Start();
        if (InterruptCnt == (4*pulseWidth)){//Test finished
            LCD_Position(0u, 0u);
            LCD_PrintString("Test completed  ");
            PWM_X_WriteCompare(0);
            return run=0;
            
        }
        else{
            if (InterruptCnt == 0){//Start First pulse 
                Timer_1_Start();
                
                LCD_Position(0u, 0u);
                LCD_PrintString("Start Pulse 1");
                PWM_X_WriteCompare(PWM_X_Value);
            } 
            else if (InterruptCnt == pulseWidth){//Finish First pulse 
                LCD_Position(0u, 0u);
                LCD_PrintString("Finish Pulse 1");
                PWM_X_WriteCompare(0);
            }
            else if (InterruptCnt == 2*pulseWidth){//Start second pulse   
                LCD_Position(0u, 0u);
                LCD_PrintString("Start Pulse 2");
                PWM_X_WriteCompare(PWM_X_Value); 
            
            }
            else if (InterruptCnt == 3*pulseWidth){//Finish second pulse 
                LCD_Position(0u, 0u);
                LCD_PrintString("Finish Pulse 2");
                LCD_PrintInt16(InterruptCnt);
                PWM_X_WriteCompare(0); 
            
            }
            
            return run=1;
            
        }    
}

int pulseDetect(uint16 InterruptCnt){
    int in=5;
        if (InterruptCnt == (3*pulseWidth+1)){//Test finished
            return in=0;
            
        }
        else{
            if (InterruptCnt >= 3*pulseWidth){//Start First pulse 
                return in=0;
            } 
            else if (InterruptCnt >= 2*pulseWidth){//Finish First pulse 
                return in=12;
            }
            else if (InterruptCnt >= pulseWidth){//Start second pulse   
                return in=0; 
            
            }
            else if (InterruptCnt >= 0){//Finish second pulse 
                return in=12; 
            
            }
            else{
            return in=0;
            }
            
        }  
}


/* [] END OF FILE */
