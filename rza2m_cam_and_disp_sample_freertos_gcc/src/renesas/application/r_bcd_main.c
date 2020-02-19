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
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <fcntl.h>

#include "FreeRTOS.h"

#include "r_typedefs.h"
#include "iodefine.h"
#include "r_cache_lld_rza2m.h"

#include "r_bcd_camera.h"
#include "r_bcd_lcd.h"
#include "r_jcu.h"
#include "jcu_swap.h"
#include "draw.h"
#include "perform.h"

#include "r_dk2_if.h"
#include "r_drp_simple_isp.h"
#include "r_mmu_drv_api.h"
#include "r_os_abstraction_api.h"

#define R_DRP_SU

#define  GS_N  "\r\n"
/******************************************************************************
Typedef definitions
******************************************************************************/
/* Structure for buffer management between CPU and DRP */


/******************************************************************************
Macro definitions
******************************************************************************/


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static uint32_t jpeg_buffer[((200*1024)/sizeof(uint32_t))+1] __attribute__ ((section("JPEG_RAM")));

/**********************************************************************************************************************
 * Function Name: gs_encode_to_jpeg
 * Description  : .
 * Arguments    : .
 * Return Value : .
 *********************************************************************************************************************/
static errnum_t  gs_encode_to_jpeg(uint32_t *raw_data, uint32_t *jpeg_data, size_t *encoded_size)
{
    errnum_t            e;
    bool_t              s;
    jcu_buffer_param_t  buffer;
    bool_t              is_jcu_initialized = false;
    /* Cast to an uintptr_t */
    uintptr_t           physical_address_of_raw = (uintptr_t)raw_data;
    /* Cast to an uintpty_t */
    uintptr_t           physical_address_of_jpeg = (uintptr_t)jpeg_data;
    uint32_t            encoded_event = 0;
    jcu_encode_param_t  encode;

    /* Variable: a_quantization_table_y
    *    Quality is IJG75 calculated by "QuantizationTable_Generator.xls" */
    static const uint8_t  a_quantization_table_y[] =
    {
        8, 6, 5, 8, 12, 20, 26, 31,
        6, 6, 7, 10, 13, 29, 30, 28,
        7, 7, 8, 12, 20, 29, 35, 28,
        7, 9, 11, 15, 26, 44, 40, 31,
        9, 11, 19, 28, 34, 55, 52, 39,
        12, 18, 28, 32, 41, 52, 57, 46,
        25, 32, 39, 44, 52, 61, 60, 51,
        36, 46, 48, 49, 56, 50, 52, 50
    };

    /* Variable: a_quantization_table_c
    *    Quality is IJG75 calculated by "QuantizationTable_Generator.xls" */
    static const uint8_t  a_quantization_table_c[] =
    {
        9, 9, 12, 24, 50, 50, 50, 50,
        9, 11, 13, 33, 50, 50, 50, 50,
        12, 13, 28, 50, 50, 50, 50, 50,
        24, 33, 50, 50, 50, 50, 50, 50,
        50, 50, 50, 50, 50, 50, 50, 50,
        50, 50, 50, 50, 50, 50, 50, 50,
        50, 50, 50, 50, 50, 50, 50, 50,
        50, 50, 50, 50, 50, 50, 50, 50
    };

    /* Variable: a_huffman_table_y_dc
    *    Example written in ITU-T T81 specification */
    static const uint8_t  a_huffman_table_y_dc[] =
    {
        0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B
    };


    /* Variable: a_huffman_table_c_dc
    *    Example written in ITU-T T81 specification */
    static const uint8_t  a_huffman_table_c_dc[] =
    {
        0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B
    };


    /* Variable: a_huffman_table_y_ac
    *    Example written in ITU-T T81 specification */
    static const uint8_t  a_huffman_table_y_ac[] =
    {
        0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03,
        0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7D,
        0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
        0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
        0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08,
        0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
        0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16,
        0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
        0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
        0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
        0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
        0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
        0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
        0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
        0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6,
        0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
        0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4,
        0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
        0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA,
        0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
        0xF9, 0xFA
    };


    /* Variable: a_huffman_table_c_ac
    *    Example written in ITU-T T81 specification */
    static const uint8_t  a_huffman_table_c_ac[] =
    {
        0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04,
        0x07, 0x05, 0x04, 0x04, 0x00, 0x01, 0x02, 0x77,
        0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
        0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
        0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
        0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
        0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34,
        0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
        0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38,
        0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
        0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
        0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
        0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96,
        0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
        0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4,
        0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
        0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2,
        0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
        0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9,
        0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
        0xF9, 0xFA
    };


    /* (variables) = ... */
    {
        /* physical_address_of_raw = ... */
        e= R_MEMORY_ToPhysicalAddress( raw_data,   &physical_address_of_raw );
        if(e)
        {
            goto fin;
        }

        /* physical_address_of_jpeg = ... */
        e= R_MEMORY_ToPhysicalAddress( jpeg_data,  &physical_address_of_jpeg );
        if(e)
        {
            goto fin;
        }

        /* encoded_event = ... */
        s= R_OS_SemaphoreCreate( &encoded_event,  1 );
        if(!s)
        {
            e=__LINE__;
            goto fin;
        }
    }


    /* Clear JPEG output memory in physical memory */
    memset( jpeg_buffer,  0,  sizeof(jpeg_buffer) );

    R_CACHE_L1DataCleanInvalidAll();


    /* R_JCU_Initialize() */
    e= R_JCU_Initialize( NULL );
    if(e)
    {
        goto fin;
    }
    is_jcu_initialized = true;

    e= R_JCU_SelectCodec( JCU_ENCODE );
    if(e)
    {
        goto fin;
    }


    /* R_JCU_SetQuantizationTable(),  R_JCU_SetHuffmanTable() */
    {
        e= R_JCU_SetQuantizationTable( JCU_TABLE_NO_0, a_quantization_table_y );
        if(e)
        {
            goto fin;
        }
        e= R_JCU_SetQuantizationTable( JCU_TABLE_NO_1, a_quantization_table_c );
        if(e)
        {
            goto fin;
        }
        e= R_JCU_SetHuffmanTable( JCU_TABLE_NO_0, JCU_HUFFMAN_DC, a_huffman_table_y_dc );
        if(e)
        {
            goto fin;
        }
        e= R_JCU_SetHuffmanTable( JCU_TABLE_NO_1, JCU_HUFFMAN_DC, a_huffman_table_c_dc );
        if(e)
        {
            goto fin;
        }
        e= R_JCU_SetHuffmanTable( JCU_TABLE_NO_0, JCU_HUFFMAN_AC, a_huffman_table_y_ac );
        if(e)
        {
            goto fin;
        }
        e= R_JCU_SetHuffmanTable( JCU_TABLE_NO_1, JCU_HUFFMAN_AC, a_huffman_table_c_ac );
        if(e)
        {
            goto fin;
        }
    }


    /* R_JCU_SetEncodeParam() */
    {
        encode.width  = R_BCD_CAMERA_WIDTH;
        encode.height = R_BCD_CAMERA_HEIGHT;
        buffer.source.swapSetting       = JCU_SWAP_CB_Y0_CR_Y1_PIXEL0123;

        /* Cast to an appropriate type */
        buffer.source.address           = (uint32_t*) physical_address_of_raw;

        /* Cast to an appropriate type */
        buffer.destination.address      = (uint32_t*) physical_address_of_jpeg;

        /* Cast to an appropriate type */
        buffer.destination.swapSetting  = JCU_SWAP_JPEG_NONE;

        /* Cast to an appropriate type */
        buffer.lineOffset               = (int16_t) R_BCD_CAMERA_WIDTH;

        /* Cast to an appropriate type */
        encode.encodeFormat = (jcu_jpeg_format_t) JCU_JPEG_YCbCr422;
        encode.QuantizationTable[ JCU_ELEMENT_Y  ] = JCU_TABLE_NO_0;
        encode.QuantizationTable[ JCU_ELEMENT_Cb ] = JCU_TABLE_NO_1;
        encode.QuantizationTable[ JCU_ELEMENT_Cr ] = JCU_TABLE_NO_1;
        encode.HuffmanTable[ JCU_ELEMENT_Y  ] = JCU_TABLE_NO_0;
        encode.HuffmanTable[ JCU_ELEMENT_Cb ] = JCU_TABLE_NO_1;
        encode.HuffmanTable[ JCU_ELEMENT_Cr ] = JCU_TABLE_NO_1;
        encode.DRI_value = 0;
        encode.inputCbCrOffset = JCU_CBCR_OFFSET_128;

        e= R_JCU_SetEncodeParam( &encode, &buffer );
        if(e)
        {
            goto fin;
        }
    }

    printf( "Encoding:"GS_N );

    /* R_JCU_StartAsync() : Encode RAW image to JPEG image */
    s= R_OS_SemaphoreWait( &encoded_event,  0 ); /* Clear. Time out must be ignored. */

    /* Cast to an appropriate type */
    e= R_JCU_StartAsync( (r_co_function_t) R_OS_SemaphoreRelease,  &encoded_event );
    if(e)
    {
        goto fin;
    } /* Set */
    s= R_OS_SemaphoreWait( &encoded_event,  R_OS_ABSTRACTION_EV_WAIT_INFINITE );
    if(!s)
    {
        e=__LINE__;
        goto fin;
    }
    e= R_JCU_GetAsyncError();
    if(e)
    {
        goto fin;
    }


    /* Show encoded JPEG size */
    e= R_JCU_GetEncodedSize( encoded_size );
    if(e)
    {
        goto fin;
    }
   // printf( "size = %u"GS_N,(unsigned long)encoded_size);

    e=0;
fin:
    /* R_JCU_TerminateAsync() */
    if ( is_jcu_initialized )
    {
        errnum_t  ee;
        uint32_t  terminate_event = 0;
        s= R_OS_SemaphoreCreate( &terminate_event,  1 );
        if((!s) && (e==0))
        {
            e=__LINE__;
        }
        s= R_OS_SemaphoreWait( &terminate_event,  0 ); /* Clear. Time out must be ignored. */

        /* Cast to an appropriate type */
        ee = R_JCU_TerminateAsync( (r_co_function_t) R_OS_SemaphoreRelease,  &terminate_event ); /* Set */
        if(e==0)
        {
            e=ee;
        }
        s= R_OS_SemaphoreWait( &terminate_event,  R_OS_ABSTRACTION_EV_WAIT_INFINITE );
        if(!s)
        {
            e=__LINE__;
        }
        ee= R_JCU_GetAsyncError();
        if(e==0)
        {
            e=ee;
        }
        R_OS_SemaphoreDelete( &terminate_event );
    }

    /* ... */
    R_OS_SemaphoreDelete( &encoded_event );
    R_ERROR_Set( e );
    return  e;
} /* End of function gs_EncodeToJPEG */


///******************************************************************************
//* Function Name: sample_main
//* Description  : First function called after initialization is completed
//* Arguments    : -
//* Return Value : -
//******************************************************************************/
//void sample_main(void)
//{
//int32_t   frame_buf_id;
//uint8_t   buf[80];
//uint8_t * input_bufadr;
//uint8_t * output_bufadr;
//size_t jpeg_size;
///* Initialization of VIN and MIPI driver */
//R_BCD_CameraInit();
//
///* Initialization of LCD driver */
//R_BCD_LcdInit();
//
///* Capture Start */
//R_BCD_CameraCaptureStart();
//
///* Initialize of Performance counter */
//PerformInit();
//
///* main loop */
//	uint32_t width;
//	uint32_t height;
//
//	/* Wait until camera capture is complete */
//	while ((frame_buf_id = R_BCD_CameraGetCaptureStatus()) == R_BCD_CAMERA_NOT_CAPTURED)
//	{
//		/* DO NOTHING */
//		;
//	}
//	input_bufadr  = R_BCD_CameraGetFrameAddress(frame_buf_id);
//	output_bufadr = R_BCD_LcdGetVramAddress();
//
//	/* Set start time of process*/
//	PerformSetStartTime(0);
//
//	for( height = 0 ; height < R_BCD_LCD_HEIGHT; height ++ )
//	{
//		for( width = 0 ; width < R_BCD_LCD_WIDTH ; width ++ )
//		{
//			output_bufadr[(R_BCD_LCD_WIDTH * height) + width]
//								= input_bufadr[(R_BCD_CAMERA_WIDTH * height) + width];
//		}
//	}
//	gs_encode_to_jpeg(input_bufadr,jpeg_buffer,jpeg_size);
//	PerformSetEndTime(0);
//	us = PerformGetElapsedTime_us(0);
//	/* Set end time of process */
//
//	printf("%d",jpeg_size);
//	/* Clear the current capture state and enable the detection of the next capture completion */
//	R_BCD_CameraClearCaptureStatus();
//
//	/* Display image processing result */
//	R_BCD_LcdSwapVideoBuffer();
//
//	/* Display time taken for image processing */
//	R_BCD_LcdClearGraphicsBuffer();
//	{
//		printf("%d\r\n",us);
//		sprintf(&buf[0],"Processing time : %d.%d[ms]", us / 1000, (us - ((us / 1000) * 1000)) / 100);
//		R_BCD_LcdWriteString(&buf[0], 2, 2, GREEN);
//	}
//	R_BCD_LcdSwapGraphicsBuffer();
//return;
//}   /* End of function sample_main() */
//
///* End of File */
/******************************************************************************
Macro definitions
******************************************************************************/
#define TILE_0          (0)
#define TILE_1          (1)
#define TILE_2          (2)
#define TILE_3          (3)
#define TILE_4          (4)
#define TILE_5          (5)

#define DRP_NOT_FINISH  (0)
#define DRP_FINISH      (1)

#define DRP_DRV_ASSERT(x) if ((x) != 0) goto sample_finish

/* ISP parameter */
#define ISP_GAIN_R              (0x1500)
#define ISP_GAIN_G              (0x1000)
#define ISP_GAIN_B              (0x18E3)
#define ISP_GAMMA               (0x00)
#define ISP_COMPONENT           (0x00)
#define ISP_BIAS_R              (0x0000)
#define ISP_BIAS_G              (0x0000)
#define ISP_BIAS_B              (0x0000)
#define ISP_MEDIAN_A            (0x0000)
#define ISP_STRENGTH            (0)
#define ISP_CORING              (0)



/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/
static r_drp_simple_isp_t param_isp __attribute__ ((section("UNCACHED_BSS")));

static uint8_t drp_lib_id[R_DK2_TILE_NUM] = {0};
static volatile uint8_t drp_lib_status[R_DK2_TILE_NUM] = {DRP_NOT_FINISH};

static void cb_drp_finish(uint8_t id);

/*******************************************************************************
* Function Name: cb_drp_finish
* Description  : This function is a callback function called from the
*              : DRP driver at the finish of the DRP library processing.
* Arguments    : id
*              :   The ID of the DRP library that finished processing.
* Return Value : -
*******************************************************************************/
static void cb_drp_finish(uint8_t id)
{
    uint32_t tile_no;

    /* Change the operation state of the DRP library notified by the argument to finish */
    for (tile_no = 0; tile_no < R_DK2_TILE_NUM; tile_no++)
    {
        if (drp_lib_id[tile_no] == id)
        {
            drp_lib_status[tile_no] = DRP_FINISH;
            break;
        }
    }

    return;
}
/*******************************************************************************
* End of function cb_drp_finish
*******************************************************************************/
/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern uint8_t g_r_drp_simple_isp_bayer2yuv_6[];
/******************************************************************************
* Function Name: sample_main
* Description  : First function called after initialization is completed
* Arguments    : -
* Return Value : -
******************************************************************************/
void sample_main(void)
{
    int32_t   frame_buf_id;
    uint8_t   buf[80];
    int32_t ret_val;
    uint8_t * input_bufadr;
    uint8_t * output_bufadr;

    /* Initialization of VIN and MIPI driver */
    R_BCD_CameraInit();

    /* Initialization of LCD driver */
    R_BCD_LcdInit();

    /* Capture Start */
    R_BCD_CameraCaptureStart();

    /* Initialize of Performance counter */
    PerformInit();

    /********************************/
    /* Initialization of DRP driver */
    /********************************/
    R_DK2_Initialize();

    /******************************/
    /* Load DRP Library           */
    /*        +-----------------+ */
    /* tile 0 |IspBayer2yuv     | */
    /*        |                 | */
    /* tile 1 |                 | */
    /*        |                 | */
    /* tile 2 |                 | */
    /*        |                 | */
    /* tile 3 |                 | */
    /*        |                 | */
    /* tile 4 |                 | */
    /*        |                 | */
    /* tile 5 |                 | */
    /*        +-----------------+ */
    /******************************/
    ret_val = R_DK2_Load(&g_r_drp_simple_isp_bayer2yuv_6[0],
                            R_DK2_TILE_0,
                            /* Cast to an appropriate type */
                            R_DK2_TILE_PATTERN_6, NULL, &cb_drp_finish, &drp_lib_id[0]);
    DRP_DRV_ASSERT(ret_val);

    /************************/
    /* Activate DRP Library */
    /************************/
    ret_val = R_DK2_Activate(drp_lib_id[TILE_0], 0);
    DRP_DRV_ASSERT(ret_val);
    size_t jpeg_size=0;
    /* main loop */
    while (1)
    {
        uint32_t us=0;
        uint32_t width;
        uint32_t height;
        /* Wait until camera capture is complete */
        while ((frame_buf_id = R_BCD_CameraGetCaptureStatus()) == R_BCD_CAMERA_NOT_CAPTURED)
        {
            /* DO NOTHING */
            R_OS_TaskSleep(1);
        }
        input_bufadr  = R_BCD_CameraGetFrameAddress(frame_buf_id);
        output_bufadr = R_BCD_LcdGetVramAddress();

        /* Cast to an uint32_t */
        R_MMU_VAtoPA((uint32_t)input_bufadr, &(param_isp.src));
        /* Cast to an uint32_t */
        R_MMU_VAtoPA((uint32_t)output_bufadr, &(param_isp.dst));

        /* Set ISP parameters */
        param_isp.width             = R_BCD_CAMERA_WIDTH;
        param_isp.height            = R_BCD_CAMERA_HEIGHT;
        param_isp.gain_r            = ISP_GAIN_R;
        param_isp.gain_g            = ISP_GAIN_G;
        param_isp.gain_b            = ISP_GAIN_B;
        param_isp.gamma             = ISP_GAMMA;
        param_isp.component         = ISP_COMPONENT;
        param_isp.bias_r            = ISP_BIAS_R;
        param_isp.bias_g            = ISP_BIAS_G;
        param_isp.bias_b            = ISP_BIAS_B;
        param_isp.blend             = ISP_MEDIAN_A;
        param_isp.strength          = ISP_STRENGTH;
        param_isp.coring            = ISP_CORING;

        /* Initialize variables to be used in termination judgment of the DRP library */
        drp_lib_status[TILE_0] = DRP_NOT_FINISH;

        /*********************/
        /* Start DRP Library */
        /*********************/
        ret_val = R_DK2_Start(drp_lib_id[TILE_0], (void *)&param_isp, sizeof(r_drp_simple_isp_t));
        DRP_DRV_ASSERT(ret_val);

        /***************************************/
        /* Wait until DRP processing is finish */
        /***************************************/
        while (drp_lib_status[TILE_0] == DRP_NOT_FINISH)
        {
            /* Do Nothing */
            ;
        };

        /* Set start time of process*/
        PerformSetStartTime(0);

        /* Clear the current capture state and enable the detection of the next capture completion */
        R_BCD_CameraClearCaptureStatus();

        /* Capture Start */
        R_BCD_CameraCaptureStart();

        /* Encode JPEG*/
        gs_encode_to_jpeg((uint32_t*)output_bufadr,jpeg_buffer,&jpeg_size);
        printf("JPEG_SIZE:%u"GS_N,jpeg_size);
        /* Display image processing result */
        R_BCD_LcdSwapVideoBuffer();

        /* Display time taken for image processing */
        R_BCD_LcdClearGraphicsBuffer();
        PerformSetEndTime(0);
        us = PerformGetElapsedTime_ms(0);
        printf("Processing time : %d[ms]"GS_N, us);
        sprintf(&buf[0],"Processing time : %d.%d[ms]", us / 1000, (us - ((us / 1000) * 1000)) / 100);
        R_BCD_LcdWriteString(&buf[0], 2, 2, GREEN);

        /* Write the data(buf) on the cache to physical memory */
        R_CACHE_L1DataCleanLine(R_BCD_LcdGetOLVramAddress(), ((R_BCD_LCD_WIDTH * R_BCD_LCD_HEIGHT) / 2));

        /* Display overlay buffer written processing time */
        R_BCD_LcdSwapGraphicsBuffer();
    }
sample_finish:
    {
        return;
    }
}   /* End of function sample_main() */

/* End of File */
