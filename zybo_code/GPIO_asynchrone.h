/***************************************************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************************************************
*But de ce code: Le but est d'allumer une LED à l'aide d'un switch (pourrait fonctionner avec un bouton). Le process est fait à l'aide d'une interruption. Le microP ne scrute le switch que l'orsqu'il change
*d'état. 

***************************************************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************************************************



******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "Xscugic.h"
#include "xparameters.h"
#include "xparameters_ps.h"
//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------

static int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static void InterruptGPIOHandler(void);

//----------------------------------------------------

#define SWS_ID                 XPAR_SWITCH_PORT_DEVICE_ID
#define LEDS_ID					XPAR_LEDS_PORT_DEVICE_ID
#define GIC_ID					XPAR_PS7_SCUGIC_0_DEVICE_ID

#define REG_INTR_SWs			XPAR_FABRIC_SWITCH_PORT_IP2INTC_IRPT_INTR  // registre d'interruption pour les sws
#define SWS_INT					XGPIO_IR_CH1_MASK
#define BTN_INT					XGPIO_IR_CH2_MASK
#define SWS_BTN_INT				XGPIO_IR_MASK

XGpio MesSWs_driver;
XGpio MesLEDs_driver;

XScuGic MonGIC_driver;
XScuGic_Config* MonGIC_driver_config;


int main()
{

	init_platform();

	XGpio_Initialize(&MesSWs_driver,SWS_ID);
	XGpio_Initialize(&MesLEDs_driver,LEDS_ID);
    	XGpio_SetDataDirection(&MesSWs_driver, 1,0xFFFFFFFF);
    	XGpio_SetDataDirection(&MesLEDs_driver, 1,0xFFFFFFF0);

	MonGIC_driver_config= XScuGic_LookupConfig(GIC_ID);
	XScuGic_CfgInitialize(&MonGIC_driver,MonGIC_driver_config,MonGIC_driver_config -> CpuBaseAddress);

	XGpio_InterruptGlobalEnable(&MesSWs_driver);
	XGpio_InterruptEnable(&MesSWs_driver, SWS_INT);

	InterruptSystemSetup(&MonGIC_driver);

	XScuGic_Connect(&MonGIC_driver,REG_INTR_SWs,(Xil_ExceptionHandler)InterruptGPIOHandler,&MesSWs_driver);
	XScuGic_Enable(&MonGIC_driver,REG_INTR_SWs);






    while(1)
    {

    }

    print("Hello World\n\r");
    print("Successfully ran Hello World application");
    cleanup_platform();
    return 0;
}





int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
{
	// Register GIC interrupt handler

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			 	 	 	 	 	 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
			 	 	 	 	 	 XScuGicInstancePtr);
	Xil_ExceptionEnable();


	return XST_SUCCESS;

}



void InterruptGPIOHandler(void)
{

	u32 LEDs;

	XGpio_InterruptGlobalDisable(&MesBTN_SW_driver);
	/*
	 * Verify that each of the inputs are valid.
	 */

	Xil_AssertVoid(&MesSWs_driver != NULL);



	LEDs=( XGpio_DiscreteRead(&MesBTN_SW_driver,1) )^( XGpio_DiscreteRead(&MesBTN_SW_driver,2) );
	XGpio_DiscreteWrite(&MesLEDs_driver,1,LEDs);

	XGpio_InterruptClear(&MesSWs_driver, SWS_BTN_INT);
	XGpio_InterruptEnable(&MesSWs_driver, SWS_BTN_INT);

}
