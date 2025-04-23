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

/******************************************************************************
 * Simple IO routines for the LEDs.
 *****************************************************************************/


/* Scheduler includes. */
#include "SafeRTOS_API.h"

/* ST driver library includes. */
#include "stm32f4xx_hal.h"

/* Demo application includes. */
#include "partest.h"


#if ( USE_NUCLEO_F429ZI == 1 )

    /* There are 3 LEDs available to toggle. */
    #define partestMAX_USER_LED_NUMBER      ( 3 )

    /* Pin mapping for the user LEDs. */
    const portUInt16Type ausLEDPinMapping[ partestMAX_USER_LED_NUMBER ] =
    {
        GPIO_PIN_0,  /* LD1 - Green - pin 0 */
        GPIO_PIN_7,  /* LD2 - Blue  - pin 7 */
        GPIO_PIN_14, /* LD3 - Red   - pin 14 */
    };
    /* Port mapping for the user LEDs. */
    GPIO_TypeDef *const pxLEDPortMapping[ partestMAX_USER_LED_NUMBER ] =
    {
        GPIOB, /* GPIO port B */
        GPIOB, /* GPIO port B */
        GPIOB, /* GPIO port B */
    };

#elif ( USE_DISC_F429ZI == 1 )

    /* There are 2 LEDs available to toggle. */
    #define partestMAX_USER_LED_NUMBER      ( 2 )

    /* Pin mapping for the user LEDs. */
    const portUInt16Type ausLEDPinMapping[ partestMAX_USER_LED_NUMBER ] =
    {
        GPIO_PIN_13,  /* LD1 - Green - pin 13 */
        GPIO_PIN_14,  /* LD2 - Blue  - pin 14 */
    };
    /* Port mapping for the user LEDs. */
    GPIO_TypeDef *const pxLEDPortMapping[ partestMAX_USER_LED_NUMBER ] =
    {
        GPIOG, /* GPIO port G */
        GPIOG, /* GPIO port G */
    };
#else
#error "No board defined!"
#endif
/*---------------------------------------------------------------------------*/

void vParTestInitialise( void )
{
    /* GPIO structure defined in library files. */
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

#if ( USE_NUCLEO_F429ZI == 1 )
    {
        /* Enable the GPIO_LED Clocks. */
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /* Configure the 2 LED output pins - PG13 and PG14 as output push-pull. */
        GPIO_InitStructure.Pin = ( GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14 ); /* PBx */
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStructure );
    }
#elif ( USE_DISC_F429ZI == 1 )
    {
        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOG_CLK_ENABLE();

        /*Configure GPIO pins : PG13 PG14 */
        GPIO_InitStructure.Pin = GPIO_PIN_13|GPIO_PIN_14;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);
    }
#else
#error "No board defined!"
#endif
}
/*---------------------------------------------------------------------------*/

void vParTestSetLED( portUnsignedBaseType uxLED, portBaseType xValue )
{
    if( uxLED < partestMAX_USER_LED_NUMBER )
    {
        /* Set or clear the output. */
        if( pdFALSE != xValue )
        {
            /* Switch on the specified LED. */
            HAL_GPIO_WritePin( pxLEDPortMapping[ uxLED ], ausLEDPinMapping[ uxLED ], GPIO_PIN_SET );
        }
        else
        {
            /* Switch off the specified LED. */
            HAL_GPIO_WritePin( pxLEDPortMapping[ uxLED ], ausLEDPinMapping[ uxLED ], GPIO_PIN_RESET );
        }
    }
}
/*---------------------------------------------------------------------------*/

void vParTestToggleLED( portUnsignedBaseType uxLED )
{
    if( uxLED < partestMAX_USER_LED_NUMBER )
    {
        safertosapiENTER_CRITICAL();
        {
            HAL_GPIO_TogglePin( pxLEDPortMapping[ uxLED ], ausLEDPinMapping[ uxLED ] );
        }
        safertosapiEXIT_CRITICAL();
    }
}
/*---------------------------------------------------------------------------*/
