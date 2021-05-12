
#ifdef __cplusplus
extern "C"
{
#endif
    /**
  ******************************************************************************
  * @file           : app_x-cube-ai.c
  * @brief          : AI program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
    /*
  * Description
  *   v1.0 - Minimum template to show how to use the Embedded Client API
  *          model. Only one input and one output is supported. All
  *          memory resources are allocated statically (AI_NETWORK_XX, defines
  *          are used).
  *          Re-target of the printf function is out-of-scope.
  *
  *   For more information, see the embeded documentation:
  *
  *       [1] %X_CUBE_AI_DIR%/Documentation/index.html
  *
  *   X_CUBE_AI_DIR indicates the location where the X-CUBE-AI pack is installed
  *   typical : C:\Users\<user_name>\STM32Cube\Repository\STMicroelectronics\X-CUBE-AI\6.0.0
  */
/* Includes ------------------------------------------------------------------*/
/* System headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "app_x-cube-ai.h"
#include "main.h"
#include "ai_datatypes_defines.h"

/* USER CODE BEGIN includes */
#include "adxl345.h"
#include "usart.h"
#include "math.h"
    /* USER CODE END includes */
    /* Global AI objects */
    static ai_handle anomalydetector = AI_HANDLE_NULL;
    static ai_network_report anomalydetector_info;

    /* Global c-array to handle the activations buffer */
    AI_ALIGNED(4)
    static ai_u8 activations[AI_ANOMALYDETECTOR_DATA_ACTIVATIONS_SIZE];

/*  In the case where "--allocate-inputs" option is used, memory buffer can be
 *  used from the activations buffer. This is not mandatory.
 */
#if !defined(AI_ANOMALYDETECTOR_INPUTS_IN_ACTIVATIONS)
    /* Allocate data payload for input tensor */
    AI_ALIGNED(4)
    static ai_u8 in_data_s[AI_ANOMALYDETECTOR_IN_1_SIZE_BYTES];
#endif

/*  In the case where "--allocate-outputs" option is used, memory buffer can be
 *  used from the activations buffer. This is no mandatory.
 */
#if !defined(AI_ANOMALYDETECTOR_OUTPUTS_IN_ACTIVATIONS)
    /* Allocate data payload for the output tensor */
    AI_ALIGNED(4)
    static ai_u8 out_data_s[AI_ANOMALYDETECTOR_OUT_1_SIZE_BYTES];
#endif

    static void ai_log_err(const ai_error err, const char *fct)
    {
        /* USER CODE BEGIN log */
        if (fct)
            printf("TEMPLATE - Error (%s) - type=0x%02x code=0x%02x\r\n", fct,
                   err.type, err.code);
        else
            printf("TEMPLATE - Error - type=0x%02x code=0x%02x\r\n", err.type, err.code);

        do
        {
        } while (1);
        /* USER CODE END log */
    }

    static int ai_boostrap(ai_handle w_addr, ai_handle act_addr)
    {
        ai_error err;

        /* 1 - Create an instance of the model */
        err = ai_anomalydetector_create(&anomalydetector, AI_ANOMALYDETECTOR_DATA_CONFIG);
        if (err.type != AI_ERROR_NONE)
        {
            ai_log_err(err, "ai_anomalydetector_create");
            return -1;
        }

        /* 2 - Initialize the instance */
        const ai_network_params params = {
            AI_ANOMALYDETECTOR_DATA_WEIGHTS(w_addr),
            AI_ANOMALYDETECTOR_DATA_ACTIVATIONS(act_addr)};

        if (!ai_anomalydetector_init(anomalydetector, &params))
        {
            err = ai_anomalydetector_get_error(anomalydetector);
            ai_log_err(err, "ai_anomalydetector_init");
            return -1;
        }

        /* 3 - Retrieve the network info of the created instance */
        if (!ai_anomalydetector_get_info(anomalydetector, &anomalydetector_info))
        {
            err = ai_anomalydetector_get_error(anomalydetector);
            ai_log_err(err, "ai_anomalydetector_get_error");
            ai_anomalydetector_destroy(anomalydetector);
            anomalydetector = AI_HANDLE_NULL;
            return -3;
        }

        return 0;
    }

    static int ai_run(void *data_in, void *data_out)
    {
        ai_i32 batch;

        ai_buffer *ai_input = anomalydetector_info.inputs;
        ai_buffer *ai_output = anomalydetector_info.outputs;

        ai_input[0].data = AI_HANDLE_PTR(data_in);
        ai_output[0].data = AI_HANDLE_PTR(data_out);

        batch = ai_anomalydetector_run(anomalydetector, ai_input, ai_output);
        if (batch != 1)
        {
            ai_log_err(ai_anomalydetector_get_error(anomalydetector),
                       "ai_anomalydetector_run");
            return -1;
        }

        return 0;
    }

    void float2Bytes(uint8_t bytes_temp[4], float val){
        union{
            float a;
            uint8_t bytes[4];
        } f2b;
        f2b.a = val;
        memcpy(bytes_temp, f2b.bytes, 4);
        //They are in the wring byte order so flip them
        uint8_t tmp = bytes_temp[0];
        bytes_temp[0] = bytes_temp[3];
        bytes_temp[3] = tmp;
        tmp = bytes_temp[1];
        bytes_temp[1] = bytes_temp[2];
        bytes_temp[2] = tmp;
    }

    /* USER CODE BEGIN 2 */
    int acquire_and_process_data(ai_u8 *data)
    {
        // Get 25 sample from the accelerometer (each spaced 2 milliseconds appart, to replicate the speed at wich we sampled our training data)
        Accelerations acc;
        char message[256];
        float acc_array[3];
        uint8_t conv[sizeof(float)];
        for (int i = 0; i < 25; ++i)
        {
            getAccelerations(&hi2c1, &acc);
            acc_array[0] = (float)acc.x * 0.0039;
            acc_array[1] = (float)acc.y * 0.0039;
            acc_array[2] = (float)acc.z * 0.0039;
            for (int n = 0; n < 3; ++n)
            {
                conv[0] = 0;
                conv[1] = 0;
                conv[2] = 0;
                conv[3] = 0;
                //float2Bytes(conv, acc_array[n]);
                memcpy(&(data[i * 3 * sizeof(float) + (n * sizeof(float))]), &acc_array[n], sizeof(float));
                // for (int j = 0; j < sizeof(float); ++j)
                // {
                //     data[(i * 3 * sizeof(float)) + (n * sizeof(float)) + j] = conv[sizeof(float) - j];
                // }
            }
            sprintf(message, "X : %f Y : %f Z : %f  \n", acc_array[0], acc_array[1], acc_array[2]);
            HAL_UART_Transmit(&huart2, message, strlen(message), 100);
            HAL_Delay(2);
        }
        return 0;
    }

    int post_process(ai_u8 *data)
    {
        char message[100];
        float result = 0.0;
        float output_array[4];
        float biggest = 0.0;
        int biggest_index = 0;
        uint8_t bytes[4];
        for(int i = 0; i < 4; ++i){
            bytes[0] = data[i * sizeof(float) + 0];
            bytes[1] = data[i * sizeof(float) + 1];
            bytes[2] = data[i * sizeof(float) + 2];
            bytes[3] = data[i * sizeof(float) + 3];
            result = *(float *)&bytes;
            output_array[i] = result;
            if (result > biggest){
                biggest = result;
                biggest_index = i;
            }
        }
        sprintf(message, "Error_1 : %f\nError_2 : %f\nNormal : %f\nOff : %f\n", output_array[0], output_array[1], output_array[2], output_array[3]);
        HAL_UART_Transmit(&huart2, message, strlen(message), 100);
        return biggest_index;
    }
    /* USER CODE END 2 */

    /*************************************************************************
  *
  */
    void MX_X_CUBE_AI_Init(void)
    {
        /* USER CODE BEGIN 5 */
        printf("\r\nTEMPLATE - initialization\r\n");

        ai_boostrap(ai_anomalydetector_data_weights_get(), activations);
        /* USER CODE END 5 */
    }

    void MX_X_CUBE_AI_Process(void)
    {
        /* USER CODE BEGIN 6 */
        HAL_UART_Transmit(&huart2, (uint8_t *)"Starting AI subroutine!\n", 24, 100);
        int res = -1;
        uint8_t *in_data = NULL;
        uint8_t *out_data = NULL;

        printf("TEMPLATE - run - main loop\r\n");

        if (anomalydetector)
        {

            if ((anomalydetector_info.n_inputs != 1) || (anomalydetector_info.n_outputs != 1))
            {
                ai_error err = {AI_ERROR_INVALID_PARAM, AI_ERROR_CODE_OUT_OF_RANGE};
                ai_log_err(err, "template code should be updated\r\n to support a model with multiple IO");
                return;
            }

            /* 1 - Set the I/O data buffer */

#if AI_ANOMALYDETECTOR_INPUTS_IN_ACTIVATIONS
            in_data = anomalydetector_info.inputs[0].data;
#else
        in_data = in_data_s;
#endif

#if AI_ANOMALYDETECTOR_OUTPUTS_IN_ACTIVATIONS
            out_data = anomalydetector_info.outputs[0].data;
#else
        out_data = out_data_s;
#endif

            if ((!in_data) || (!out_data))
            {
                printf("TEMPLATE - I/O buffers are invalid\r\n");
                return;
            }

            HAL_UART_Transmit(&huart2, (uint8_t *)"Data arrays correctly initialized!\n", 35, 100);
            /* 2 - main loop */
            do
            {
                /* 1 - acquire and pre-process input data */
                HAL_UART_Transmit(&huart2, (uint8_t *)"Acquiring data!\n", 16, 100);
                res = acquire_and_process_data(in_data);
                /* 2 - process the data - call inference engine */
                if (res == 0){
                    HAL_UART_Transmit(&huart2, (uint8_t *)"Infering result!\n", 17, 100);
                    res = ai_run(in_data, out_data);
                }
                /* 3- post-process the predictions */
                if (res == 0){
                    HAL_UART_Transmit(&huart2, (uint8_t *)"Post processing!\n", 17, 100);
                    res = post_process(out_data);
                }
                switch (res)
                {
                case 0:
                    HAL_UART_Transmit(&huart2, (uint8_t *)"Error_1\n", 8, 100);
                    res = 0;
                    break;
                case 1:
                    HAL_UART_Transmit(&huart2, (uint8_t *)"Error_2\n", 8, 100);
                    res = 0;
                    break;
                case 2:
                    HAL_UART_Transmit(&huart2, (uint8_t *)"Normal\n", 7, 100);
                    res = 0;
                    break;
                case 3:
                    HAL_UART_Transmit(&huart2, (uint8_t *)"Off\n", 4, 100);
                    res = 0;
                    break;
                default:
                    res = -1;
                    break;
                }
                HAL_UART_Transmit(&huart2, (uint8_t *)"Inference completed, next round coming up!\n", 43, 100);
                HAL_Delay(1000);
            } while (res == 0);
        }

        if (res)
        {
            ai_error err = {AI_ERROR_INVALID_STATE, AI_ERROR_CODE_NETWORK};
            ai_log_err(err, "Process has FAILED");
        }
        /* USER CODE END 6 */
    }
#ifdef __cplusplus
}
#endif
