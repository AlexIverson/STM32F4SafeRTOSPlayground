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
 * This is a 'quick start' project to demonstrate the use of SafeRTOS running
 * on an STM32F429 platform with MPU enabled. It can be used to quickly
 * evaluate SafeRTOS and as a reference for the project settings and syntax
 * necessary to utilise SafeRTOS in an GCC based STM32F4xx project.
 *
 * This demo program is structured as follows:
 * - Kernel: this folder contains the product source code.
 * - Common: this folder contains standard demo tasks that are used to
 *          illustrate the RTOS operation.
 * - Drivers: the ST provided peripheral driver files.
 * - FullDemo.c: Initialises the standard demo tasks and provides a check task
 *          to monitor the operation of the standard demo tasks.
 * - ParTest.c: provides simple routines to initialise and control the LEDs on
 *          the board.
 * - PortSpecifics.c/h: Provide macros and declarations that tune the standard
 *          demo tasks for operation with IAR and the STM32F4xx processor.
 * - SafeRTOSConfig.c/h: Provide macro's and declaration that provide an example
 *          of configuring and initialising the SafeRTOS system. Note that these
 *          declarations are considered to be part of the host application but
 *          must be provided for correct operation of SafeRTOS.
 * - main.c: This provides an example of managing the scheduler initialisation,
 *          performing any required application initialisation and starting the
 *          scheduler.
 *****************************************************************************/

/* Standard includes. */
#include <stdio.h>
#include <string.h>

/* SafeRTOS includes. */
#include "SafeRTOS_API.h"

/* Demo application includes. */
#include "FullDemo.h"

#include "ParTest.h"
#include "main.h"
#include "SafeRTOSHooks.h"




/*
 * This demo project was developed for two specific devices.
 * In order to select each device, the following macros must
 * be enabled and/or disabled in the project settings.
 *      USE_DISC_F429ZI=(1 or 0)
 *      USE_NUCLEO_F429ZI=(1 or 0)
 * */

/* Private function prototypes -----------------------------------------------*/
void prvSetupHardware( void );
portBaseType xInitializeScheduler( void );
/*---------------------------------------------------------------------------*/

/*
 * Calls routines to:
 * - Initialise the hardware;
 * - Initialise the scheduler;
 * - Create the demo application tasks;
 * - Start the scheduler.
 */
int main( void )
{
    portBaseType xStatus = pdPASS;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    prvSetupHardware();

    /* Initialise the kernel passing in a pointer to the xPortInit structure. */
    if( xInitializeScheduler() == pdPASS )
    {
        /* Create the demonstration tasks. */
        if( pdPASS == xStatus )
        {
            xStatus = xFullDemoCreate();
        }

        /* If everything is OK so far, start scheduler here. */
        if( pdPASS == xStatus )
        {
            xStatus = xTaskStartScheduler();
        }
    }

    /* The scheduler should now be running the tasks so this line should not
     * ever be reached. If it is reached, then the system initialisation
     * failed. */
    vErrorHandler();
}
/*---------------------------------------------------------------------------*/


/**
  * @brief System Clock Configuration
  * @retval None
  */
void prvSetupHardware( void )
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    {
        /** Configure the main internal regulator output voltage
        */
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        /** Initializes the RCC Oscillators according to the specified parameters
        * in the RCC_OscInitTypeDef structure.
        */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
        RCC_OscInitStruct.HSIState = RCC_HSI_ON;
        RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
        RCC_OscInitStruct.PLL.PLLM = 8;
        RCC_OscInitStruct.PLL.PLLN = 180;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 6;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /** Activate the Over-Drive mode
        */
        if (HAL_PWREx_EnableOverDrive() != HAL_OK)
        {
            Error_Handler();
        }

        /** Initializes the CPU, AHB and APB buses clocks
        */
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
        {
            Error_Handler();
        }
      }

    vParTestInitialise();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
