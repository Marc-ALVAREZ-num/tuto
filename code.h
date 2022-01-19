int InterruptSystemSetup(XScuGic *XScuGicInstancePtr)
	{
	  // Register GIC interrupt handler
	  Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler, XScuGicInstancePtr);
	  Xil_ExceptionEnable();
	  return XST_SUCCESS;
	}

Approche sequentielle :


while(1)
    {
    	
	XIic_Recv(I2C_Baseaddress, 0x64,&data8, 1, XIIC_STOP);

    	data32=data8;
    	XGpio_DiscreteWrite(&monGPIO, CHANNEL_GPIO_LED,data32);
    }
