/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/*******************************************************************************
* File Name    : main.c
* Device(s)    : RZ/A2M
* Tool-Chain   : e2Studio Ver 7.4.0
*              : GNU Arm Embedded Toolchain 6-2017-q2-update
* OS           : None
* H/W Platform : RZ/A2M Evaluation Board
* Description  : RZ/A2M Sample Program - Main
* Operation    :
* Limitations  :
*******************************************************************************/
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "r_typedefs.h"
#include "iodefine.h"
#include "r_cpg_drv_api.h"
#include "r_ostm_drv_api.h"
#include "r_scifa_drv_api.h"
#include "r_gpio_drv_api.h"
#include "r_startup_config.h"
#include "compiler_settings.h"
#include "main.h"
#include "r_os_abstraction_api.h"
#include "r_task_priority.h"
#include "version.h"

#include "iodefine.h"

/******************************************************************************
Typedef definitions
******************************************************************************/


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern void sample_main(void);
extern void CreateCuiTask(void);

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static int_t gs_my_gpio_handle;

/* Terminal window escape sequences */
static const char_t * const sp_clear_screen = "\x1b[2J";
static const char_t * const sp_cursor_home  = "\x1b[H";

/******************************************************************************
* Function Name: os_main_task_t
* Description  : FreeRTOS main task called by R_OS_KernelInit()
*              : FreeRTOS is now configured and R_OS_Abstraction calls
*              : can be used.
*              : From this point forward no longer use direct_xxx calls.
*              : For example
*              : in place of   direct_open("ostm2", O_RDWR);
*              : use           open(DEVICE_INDENTIFIER "ostm2", O_RDWR);
*              :
* Arguments    : none
* Return Value : 0
******************************************************************************/
int_t os_main_task_t(void)
{
    int_t err;
    st_os_abstraction_info_t ver_info;
    st_r_drv_gpio_sc_config_t pin_led;

    /* For information only
     * Use stdio calls to open drivers once  the kernel is initialised
     *
     * ie
     * int_t ostm3_handle;
     * ostm3_handle = open (DEVICE_INDENTIFIER "ostm2", O_RDWR);
     * close (ostm3_handle);
     */

    gs_my_gpio_handle = open (DEVICE_INDENTIFIER "gpio", O_RDWR);

    /* On error */
    if ( gs_my_gpio_handle < 0 )
    {
        /* stop execute */
        while(1)
        {
            /* Do Nothing */
            ;
        }
    }


    /* ==== Output banner message ==== */
    printf("%s%s", sp_clear_screen, sp_cursor_home);
    printf("RZ/A2M sample for GCC Ver. %u.%u\r\n", APPLICATION_INFO_VERSION, APPLICATION_INFO_RELEASE);
    printf("Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.\r\n");
    printf("Build Info Date %s at %s \r\n", __DATE__, __TIME__);

    if(R_OS_GetVersion(&ver_info) == 0)
    {
        printf("%s Version %d.%d\r\n", ver_info.p_szdriver_name,
                ver_info.version.sub.major, ver_info.version.sub.minor);
    }

    //CreateCuiTask();
    sample_main();
    while(1)
    {
        /* Do Nothing */
        ;
    }
}/* End of function os_main_task_t */

/******************************************************************************
* Function Name: main
* Description  : C Entry point
*              : opens and configures cpg driver
*              : starts the freertos kernel
* Arguments    : none
* Return Value : 0
******************************************************************************/
int_t main(void)
{
    int_t cpg_handle;

    /* configure any drivers that are required before the Kernel initializes */

    /* Initialize the devlink layer */
    R_DEVLINK_Init();

    /* Initialize CPG */
    cpg_handle = direct_open("cpg", 0);
    if ( cpg_handle < 0 )
    {
        /* stop execute */
        while(1)
        {
            /* Do Nothing */
            ;
        }
    }

    /* Can close handle if no need to change clock after here */
    direct_close(cpg_handle);

    /* Start FreeRTOS */
    /* R_OS_InitKernel should never return */
    R_OS_KernelInit();

}/* End of function main */

/* End of File */
