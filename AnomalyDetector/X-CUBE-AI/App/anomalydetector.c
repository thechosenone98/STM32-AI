/**
  ******************************************************************************
  * @file    anomalydetector.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Fri May  7 19:00:33 2021
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */


#include "anomalydetector.h"

#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



/*
#define AI_TOOLS_VERSION_MAJOR 6
#define AI_TOOLS_VERSION_MINOR 0
#define AI_TOOLS_VERSION_MICRO 0
#define AI_TOOLS_VERSION_EXTRA "RC6"

*/

/*
#define AI_TOOLS_API_VERSION_MAJOR 1
#define AI_TOOLS_API_VERSION_MINOR 4
#define AI_TOOLS_API_VERSION_MICRO 0
*/

#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_anomalydetector
 
#undef AI_ANOMALYDETECTOR_MODEL_SIGNATURE
#define AI_ANOMALYDETECTOR_MODEL_SIGNATURE     "b4aafa7e5cde8241888598f2a1d57b8b"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "Fri May  7 19:00:33 2021"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_ANOMALYDETECTOR_N_BATCHES
#define AI_ANOMALYDETECTOR_N_BATCHES         (1)

/**  Forward network declaration section  *************************************/
AI_STATIC ai_network AI_NET_OBJ_INSTANCE;


/**  Forward network array declarations  **************************************/
AI_STATIC ai_array input_0_output_array;   /* Array #0 */
AI_STATIC ai_array conv2d_output_array;   /* Array #1 */
AI_STATIC ai_array conv2d_1_output_array;   /* Array #2 */
AI_STATIC ai_array dense_output_array;   /* Array #3 */
AI_STATIC ai_array dense_nl_output_array;   /* Array #4 */
AI_STATIC ai_array dense_1_output_array;   /* Array #5 */
AI_STATIC ai_array dense_1_nl_output_array;   /* Array #6 */
AI_STATIC ai_array conv2d_weights_array;   /* Array #7 */
AI_STATIC ai_array conv2d_bias_array;   /* Array #8 */
AI_STATIC ai_array conv2d_1_weights_array;   /* Array #9 */
AI_STATIC ai_array conv2d_1_bias_array;   /* Array #10 */
AI_STATIC ai_array dense_weights_array;   /* Array #11 */
AI_STATIC ai_array dense_bias_array;   /* Array #12 */
AI_STATIC ai_array dense_1_weights_array;   /* Array #13 */
AI_STATIC ai_array dense_1_bias_array;   /* Array #14 */
AI_STATIC ai_array conv2d_scratch0_array;   /* Array #15 */
AI_STATIC ai_array conv2d_1_scratch0_array;   /* Array #16 */


/**  Forward network tensor declarations  *************************************/
AI_STATIC ai_tensor input_0_output;   /* Tensor #0 */
AI_STATIC ai_tensor conv2d_output;   /* Tensor #1 */
AI_STATIC ai_tensor conv2d_1_output;   /* Tensor #2 */
AI_STATIC ai_tensor conv2d_1_output0;   /* Tensor #3 */
AI_STATIC ai_tensor dense_output;   /* Tensor #4 */
AI_STATIC ai_tensor dense_nl_output;   /* Tensor #5 */
AI_STATIC ai_tensor dense_1_output;   /* Tensor #6 */
AI_STATIC ai_tensor dense_1_nl_output;   /* Tensor #7 */
AI_STATIC ai_tensor conv2d_weights;   /* Tensor #8 */
AI_STATIC ai_tensor conv2d_bias;   /* Tensor #9 */
AI_STATIC ai_tensor conv2d_1_weights;   /* Tensor #10 */
AI_STATIC ai_tensor conv2d_1_bias;   /* Tensor #11 */
AI_STATIC ai_tensor dense_weights;   /* Tensor #12 */
AI_STATIC ai_tensor dense_bias;   /* Tensor #13 */
AI_STATIC ai_tensor dense_1_weights;   /* Tensor #14 */
AI_STATIC ai_tensor dense_1_bias;   /* Tensor #15 */
AI_STATIC ai_tensor conv2d_scratch0;   /* Tensor #16 */
AI_STATIC ai_tensor conv2d_1_scratch0;   /* Tensor #17 */


/**  Forward network tensor chain declarations  *******************************/
AI_STATIC_CONST ai_tensor_chain conv2d_chain;   /* Chain #0 */
AI_STATIC_CONST ai_tensor_chain conv2d_1_chain;   /* Chain #1 */
AI_STATIC_CONST ai_tensor_chain dense_chain;   /* Chain #2 */
AI_STATIC_CONST ai_tensor_chain dense_nl_chain;   /* Chain #3 */
AI_STATIC_CONST ai_tensor_chain dense_1_chain;   /* Chain #4 */
AI_STATIC_CONST ai_tensor_chain dense_1_nl_chain;   /* Chain #5 */


/**  Forward network layer declarations  **************************************/
AI_STATIC ai_layer_conv2d_nl_pool conv2d_layer; /* Layer #0 */
AI_STATIC ai_layer_conv2d_nl_pool conv2d_1_layer; /* Layer #1 */
AI_STATIC ai_layer_dense dense_layer; /* Layer #2 */
AI_STATIC ai_layer_nl dense_nl_layer; /* Layer #3 */
AI_STATIC ai_layer_dense dense_1_layer; /* Layer #4 */
AI_STATIC ai_layer_nl dense_1_nl_layer; /* Layer #5 */




/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  input_0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 75, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 156, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 168, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  dense_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  dense_nl_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_output_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_nl_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 4, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 36, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 480, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 8, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  dense_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4200, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  dense_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 25, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_weights_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 100, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  dense_1_bias_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 4, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 24, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  conv2d_1_scratch0_array, AI_ARRAY_FORMAT_FLOAT,
  NULL, NULL, 48, AI_STATIC)

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  input_0_output, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 1, 3, 25), AI_STRIDE_INIT(4, 4, 4, 4, 12),
  1, &input_0_output_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 3, 13), AI_STRIDE_INIT(4, 4, 4, 16, 48),
  1, &conv2d_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_output, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 3, 7), AI_STRIDE_INIT(4, 4, 4, 32, 96),
  1, &conv2d_1_output_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_output0, AI_STATIC,
  3, 0x0,
  AI_SHAPE_INIT(4, 1, 168, 1, 1), AI_STRIDE_INIT(4, 4, 4, 672, 672),
  1, &conv2d_1_output_array, NULL)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  dense_output, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 25, 1, 1), AI_STRIDE_INIT(4, 4, 4, 100, 100),
  1, &dense_output_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  dense_nl_output, AI_STATIC,
  5, 0x0,
  AI_SHAPE_INIT(4, 1, 25, 1, 1), AI_STRIDE_INIT(4, 4, 4, 100, 100),
  1, &dense_nl_output_array, NULL)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_output, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &dense_1_output_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_nl_output, AI_STATIC,
  7, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &dense_1_nl_output_array, NULL)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_weights, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 3, 3, 4), AI_STRIDE_INIT(4, 4, 4, 12, 36),
  1, &conv2d_weights_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_bias, AI_STATIC,
  9, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &conv2d_bias_array, NULL)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_weights, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 4, 3, 5, 8), AI_STRIDE_INIT(4, 4, 16, 48, 240),
  1, &conv2d_1_weights_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_bias, AI_STATIC,
  11, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &conv2d_1_bias_array, NULL)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  dense_weights, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 168, 25, 1, 1), AI_STRIDE_INIT(4, 4, 672, 16800, 16800),
  1, &dense_weights_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  dense_bias, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 25, 1, 1), AI_STRIDE_INIT(4, 4, 4, 100, 100),
  1, &dense_bias_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_weights, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 25, 4, 1, 1), AI_STRIDE_INIT(4, 4, 100, 400, 400),
  1, &dense_1_weights_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  dense_1_bias, AI_STATIC,
  15, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &dense_1_bias_array, NULL)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_scratch0, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 3, 2), AI_STRIDE_INIT(4, 4, 4, 16, 48),
  1, &conv2d_scratch0_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  conv2d_1_scratch0, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 3, 2), AI_STRIDE_INIT(4, 4, 4, 32, 96),
  1, &conv2d_1_scratch0_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &input_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_weights, &conv2d_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_layer, 1,
  OPTIMIZED_CONV2D_TYPE, 0x0, NULL,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &conv2d_chain,
  &AI_NET_OBJ_INSTANCE, &conv2d_1_layer, AI_STATIC, 
  .groups = 1, 
  .nl_params = NULL, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 1, 1, 1, 1), 
  .pool_size = AI_SHAPE_2D_INIT(1, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 1, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conv2d_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 3, &conv2d_1_weights, &conv2d_1_bias, NULL),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  conv2d_1_layer, 3,
  OPTIMIZED_CONV2D_TYPE, 0x0, NULL,
  conv2d_nl_pool, forward_conv2d_nl_pool,
  &conv2d_1_chain,
  &AI_NET_OBJ_INSTANCE, &dense_layer, AI_STATIC, 
  .groups = 1, 
  .nl_params = NULL, 
  .nl_func = nl_func_relu_array_f32, 
  .filter_stride = AI_SHAPE_2D_INIT(1, 1), 
  .dilation = AI_SHAPE_2D_INIT(1, 1), 
  .filter_pad = AI_SHAPE_INIT(4, 2, 1, 2, 1), 
  .pool_size = AI_SHAPE_2D_INIT(1, 2), 
  .pool_stride = AI_SHAPE_2D_INIT(1, 2), 
  .pool_pad = AI_SHAPE_INIT(4, 0, 0, 1, 0), 
  .pool_func = pool_func_mp_array_f32, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conv2d_1_output0),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_weights, &dense_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_layer, 5,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_chain,
  &AI_NET_OBJ_INSTANCE, &dense_nl_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_nl_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_nl_layer, 5,
  NL_TYPE, 0x0, NULL,
  nl, forward_relu,
  &dense_nl_chain,
  &AI_NET_OBJ_INSTANCE, &dense_1_layer, AI_STATIC, 
  .nl_params = NULL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_nl_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &dense_1_weights, &dense_1_bias),
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_1_layer, 7,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense,
  &dense_1_chain,
  &AI_NET_OBJ_INSTANCE, &dense_1_nl_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  dense_1_nl_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &dense_1_nl_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  dense_1_nl_layer, 7,
  NL_TYPE, 0x0, NULL,
  nl, forward_sm,
  &dense_1_nl_chain,
  &AI_NET_OBJ_INSTANCE, &dense_1_nl_layer, AI_STATIC, 
  .nl_params = NULL, 
)


AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 19428, 1,
                     NULL),
  AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8,
                     1, 1, 1296, 1,
                     NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_ANOMALYDETECTOR_IN_NUM, &input_0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_ANOMALYDETECTOR_OUT_NUM, &dense_1_nl_output),
  &conv2d_layer, 0, NULL)



AI_DECLARE_STATIC
ai_bool anomalydetector_configure_activations(
  ai_network* net_ctx, const ai_buffer* activation_buffer)
{
  AI_ASSERT(net_ctx &&  activation_buffer && activation_buffer->data)

  ai_ptr activations = AI_PTR(AI_PTR_ALIGN(activation_buffer->data, 4));
  AI_ASSERT(activations)
  AI_UNUSED(net_ctx)

  {
    /* Updating activations (byte) offsets */
    input_0_output_array.data = AI_PTR(NULL);
    input_0_output_array.data_start = AI_PTR(NULL);
    conv2d_output_array.data = AI_PTR(activations + 480);
    conv2d_output_array.data_start = AI_PTR(activations + 480);
    conv2d_1_output_array.data = AI_PTR(activations + 0);
    conv2d_1_output_array.data_start = AI_PTR(activations + 0);
    dense_output_array.data = AI_PTR(activations + 672);
    dense_output_array.data_start = AI_PTR(activations + 672);
    dense_nl_output_array.data = AI_PTR(activations + 0);
    dense_nl_output_array.data_start = AI_PTR(activations + 0);
    dense_1_output_array.data = AI_PTR(activations + 100);
    dense_1_output_array.data_start = AI_PTR(activations + 100);
    dense_1_nl_output_array.data = AI_PTR(NULL);
    dense_1_nl_output_array.data_start = AI_PTR(NULL);
    conv2d_scratch0_array.data = AI_PTR(activations + 384);
    conv2d_scratch0_array.data_start = AI_PTR(activations + 384);
    conv2d_1_scratch0_array.data = AI_PTR(activations + 1104);
    conv2d_1_scratch0_array.data_start = AI_PTR(activations + 1104);
    
  }
  return true;
}



AI_DECLARE_STATIC
ai_bool anomalydetector_configure_weights(
  ai_network* net_ctx, const ai_buffer* weights_buffer)
{
  AI_ASSERT(net_ctx &&  weights_buffer && weights_buffer->data)

  ai_ptr weights = AI_PTR(weights_buffer->data);
  AI_ASSERT(weights)
  AI_UNUSED(net_ctx)

  {
    /* Updating weights (byte) offsets */
    
    conv2d_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_weights_array.data = AI_PTR(weights + 0);
    conv2d_weights_array.data_start = AI_PTR(weights + 0);
    conv2d_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_bias_array.data = AI_PTR(weights + 144);
    conv2d_bias_array.data_start = AI_PTR(weights + 144);
    conv2d_1_weights_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_weights_array.data = AI_PTR(weights + 160);
    conv2d_1_weights_array.data_start = AI_PTR(weights + 160);
    conv2d_1_bias_array.format |= AI_FMT_FLAG_CONST;
    conv2d_1_bias_array.data = AI_PTR(weights + 2080);
    conv2d_1_bias_array.data_start = AI_PTR(weights + 2080);
    dense_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_weights_array.data = AI_PTR(weights + 2112);
    dense_weights_array.data_start = AI_PTR(weights + 2112);
    dense_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_bias_array.data = AI_PTR(weights + 18912);
    dense_bias_array.data_start = AI_PTR(weights + 18912);
    dense_1_weights_array.format |= AI_FMT_FLAG_CONST;
    dense_1_weights_array.data = AI_PTR(weights + 19012);
    dense_1_weights_array.data_start = AI_PTR(weights + 19012);
    dense_1_bias_array.format |= AI_FMT_FLAG_CONST;
    dense_1_bias_array.data = AI_PTR(weights + 19412);
    dense_1_bias_array.data_start = AI_PTR(weights + 19412);
  }

  return true;
}


/**  PUBLIC APIs SECTION  *****************************************************/

AI_API_ENTRY
ai_bool ai_anomalydetector_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if ( report && net_ctx )
  {
    ai_network_report r = {
      .model_name        = AI_ANOMALYDETECTOR_MODEL_NAME,
      .model_signature   = AI_ANOMALYDETECTOR_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 27106,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .activations       = AI_STRUCT_INIT,
      .params            = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x0,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }

  return false;
}

AI_API_ENTRY
ai_error ai_anomalydetector_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}

AI_API_ENTRY
ai_error ai_anomalydetector_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    &AI_NET_OBJ_INSTANCE,
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}

AI_API_ENTRY
ai_handle ai_anomalydetector_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}

AI_API_ENTRY
ai_bool ai_anomalydetector_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = ai_platform_network_init(network, params);
  if ( !net_ctx ) return false;

  ai_bool ok = true;
  ok &= anomalydetector_configure_weights(net_ctx, &params->params);
  ok &= anomalydetector_configure_activations(net_ctx, &params->activations);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_anomalydetector_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}

AI_API_ENTRY
ai_i32 ai_anomalydetector_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_ANOMALYDETECTOR_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

