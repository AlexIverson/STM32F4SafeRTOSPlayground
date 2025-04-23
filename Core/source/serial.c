/*
    Copyright (C)2006 onward WITTENSTEIN high integrity systems Limited. All rights reserved.

    This file is part of the SafeRTOS product, see projdefs.h for version number
    information.

    SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
    and is subject to the terms of the License granted to your organization,
    including its warranties and limitations on use and distribution. It cannot be
    copied or reproduced in any way except as permitted by the License.

    Licenses authorise use by processor, compiler, business unit, and product.

    WITTENSTEIN high integrity systems Limited, Registered Office: Brown's Court,
    Long Ashton Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
    Tel: +44 1275 395 600
    Email: info@highintegritysystems.com
    www.highintegritysystems.com
*/

/* Scheduler include files. */
#define COMTEST_C

#include "SafeRTOS_API.h"
#include "PortSpecifics.h"
#include "serial.h"
#include "stm32f4xx_hal.h"

/* Serial interrupt priority.
 * This is set to the maximum interrupt priority level that SafeRTOS API calls
 * are permitted to use. */
#define serINTERRUPT_PRIORITY   ( configSYSTEM_INTERRUPT_PRIORITY >> 4U )

/* Some driver specifics. */
#define serNO_BLOCK             ( ( portTickType ) 0 )

/* Define the buffers to be used by the queues. */
#define serQUEUE_LENGTH         ( 60U )
#define serRX_QUEUE_BUFFER_LEN  ( serQUEUE_LENGTH + safertosapiQUEUE_OVERHEAD_BYTES )
#define serTX_QUEUE_BUFFER_LEN  ( serQUEUE_LENGTH + safertosapiQUEUE_OVERHEAD_BYTES )

/* Baud Rate. */
#define serBAUD_RATE            ( 115200U )

/*---------------------------------------------------------------------------*/

static portInt8Type acRxQueueBuffer[ serRX_QUEUE_BUFFER_LEN ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };
static portInt8Type acTxQueueBuffer[ serTX_QUEUE_BUFFER_LEN ] __attribute__( ( aligned ( 4 ) ) ) = { 0 };

/* Queues used to hold received characters, and characters waiting to be
 * transmitted. */
portspecCOM_TEST_ZERO_DATA_SECTION static xQueueHandle xRxedChars = NULL;
portspecCOM_TEST_ZERO_DATA_SECTION static xQueueHandle xCharsForTx = NULL;
portspecCOM_TEST_ZERO_DATA_SECTION static volatile portBaseType xCharsWaitingForTx = pdFALSE;

portspecCOM_TEST_ZERO_DATA_SECTION static UART_HandleTypeDef xUartHandle = { 0 };

/*---------------------------------------------------------------------------*/

void vSerialPortInit( void )
{
    portBaseType xRxQueueStatus, xTxQueueStatus;
    GPIO_InitTypeDef xGPIOInitStruct = { 0 };

    /* Create the queues used to hold Rx and Tx characters. */
    xRxQueueStatus = xQueueCreate( acRxQueueBuffer, serRX_QUEUE_BUFFER_LEN, serQUEUE_LENGTH, ( portUnsignedBaseType ) sizeof( portCharType ), &xRxedChars );
    xTxQueueStatus = xQueueCreate( acTxQueueBuffer, serTX_QUEUE_BUFFER_LEN, serQUEUE_LENGTH, ( portUnsignedBaseType ) sizeof( portCharType ), &xCharsForTx );

    if( ( pdPASS == xRxQueueStatus ) && ( pdPASS == xTxQueueStatus ) )
    {
        safertosapiENTER_CRITICAL();
        {
            /* Enable GPIO clock */
            __GPIOA_CLK_ENABLE();

            /* Enable UART clock */
            __USART1_CLK_ENABLE();

            /* Connect PXx to USARTx_Tx */
            /* Connect PXx to USARTx_Rx */
            /* Configure USART Tx as alternate function */
            /* Configure USART Rx as alternate function */
            xGPIOInitStruct.Mode = GPIO_MODE_AF_PP;
            xGPIOInitStruct.Pull = GPIO_PULLUP;
            xGPIOInitStruct.Speed = GPIO_SPEED_FAST;
            xGPIOInitStruct.Alternate = GPIO_AF7_USART1;
            xGPIOInitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;

            HAL_GPIO_Init( GPIOA, &xGPIOInitStruct );

            xUartHandle.Instance = USART1;
            xUartHandle.Init.BaudRate = serBAUD_RATE;
            xUartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
            xUartHandle.Init.Mode = UART_MODE_TX_RX;
            xUartHandle.Init.Parity = UART_PARITY_NONE;
            xUartHandle.Init.StopBits = UART_STOPBITS_1;
            xUartHandle.Init.WordLength = UART_WORDLENGTH_8B;
            xUartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

            ( void ) HAL_UART_Init( &xUartHandle );

            /* Enable the USART Interrupt */
            __HAL_UART_ENABLE_IT( &xUartHandle, UART_IT_RXNE );

            HAL_NVIC_SetPriority( USART1_IRQn, serINTERRUPT_PRIORITY, 0 );
            HAL_NVIC_EnableIRQ( USART1_IRQn );
        }
        safertosapiEXIT_CRITICAL();
    }
}
/*---------------------------------------------------------------------------*/

portBaseType xSerialGetChar( portCharType *pcRxedChar, portTickType xBlockTime )
{
    portBaseType xResult;

    /* Get the next character from the buffer.  Return false if no characters
     * are available, or arrive before xBlockTime expires. */
    if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) == pdPASS )
    {
        xResult = pdTRUE;
    }
    else
    {
        xResult = pdFALSE;
    }

    return xResult;
}
/*---------------------------------------------------------------------------*/

portBaseType xSerialPutChar( portCharType cOutChar, portTickType xBlockTime )
{
    portBaseType xReturn = pdPASS;

    /* Place the character in the queue of characters to be transmitted. */
    safertosapiENTER_CRITICAL();
    {
        if( xCharsWaitingForTx == pdFALSE )
        {
            xCharsWaitingForTx = pdTRUE;
            xUartHandle.Instance->DR = cOutChar;
        }
        else
        {
            if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) != pdPASS )
            {
                xReturn = pdFAIL;
            }
            else
            {
                /*
                 * Depending on queue sizing and task prioritisation:  While we
                 * were blocked waiting to post, interrupts were not disabled.
                 * It is possible that the serial ISR has emptied the Tx queue,
                 * in which case we need to start the Tx off again.
                 */
                if( pdFALSE == xCharsWaitingForTx )
                {
                    ( void ) xQueueReceive( xCharsForTx, &cOutChar, serNO_BLOCK );
                    xCharsWaitingForTx = pdTRUE;
                    xUartHandle.Instance->DR = cOutChar;
                }
            }
        }
    }
    safertosapiEXIT_CRITICAL();

    return xReturn;
}
/*---------------------------------------------------------------------------*/

void USART1_IRQHandler( void )
{
    portUInt8Type cChar;

    if( __HAL_UART_GET_FLAG( &xUartHandle, UART_FLAG_RXNE ) != RESET )
    {
        /* Clear the USART3 Receive interrupt */
        __HAL_UART_CLEAR_FLAG( &xUartHandle, UART_FLAG_RXNE );
        cChar = xUartHandle.Instance->DR;

        ( void ) xQueueSendFromISR( xRxedChars, &cChar );

        /* Are there any more characters to transmit? */
        if( xQueueReceiveFromISR( xCharsForTx, &cChar ) == pdPASS )
        {
            /* A character was retrieved from the queue so can be sent now. */
            xUartHandle.Instance->DR = cChar;
        }
        else
        {
            xCharsWaitingForTx = pdFALSE;
        }
    }

    safertosapiYIELD_FROM_ISR();
}
/*---------------------------------------------------------------------------*/
