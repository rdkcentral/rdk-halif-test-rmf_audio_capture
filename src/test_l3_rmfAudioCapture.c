/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:*
* Copyright 2024 RDK Management
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


/**
 * @addtogroup HPK Hardware Porting Kit
 * @{
 * @par The Hardware Porting Kit
 * HPK is the next evolution of the well-defined Hardware Abstraction Layer
 * (HAL), but augmented with more comprehensive documentation and test suites
 * that OEM or SOC vendors can use to self-certify their ports before taking
 * them to RDKM for validation or to an operator for final integration and
 * deployment. The Hardware Porting Kit effectively enables an OEM and/or SOC
 * vendor to self-certify their own Video Accelerator devices, with minimal RDKM
 * assistance.
 */

/**
 * @addtogroup RMF_AudioCapture RMF Audio Capture
 * @{
 *
 */

/**
 * @addtogroup RMF_AudioCapture_HALTESTS RMF Audio Capture HAL Tests
 * @{
 * @par
 * Unit Testing Suite for RMF Audio Capture HAL
 */

/**
 * @defgroup RMF_AudioCapture_HALTESTS_L3 RMF Audio Capture HAL Tests L3 File
 * @{
 * @parblock
 *
 * ### L3 Tests for RMF Audio Capture HAL :
 *
 * Level 3 module tests will perform module level testing by exercising the full set of APIs to validate various use cases.
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [rmf-audio-capture_halSpec.md](../../docs/pages/rmf-audio-capture_halSpec.md)
 *
 * @endparblock
 *
 */


/**
* @file test_l3_rmfAudioCapture.c
*
*/


#include <ut.h>
#include <ut_control_plane.h>
#include <ut_cunit.h>
#include <ut_kvp_profile.h>
#include <unistd.h>
#include <stdatomic.h>
#include "rmfAudioCapture.h"
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MEASUREMENT_WINDOW_SECONDS 10
#define MEASUREMENT_WINDOW_2MINUTES 120
#define MONITOR_JITTER_MICROSECONDS 100000 //100ms
#define WAIT_WINDOW_SECONDS 2
#define DATA_RATE 192000    // Bytes per second
#define TOTAL_DATA_SIZE (DATA_RATE * MEASUREMENT_WINDOW_SECONDS) // Total data size
#define THRESHOLD 16 * 1024

static int gTestGroup = 3;
static int gTestID = 1;

/* rmf_error */
const static ut_control_keyStringMapping_t rmfError_mapTable [] = {
  { "RMF_SUCCESS",                    (int32_t)RMF_SUCCESS               },
  { "RMF_ERROR",                      (int32_t)RMF_ERROR                 },
  { "RMF_INVALID_PARM",               (int32_t)RMF_INVALID_PARM          },
  { "RMF_INVALID_HANDLE",             (int32_t)RMF_INVALID_HANDLE        },
  { "RMF_NOT_INITIALIZED",            (int32_t)RMF_NOT_INITIALIZED       },
  { "RMF_INVALID_STATE",              (int32_t)RMF_INVALID_STATE         },
  {  NULL, -1 }
};

typedef struct
{
    uint32_t bytes_received;
    atomic_int cookie;
    unsigned char *data_buffer;
} capture_session_context_t;

static bool g_aux_capture_supported = false;

/**
 * @brief Function to extract values from RMF_AudioCapture_Settings
 *
 * This function is called when values from settings are required for other operations
 */
static rmf_Error getValuesFromSettings(RMF_AudioCapture_Settings *settings, uint16_t *num_channels, uint32_t *sampling_rate, uint16_t *bits_per_sample)
{    
    switch (settings->format)
    {
    case racFormat_e16BitStereo:
        *bits_per_sample = 16;
        *num_channels = 2;
        break;
    case racFormat_e24BitStereo:
        *bits_per_sample = 24;
        *num_channels = 2;
        break;
    case racFormat_e16BitMonoLeft:  // fall through
    case racFormat_e16BitMonoRight: // fall through
    case racFormat_e16BitMono:
        *bits_per_sample = 16;
        *num_channels = 1;
        break;
    case racFormat_e24Bit5_1:
        *bits_per_sample = 24;
        *num_channels = 6;
        break;
    default: // Unsupported format
        UT_LOG_DEBUG("Error: Invalid format detected.\n");
        return RMF_ERROR;
    }

    switch (settings->samplingFreq)
    {
    case racFreq_e16000:
        *sampling_rate = 16000;
        break;
    case racFreq_e22050:
        *sampling_rate = 22050;
        break;
    case racFreq_e24000:
        *sampling_rate = 24000;
        break;
    case racFreq_e32000:
        *sampling_rate = 32000;
        break;
    case racFreq_e44100:
        *sampling_rate = 44100;
        break;
    case racFreq_e48000:
        *sampling_rate = 48000;
        break;
    default: // unsupported sampling rate
        UT_LOG_DEBUG("Error: Invalid samping rate detected.\n");
        return RMF_ERROR;
    }    
    return RMF_SUCCESS;
}

/**
 * @brief Callback function for buffer ready
 *
 * This function is triggered when the audio samples are ready (for counting tests)
 */
static rmf_Error test_l3_counting_data_cb(void *context_blob, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    capture_session_context_t *ctx = (capture_session_context_t *)context_blob;

    UT_ASSERT_PTR_NOT_NULL(AudioCaptureBuffer);
    UT_ASSERT_PTR_NOT_NULL_FATAL(context_blob);
    UT_ASSERT_TRUE(AudioCaptureBufferSize > 0);

    ctx->bytes_received += AudioCaptureBufferSize;
    ctx->cookie = 1;
    return RMF_SUCCESS;
}

/**
 * @brief Function to set RMF_AudioCapture_Settings as required for the tests.
 *
 * This function is called to set buffer ready callback and caller context data
 */
static void test_l3_prepare_start_settings_for_data_counting(RMF_AudioCapture_Settings *settings, void *context_blob)
{
    UT_LOG_INFO("Setting cb buffer ready and caller context data");
    settings->cbBufferReady = test_l3_counting_data_cb;
    settings->cbStatusChange = NULL;
    settings->cbBufferReadyParm = context_blob;
}

/**
 * @brief Callback function for buffer ready
 *
 * This function is triggered when the audio samples are ready (for tests that capture audio buffer)
 */
static rmf_Error test_l3_tracking_data_cb(void *context_blob, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    capture_session_context_t *ctx = (capture_session_context_t *)context_blob;

    UT_ASSERT_PTR_NOT_NULL(AudioCaptureBuffer);
    UT_ASSERT_PTR_NOT_NULL_FATAL(context_blob);
    UT_ASSERT_TRUE(AudioCaptureBufferSize > 0);

    ctx->cookie = 1;

    // Copy data into the global buffer
    memcpy(ctx->data_buffer + ctx->bytes_received, AudioCaptureBuffer, AudioCaptureBufferSize);
    ctx->bytes_received += AudioCaptureBufferSize;
    
    return RMF_SUCCESS;
}

/**
 * @brief Function to set RMF_AudioCapture_Settings as required for the tests.
 *
 * This function is called to set buffer ready callback and caller context data.
 */
static void test_l3_prepare_start_settings_for_data_tracking(RMF_AudioCapture_Settings *settings, void *context_blob)
{
    UT_LOG_INFO("Setting cb buffer ready and caller context data");
    settings->cbBufferReady = test_l3_tracking_data_cb;
    settings->cbStatusChange = NULL;
    settings->cbBufferReadyParm = context_blob;

    /* Calculate data size for audio data */
    capture_session_context_t *ctx = (capture_session_context_t *)context_blob;
    uint16_t num_channels = 0;
    uint32_t sampling_rate = 0;
    uint16_t bits_per_sample = 0;
    uint64_t computed_data_size = 0;
    ctx->data_buffer = NULL;

    if (RMF_SUCCESS != getValuesFromSettings(settings, &num_channels, &sampling_rate, &bits_per_sample))
    {
        computed_data_size = TOTAL_DATA_SIZE;
    } else
    {    
        computed_data_size = MEASUREMENT_WINDOW_SECONDS * num_channels * sampling_rate * bits_per_sample / 8;
    }
    // Allocate buffer to store audio data
    ctx->data_buffer = (unsigned char *)malloc(computed_data_size);
    if (ctx->data_buffer == NULL)
    {
        UT_FAIL_FATAL("Aborting test - Error allocating buffer to store audio data");
    }
    ctx->bytes_received = 0;
}

static rmf_Error validateBytesReceived(RMF_AudioCapture_Settings *settings, void *context_blob, int testedTime)
{
    UT_LOG_INFO("Validate bytes received with expected bytes");
    capture_session_context_t *ctx = (capture_session_context_t *)context_blob;

    /* Get values based on settings */
    uint16_t num_channels = 0;
    uint32_t sampling_rate = 0;
    uint16_t bits_per_sample = 0;

    if (RMF_SUCCESS != getValuesFromSettings(settings, &num_channels, &sampling_rate, &bits_per_sample) )
    {
        UT_LOG_DEBUG("Error: Invalid values detected in settings ! Not able to calculate data size !");
        return RMF_ERROR;
    }

    /* Check actual bytes received is over 90% of expected data size before  */
    uint64_t computed_bytes_received = testedTime * num_channels * sampling_rate * bits_per_sample / 8;
    double percentage_received = (double)ctx->bytes_received / (double)computed_bytes_received * 100;
    UT_LOG_DEBUG("Actual bytes received: %" PRIu64 ", Expected bytes received: %" PRIu64 ", Computed percentage: %f\n",
                 ctx->bytes_received, computed_bytes_received, percentage_received);
    if ((percentage_received <= 90.0) || (percentage_received >= 110.0))
    {
        UT_LOG_DEBUG("Error: data delivery does not meet tolerance!");
        return RMF_ERROR;
    }
    return RMF_SUCCESS;
}

/**
 * @brief Function to write captured audio data to a wav file
 *
 * This function is called after audio is captured, at end of test to create output file
 */
static rmf_Error test_l3_write_wav_file(RMF_AudioCapture_Settings *settings, void *context_blob, const char *filename)
{
    UT_LOG_INFO("Called test_l3_write_wav_file with output file name %s", filename);
    capture_session_context_t *ctx = (capture_session_context_t *)context_blob;
    
    /* Get values based on settings */
    uint16_t num_channels = 0;
    uint32_t sampling_rate = 0;
    uint16_t bits_per_sample = 0;
    uint32_t data_rate = 0;

    /* Validate if acceptable level of bytes received first */
    if (RMF_SUCCESS != validateBytesReceived(settings, (void *)context_blob, MEASUREMENT_WINDOW_SECONDS) )
    {
        UT_LOG_ERROR ("Bytes received is not in acceptable levels. Output file will not be created !");
        return RMF_ERROR;
    }

    if (RMF_SUCCESS != getValuesFromSettings(settings, &num_channels, &sampling_rate, &bits_per_sample) )
    {
        UT_LOG_DEBUG("Error: Invalid values detected in settings ! Not able to calculate data size !");
        if(ctx->data_buffer) 
        {
            free(ctx->data_buffer);
            ctx->data_buffer = NULL;
        }
        return RMF_ERROR;
    }

    /* Start writing wav file  */
    FILE *file = fopen(filename, "wb");
    if (file == NULL) 
    {
        UT_LOG_ERROR("Error with fopen for output wav file");
        if(ctx->data_buffer) 
        {
            free(ctx->data_buffer);
            ctx->data_buffer = NULL;
        }
        return RMF_ERROR;
    }
    uint32_t fmt_chunk_size = 16; // Size of the fmt chunk
    uint16_t audio_format = 1;  // PCM format
    
    /* Write WAV Header first */
    fwrite("RIFF", 1, 4, file);
    uint32_t fileSize = 36 + ctx->bytes_received;
    fwrite(&fileSize, 4, 1, file);
    fwrite("WAVE", 1, 4, file);
    fwrite("fmt ", 1, 4, file);
    fwrite(&fmt_chunk_size, 4, 1, file);
    fwrite(&audio_format, 2, 1, file);

    data_rate = sampling_rate * num_channels * bits_per_sample / 8;
    uint16_t block_align = num_channels * bits_per_sample / 8;
    fwrite(&num_channels, 2, 1, file);
    fwrite(&sampling_rate, 4, 1, file);
    fwrite(&data_rate, 4, 1, file);
    fwrite(&block_align, 2, 1, file);
    fwrite(&bits_per_sample, 2, 1, file);
    fwrite("data", 1, 4, file);
    fwrite(&ctx->bytes_received, 4, 1, file);

    /* Write PCM Data */
    fwrite(ctx->data_buffer, 1, ctx->bytes_received, file);

    fclose(file);
    if(ctx->data_buffer) 
    {
        free(ctx->data_buffer);
        ctx->data_buffer = NULL;
    }
    UT_LOG_INFO("test_l3_write_wav_file created output file : %s", filename);
    return RMF_SUCCESS;
}

/**
 * @brief Function to monitor bytes received to detect jitter
 *
 * This function checks byte received in intervals to make sure there is no jitter
 */
static void* monitorBufferCount(void* context_blob) 
{
    UT_LOG_INFO("Created thread to monitor buffer for jitter");
    capture_session_context_t *ctx = (capture_session_context_t *)context_blob;
    uint32_t bytes_received = 0;
    uint32_t difference_in_bytes = 0;
    time_t start_time = time(NULL);
    time_t end_time = start_time + MEASUREMENT_WINDOW_2MINUTES;
    rmf_Error *result = malloc (sizeof(rmf_Error));
    if (result == NULL)
    {
        UT_LOG_ERROR ("malloc for storing rmf_Error failed, refer prints to confirm if jitter test passed");
    }

    while ((ctx->cookie == 1) && (time(NULL) < end_time))
    {
        difference_in_bytes = ctx->bytes_received - bytes_received;
        if (difference_in_bytes < THRESHOLD) 
        {
            UT_LOG_INFO ("Bytes received in last iteration : %d. This is less than threshold level of %d bytes.\n", difference_in_bytes, THRESHOLD);
            UT_LOG_ERROR ("Jitter detected !");
            *result = RMF_ERROR;
            return (void *)result;
        }
        bytes_received = ctx->bytes_received;
        usleep (MONITOR_JITTER_MICROSECONDS); //100ms
    }
    UT_LOG_INFO("No jitter detected");
    *result = RMF_SUCCESS;
    return (void *)result;
}

/**
* @brief Test the primary audio capture functionality
*
* This test verifies the primary audio capture functionality by opening an audio capture handle,
* getting default settings, modifying settings, starting the capture, capturing audio for a duration,
* writing it to a file for validation, stopping the capture, and finally closing the handle. 
* The test ensures that each step returns the expected success status and that 
* the callbacks are invoked correctly.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_primary_data_check(void)
{
    RMF_AudioCaptureHandle handle;
    RMF_AudioCapture_Settings settings;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Calling RMF_AudioCapture_Open(OUT:handle:[])");
    result = RMF_AudioCapture_Open(&handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Open(OUT:handle:[0x%0X]) rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:settings:[0x%0X]) rmf_error:[%s]", &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    capture_session_context_t ctx = {0, 0, 0};
    test_l3_prepare_start_settings_for_data_tracking(&settings, (void *)&ctx);

    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X])", &handle, &settings);
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &handle, &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(handle);
        if(ctx.data_buffer) 
        {
            free(ctx.data_buffer);
            ctx.data_buffer = NULL;
        }
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }

    UT_LOG_INFO("Started audio capture, wait for %d seconds ", MEASUREMENT_WINDOW_SECONDS);
    sleep(MEASUREMENT_WINDOW_SECONDS);
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Stop(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(ctx.cookie, 0);

    result = test_l3_write_wav_file(&settings, (void *)&ctx, "/tmp/output_primary.wav");
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Close(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test jitter for primary audio capture
*
* This test verifies the primary audio capture functionality by opening an audio capture handle,
* getting default settings, modifying settings, starting the capture, capturing audio for a duration,
* stopping the capture, and finally closing the handle. The test verifies that jitter is low 
* enough to avoid underruns when compared against a given threshold.
* The test ensures that each step returns the expected success status and that 
* the callbacks are invoked correctly. 
*
* **Test Group ID:** 03@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_primary_jitter_check(void)
{
    RMF_AudioCaptureHandle handle;
    RMF_AudioCapture_Settings settings;
    rmf_Error result = RMF_SUCCESS;
    pthread_t thread;
    void *ret_value;

    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Calling RMF_AudioCapture_Open(OUT:handle:[])");
    result = RMF_AudioCapture_Open(&handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Open(OUT:handle:[0x%0X]) rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:settings:[0x%0X]) rmf_error:[%s]", &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    capture_session_context_t ctx = {0, 0, 0};
    test_l3_prepare_start_settings_for_data_counting(&settings, (void *)&ctx);

    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X])", &handle, &settings);
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &handle, &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }
    usleep(MONITOR_JITTER_MICROSECONDS);

    UT_LOG_INFO("Monitoring buffers for jitter, maximum wait time is %d seconds ", MEASUREMENT_WINDOW_2MINUTES);
    // Create the thread
    if (pthread_create(&thread, NULL, monitorBufferCount, (void *)&ctx) != 0) 
    {
        result = RMF_INVALID_PARM;
        UT_FAIL_FATAL("Aborting test - Failed to create monitor thread");
    }
    if (pthread_join(thread, &ret_value) != 0) 
    {
        UT_LOG_INFO("Error joining monitor thread");
    }
    if (ret_value != NULL) 
    {
        result = *(rmf_Error *)ret_value;
        UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    
        free(ret_value);
    } else 
    {
        UT_LOG_INFO("Thread ret_value NULL, unable to assert for jitter check. Refer prints to confirm if test passed");
    }

    /* Validate if acceptable level of bytes received */
    result = validateBytesReceived(&settings, (void *)&ctx, MEASUREMENT_WINDOW_2MINUTES);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Stop(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(ctx.cookie, 0);

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Close(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the auxiliary audio capture functionality
*
* This test verifies the complete flow of auxiliary audio capture including opening the capture,
* getting default settings, modifying settings, starting capture, capturing audio, 
* writing it to a file for validation, stopping capture, and closing the capture. 
* It ensures that each step returns the expected success status.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_auxiliary_data_check(void)
{
    RMF_AudioCaptureHandle handle;
    RMF_AudioCapture_Settings settings;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:handle:[])", RMF_AC_TYPE_AUXILIARY);
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_AUXILIARY, &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:settings:[0x%0X]) rmf_error:[%s]", &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    capture_session_context_t ctx = {0, 0, 0};
    test_l3_prepare_start_settings_for_data_tracking(&settings, (void *)&ctx);

    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X])", &handle, &settings);
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &handle, &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(handle);
        if(ctx.data_buffer) 
        {
            free(ctx.data_buffer);
            ctx.data_buffer = NULL;
        }
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }
    sleep(MEASUREMENT_WINDOW_SECONDS);
    UT_LOG_INFO("Started audio capture, wait for %d seconds ", MEASUREMENT_WINDOW_SECONDS);

    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Stop(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(ctx.cookie, 0);

    result = test_l3_write_wav_file(&settings, (void *)&ctx, "/tmp/output_auxiliary.wav");
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Close(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test jitter for auxiliary audio capture
*
* This test verifies the primary audio capture functionality by opening an audio capture handle,
* getting default settings, modifying settings, starting the capture, capturing audio for a duration,
* stopping the capture, and finally closing the handle. The test verifies that jitter is low 
* enough to avoid underruns when compared against a given threshold.
* The test ensures that each step returns the expected success status and that 
* the callbacks are invoked correctly. 
*
* **Test Group ID:** 03@n
* **Test Case ID:** 004@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_auxiliary_jitter_check(void)
{
    RMF_AudioCaptureHandle handle;
    RMF_AudioCapture_Settings settings;
    rmf_Error result = RMF_SUCCESS;
    pthread_t thread;
    void *ret_value;

    gTestID = 4;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:handle:[])", RMF_AC_TYPE_AUXILIARY);
    result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_AUXILIARY, &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:settings:[0x%0X]) rmf_error:[%s]", &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    capture_session_context_t ctx = {0, 0, 0};
    test_l3_prepare_start_settings_for_data_counting(&settings, (void *)&ctx);

    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X])", &handle, &settings);
    result = RMF_AudioCapture_Start(handle, &settings);
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &handle, &settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }
    usleep(MONITOR_JITTER_MICROSECONDS);

    UT_LOG_INFO("Monitoring buffers for jitter, maximum wait time is %d seconds ", MEASUREMENT_WINDOW_2MINUTES);
    // Create the thread
    if (pthread_create(&thread, NULL, monitorBufferCount, (void *)&ctx) != 0) 
    {
        result = RMF_INVALID_PARM;
        UT_FAIL_FATAL("Aborting test - Failed to create monitor thread");
    }
    if (pthread_join(thread, &ret_value) != 0) 
    {
        UT_LOG_INFO("Error joining monitor thread");
    }
    // Check return value to determine if the thread failed
    if (ret_value != NULL) 
    {
        result = *(rmf_Error *)ret_value;
        UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    
        free(ret_value);
    } else 
    {
        UT_LOG_INFO("Thread ret_value NULL, unable to assert for jitter check. Refer prints to confirm if test passed");
    }

    /* Validate if acceptable level of bytes received */
    result = validateBytesReceived(&settings, (void *)&ctx, MEASUREMENT_WINDOW_2MINUTES);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Stop(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(ctx.cookie, 0);

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:handle:[0x%0X])", &handle);
    result = RMF_AudioCapture_Close(handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:handle:[0x%0X] OUT:rmf_error:[%s]", &handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the functionality of running auxiliary and primary audio capture simultaneously
*
* This test verifies the ability to open, start, capture, and stop audio capture on 
* both primary and auxiliary interfaces. It ensures that the audio capture handles are 
* correctly opened, the default settings are retrieved and modified, and audio capture 
* is successfully started and stopped. The test also verifies that the appropriate 
* callbacks are invoked and no more callbacks are issued after stopping the capture.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 005@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_combined_data_check(void)
{
    RMF_AudioCaptureHandle aux_handle, prim_handle;
    RMF_AudioCapture_Settings aux_settings, prim_settings;
    rmf_Error result = RMF_SUCCESS;

    gTestID = 5;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:aux_handle:[])", RMF_AC_TYPE_AUXILIARY);
    result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:aux_handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_AUXILIARY, &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:aux_settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&aux_settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:aux_settings:[0x%0X]) rmf_error:[%s]", &aux_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    prim_settings = aux_settings;
    capture_session_context_t prim_ctx = {0, 0, 0};
    capture_session_context_t aux_ctx = {0, 0, 0};
    test_l3_prepare_start_settings_for_data_tracking(&aux_settings, (void *)&aux_ctx);
    test_l3_prepare_start_settings_for_data_tracking(&prim_settings, (void *)&prim_ctx);

    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:aux_handle[0x%0X] settings:[0x%0X])", &aux_handle, &aux_settings);
    result = RMF_AudioCapture_Start(aux_handle, &aux_settings); // Started auxiliary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:aux_handle[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(aux_handle);
        if(aux_ctx.data_buffer) 
        {
            free(aux_ctx.data_buffer);
            aux_ctx.data_buffer = NULL;
        }
        if(prim_ctx.data_buffer) 
        {
            free(prim_ctx.data_buffer);
            prim_ctx.data_buffer = NULL;
        }
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:prim_handle:[])", RMF_AC_TYPE_PRIMARY);
    result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:prim_handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_PRIMARY, &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to open primary capture interface. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        if(aux_ctx.data_buffer) 
        {
            free(aux_ctx.data_buffer);
            aux_ctx.data_buffer = NULL;
        }
        if(prim_ctx.data_buffer) 
        {
            free(prim_ctx.data_buffer);
            prim_ctx.data_buffer = NULL;
        }
        UT_FAIL_FATAL("Aborting test - unable to open primary capture interface.");
    }
    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X])", &prim_handle, &prim_settings);
    result = RMF_AudioCapture_Start(prim_handle, &prim_settings); // Started primary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, &prim_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to start primary capture. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        result = RMF_AudioCapture_Close(prim_handle);
        if(aux_ctx.data_buffer) 
        {
            free(aux_ctx.data_buffer);
            aux_ctx.data_buffer = NULL;
        }
        if(prim_ctx.data_buffer) 
        {
            free(prim_ctx.data_buffer);
            prim_ctx.data_buffer = NULL;
        }
        UT_FAIL_FATAL("Aborting test - unable to start primary capture.");
    }
    UT_LOG_INFO("Started audio capture, wait for %d seconds ", MEASUREMENT_WINDOW_SECONDS);
    sleep(MEASUREMENT_WINDOW_SECONDS);

    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X])", &prim_handle);
    result = RMF_AudioCapture_Stop(prim_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    prim_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X])", &aux_handle);
    result = RMF_AudioCapture_Stop(aux_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    aux_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(prim_ctx.cookie, 0);
    UT_ASSERT_EQUAL(aux_ctx.cookie, 0);

    result = test_l3_write_wav_file(&aux_settings, (void *)&aux_ctx, "/tmp/output_combined_auxiliary.wav");
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = test_l3_write_wav_file(&prim_settings, (void *)&prim_ctx, "/tmp/output_combined_primary.wav");
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:prim_handle:[0x%0X])", &prim_handle);
    result = RMF_AudioCapture_Close(prim_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:prim_handle:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:aux_handle:[0x%0X])", &aux_handle);
    result = RMF_AudioCapture_Close(aux_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:aux_handle:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the functionality of running auxiliary and primary audio capture independently
*
* This test verifies the ability to open, start, capture, and stop audio capture on 
* both primary and auxiliary interfaces. It ensures that the audio capture handles are 
* correctly opened, the default settings are retrieved and modified, and audio capture 
v* is successfully started and stopped. The test also verifies that the primary and
* auxiliary captures work independently, stopping one does not stop the other.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 006@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_independent_data_check(void)
{
    RMF_AudioCaptureHandle aux_handle, prim_handle;
    RMF_AudioCapture_Settings aux_settings, prim_settings;
    rmf_Error result = RMF_SUCCESS;
    uint32_t primary_bytes_received = 0;
    uint32_t auxiliary_bytes_received = 0;

    gTestID = 6;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:aux_handle:[])", RMF_AC_TYPE_AUXILIARY);
    result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:aux_handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_AUXILIARY, &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:aux_settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&aux_settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:aux_settings:[0x%0X]) rmf_error:[%s]", &aux_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    prim_settings = aux_settings;
    capture_session_context_t prim_ctx = {0, 0, 0};
    capture_session_context_t aux_ctx = {0, 0, 0};
    test_l3_prepare_start_settings_for_data_counting(&aux_settings, (void *)&aux_ctx);
    test_l3_prepare_start_settings_for_data_counting(&prim_settings, (void *)&prim_ctx);

    /* Primary : Not Started | Auxiliary : Started */
    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:aux_handle[0x%0X] settings:[0x%0X])", &aux_handle, &aux_settings);
    result = RMF_AudioCapture_Start(aux_handle, &aux_settings); // Started auxiliary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:aux_handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, &aux_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(aux_handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:prim_handle:[])", RMF_AC_TYPE_PRIMARY);
    result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:prim_handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_PRIMARY, &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to open primary capture interface. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        UT_FAIL_FATAL("Aborting test - unable to open primary capture interface.");
    }
    /* Primary : Started | Auxiliary : Running */
    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X])", &prim_handle, &prim_settings);
    result = RMF_AudioCapture_Start(prim_handle, &prim_settings); // Started primary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, &prim_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to start primary capture. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        result = RMF_AudioCapture_Close(prim_handle);
        UT_FAIL_FATAL("Aborting test - unable to start primary capture.");
    }
    sleep(WAIT_WINDOW_SECONDS);
    primary_bytes_received = prim_ctx.bytes_received;
    auxiliary_bytes_received = aux_ctx.bytes_received;

    /* Primary : Stopped | Auxiliary : Running */
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X])", &prim_handle);
    result = RMF_AudioCapture_Stop(prim_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    prim_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_LOG_INFO("Primary stopped, auxiliary running. Verifying if bytes received for auxiliary has increased.\n");
    if ((prim_ctx.bytes_received != primary_bytes_received) && (aux_ctx.bytes_received < auxiliary_bytes_received))
    {
        result = RMF_ERROR;
    }
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    /* Primary : Stopped | Auxiliary : Stopped */
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X])", &aux_handle);
    result = RMF_AudioCapture_Stop(aux_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    aux_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed

    UT_ASSERT_EQUAL(prim_ctx.cookie, 0);
    UT_ASSERT_EQUAL(aux_ctx.cookie, 0);

    /* Primary : Started | Auxiliary : Not started */
    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X])", &prim_handle, &prim_settings);
    result = RMF_AudioCapture_Start(prim_handle, &prim_settings); // Started primary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, &prim_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to start primary capture. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        result = RMF_AudioCapture_Close(prim_handle);
        UT_FAIL_FATAL("Aborting test - unable to start primary capture.");
    }

    /* Primary : Running | Auxiliary : Started */
    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:aux_handle[0x%0X] settings:[0x%0X])", &aux_handle, &aux_settings);
    result = RMF_AudioCapture_Start(aux_handle, &aux_settings); // Started auxiliary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:aux_handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, &aux_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(aux_handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }
    sleep(WAIT_WINDOW_SECONDS);
    primary_bytes_received = prim_ctx.bytes_received;
    auxiliary_bytes_received = aux_ctx.bytes_received;

    /* Primary : Running | Auxiliary : Stopped */
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X])", &aux_handle);
    result = RMF_AudioCapture_Stop(aux_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    aux_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_LOG_INFO("Primary running, auxiliary stopped. Verifying if bytes received for primary has increased.\n");
    if ((prim_ctx.bytes_received < primary_bytes_received) && (aux_ctx.bytes_received != auxiliary_bytes_received))
    {
        result = RMF_ERROR;
    }
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    /* Primary : Stopped | Auxiliary : Stopped */
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X])", &prim_handle);
    result = RMF_AudioCapture_Stop(prim_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    aux_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:prim_handle:[0x%0X])", &prim_handle);
    result = RMF_AudioCapture_Close(prim_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:prim_handle:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:aux_handle:[0x%0X])", &aux_handle);
    result = RMF_AudioCapture_Close(aux_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:aux_handle:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the functionality of running auxiliary and primary audio capture simultaneously
*
* This test verifies the ability to open, start, capture, and stop audio capture on 
* both primary and auxiliary interfaces. It ensures that the audio capture handles are 
* correctly opened, the default settings are retrieved and modified, and audio capture 
* is successfully started and stopped. The test verifies that jitter is low 
* enough to avoid underruns when compared against a given threshold.
*
* **Test Group ID:** 03@n
* **Test Case ID:** 007@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmf-audio-capture_L2-Low-Level_TestSpecification.md](../docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)
*/
void test_l3_rmfAudioCapture_combined_jitter_check(void)
{
    RMF_AudioCaptureHandle aux_handle, prim_handle;
    RMF_AudioCapture_Settings aux_settings, prim_settings;
    rmf_Error result = RMF_SUCCESS;
    pthread_t primary_thread, aux_thread;
    void *prim_ret_value, *aux_ret_value;

    gTestID = 7;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:aux_handle:[])", RMF_AC_TYPE_AUXILIARY);
    result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:aux_handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_AUXILIARY, &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_FAIL_FATAL("Aborting test - unable to open capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_GetDefaultSettings(OUT:aux_settings:[])");
    result = RMF_AudioCapture_GetDefaultSettings(&aux_settings);
    UT_LOG_INFO("Result RMF_AudioCapture_GetDefaultSettings(OUT:aux_settings:[0x%0X]) rmf_error:[%s]", &aux_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    prim_settings = aux_settings;
    capture_session_context_t prim_ctx = {0, 0, 0};
    capture_session_context_t aux_ctx = {0, 0, 0};
    test_l3_prepare_start_settings_for_data_counting(&aux_settings, (void *)&aux_ctx);
    test_l3_prepare_start_settings_for_data_counting(&prim_settings, (void *)&prim_ctx);

    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:aux_handle[0x%0X] settings:[0x%0X])", &aux_handle, &aux_settings);
    result = RMF_AudioCapture_Start(aux_handle, &aux_settings); // Started auxiliary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:aux_handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, &aux_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Capture start failed with error code: %d", result);
        result = RMF_AudioCapture_Close(aux_handle);
        UT_FAIL_FATAL("Aborting test - unable to start capture.");
    }

    UT_LOG_INFO("Calling RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:prim_handle:[])", RMF_AC_TYPE_PRIMARY);
    result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
    UT_LOG_INFO("Result RMF_AudioCapture_Open_Type(IN:captureType:[%s] OUT:prim_handle:[0x%0X]) rmf_error:[%s]", RMF_AC_TYPE_PRIMARY, &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to open primary capture interface. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        UT_FAIL_FATAL("Aborting test - unable to open primary capture interface.");
    }
    UT_LOG_INFO("Calling RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X])", &prim_handle, &prim_settings);
    result = RMF_AudioCapture_Start(prim_handle, &prim_settings); // Started primary capture
    UT_LOG_INFO("Result RMF_AudioCapture_Start(IN:prim_handle[0x%0X] settings:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, &prim_settings, UT_Control_GetMapString(rmfError_mapTable, result));
    if (RMF_SUCCESS != result)
    {
        UT_LOG_DEBUG("Aborting test - unable to start primary capture. Error code: %d", result);
        result = RMF_AudioCapture_Stop(aux_handle);
        result = RMF_AudioCapture_Close(aux_handle);
        result = RMF_AudioCapture_Close(prim_handle);
        UT_FAIL_FATAL("Aborting test - unable to start primary capture.");
    }
    usleep(MONITOR_JITTER_MICROSECONDS);

    UT_LOG_INFO("Monitoring buffers for jitter, maximum wait time is %d seconds ", MEASUREMENT_WINDOW_2MINUTES);
    if (pthread_create(&primary_thread, NULL, monitorBufferCount, (void *)&prim_ctx) != 0) 
    {
        UT_FAIL_FATAL("Aborting test - Failed to create monitor thread for primary capture");
        result = RMF_INVALID_PARM;
    }
    if (pthread_create(&aux_thread, NULL, monitorBufferCount, (void *)&aux_ctx) != 0) 
    {
        UT_FAIL_FATAL("Aborting test - Failed to create monitor thread for auxiliary capture");
        result = RMF_INVALID_PARM;
    }

    if (pthread_join(primary_thread, &prim_ret_value) != 0) 
    {
        UT_LOG_INFO("Error joining primary capture monitor thread");
    }
    if (prim_ret_value != NULL) 
    {
        result = *(rmf_Error *)prim_ret_value;
        UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    
        free(prim_ret_value);
    } else 
    {
        UT_LOG_INFO("Thread ret_value NULL, unable to assert for jitter check. Refer prints to confirm if test passed");
    }

    if (pthread_join(aux_thread, &aux_ret_value) != 0) 
    {
        UT_LOG_INFO("Error joining auxiliary capture monitor thread");
    }
    if (aux_ret_value != NULL) 
    {
        result = *(rmf_Error *)aux_ret_value;
        UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    
        free(aux_ret_value);
    } else 
    {
        UT_LOG_INFO("Thread ret_value NULL, unable to assert for jitter check. Refer prints to confirm if test passed");
    }

    /* Validate if acceptable level of bytes received */
    result = validateBytesReceived(&prim_settings, (void *)&prim_ctx, MEASUREMENT_WINDOW_2MINUTES);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    result = validateBytesReceived(&aux_settings, (void *)&aux_ctx, MEASUREMENT_WINDOW_2MINUTES);
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X])", &prim_handle);
    result = RMF_AudioCapture_Stop(prim_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:prim_handle:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    prim_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    UT_LOG_INFO("Calling RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X])", &aux_handle);
    result = RMF_AudioCapture_Stop(aux_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Stop(IN:aux_handle:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    aux_ctx.cookie = 0;
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);

    sleep(1); // Wait for the last callback to be processed
    UT_ASSERT_EQUAL(prim_ctx.cookie, 0);
    UT_ASSERT_EQUAL(aux_ctx.cookie, 0);

    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:prim_handle:[0x%0X])", &prim_handle);
    result = RMF_AudioCapture_Close(prim_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:prim_handle:[0x%0X] OUT:rmf_error:[%s]", &prim_handle, UT_Control_GetMapString(rmfError_mapTable, result));
    UT_ASSERT_EQUAL(result, RMF_SUCCESS);
    UT_LOG_INFO("Calling RMF_AudioCapture_Close(IN:aux_handle:[0x%0X])", &aux_handle);
    result = RMF_AudioCapture_Close(aux_handle);
    UT_LOG_INFO("Result RMF_AudioCapture_Close(IN:aux_handle:[0x%0X] OUT:rmf_error:[%s]", &aux_handle, UT_Control_GetMapString(rmfError_mapTable, result));

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_rmfAudioCapture_l3_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L3 rmfAudioCapture]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings
    UT_add_test(pSuite, "test_l3_rmfAudioCapture_primary_data_check", test_l3_rmfAudioCapture_primary_data_check);
    UT_add_test(pSuite, "test_l3_rmfAudioCapture_primary_jitter_check", test_l3_rmfAudioCapture_primary_jitter_check);   
    g_aux_capture_supported = ut_kvp_getBoolField(ut_kvp_profile_getInstance(), "rmfaudiocapture/features/auxsupport");
    if (true == g_aux_capture_supported)
    {
        UT_add_test(pSuite, "test_l3_rmfAudioCapture_auxiliary_data_check", test_l3_rmfAudioCapture_auxiliary_data_check);
        UT_add_test(pSuite, "test_l3_rmfAudioCapture_auxiliary_jitter_check", test_l3_rmfAudioCapture_auxiliary_jitter_check);

        UT_add_test(pSuite, "test_l3_rmfAudioCapture_combined_data_check", test_l3_rmfAudioCapture_combined_data_check);
        UT_add_test(pSuite, "test_l3_rmfAudioCapture_independent_data_check", test_l3_rmfAudioCapture_independent_data_check);
        UT_add_test(pSuite, "test_l3_rmfAudioCapture_combined_jitter_check", test_l3_rmfAudioCapture_combined_jitter_check);
    }
    
    return 0;
}

/** @} */ // End of RMF Audio Capture HAL Tests L2 File
/** @} */ // End of RMF Audio Capture HAL Tests
/** @} */ // End of RMF Audio Capture Module
/** @} */ // End of HPK
