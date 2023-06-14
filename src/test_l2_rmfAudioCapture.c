/**
*  If not stated otherwise in this file or this component's Licenses.txt
*  file the following copyright and licenses apply:
*
*  Copyright 2022 RDK Management
*
*  Licensed under the Apache License, Version 2.0 (the License);
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an AS IS BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

/**
 * @addtogroup RMF_AudioCapture RDK-V RMF AudioCapture
 * @{
 */

/**
 * @addtogroup RDK-V RMF AudioCapture Tests
 * @ingroup RMF_AudioCapture RDK-V RMF AudioCapture
 * @{
 */

/**
* @file test_l2_rmfAudioCapture.c
*
* ## Module's Role
* Level 2 module tests will perform module level testing by exercising the full set of APIs to validate
* various use cases. This is to ensure that the API meets the operational requirements of the module across all vendors.
*
* **Pre-Conditions:**  Launch video playback before commencing tests.@n
* **Dependencies:** None@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ut.h>
#include <ut_log.h>
#include "rmfAudioCapture.h"
#include "test_lx_rmfAudioCapture_helper.h"


#define MEASUREMENT_WINDOW_SECONDS 10

static int gTestGroup = 2;
static int gTestID = 1;

extern void test_l1_prepare_dummy_start_settings(RMF_AudioCapture_Settings * settings);
extern rmf_Error test_l1_dummy_data_cb(void *cbBufferReadyParm, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize);
extern rmf_Error test_l1_validate_status_active(RMF_AudioCapture_Status * status);
extern rmf_Error test_l1_compare_settings(RMF_AudioCapture_Settings * left, RMF_AudioCapture_Settings * right);
extern bool test_l1_is_aux_capture_supported();

static rmf_Error test_l2_counting_data_cb(void *context_blob, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
	uint64_t * counter = (uint64_t *) context_blob;
	*counter += AudioCaptureBufferSize;
	return RMF_SUCCESS;
}

static void test_l2_prepare_start_settings_for_data_tracking(RMF_AudioCapture_Settings * settings, void * context_blob)
{
	settings->cbBufferReady = test_l2_counting_data_cb;
	settings->cbStatusChange = NULL;
	settings->cbBufferReadyParm = context_blob;
}

static rmf_Error test_l2_validate_bytes_received(RMF_AudioCapture_Settings * settings, uint32_t seconds, uint64_t bytes_received)
{
	uint8_t num_channels = 0;
	uint32_t sampling_rate = 0;
	uint8_t bits_per_sample = 0;
	switch(settings->format)
	{
		case racFormat_e16BitStereo:
			bits_per_sample = 16; num_channels = 2; break;
		case racFormat_e24BitStereo:
			bits_per_sample = 24; num_channels = 2; break;
		case racFormat_e16BitMonoLeft: //fall through
		case racFormat_e16BitMonoRight: //fall through
		case racFormat_e16BitMono:
			bits_per_sample = 16; num_channels = 1; break;
		case racFormat_e24Bit5_1:
			bits_per_sample = 24; num_channels = 6; break;
		default: //Unsupported format
			UT_LOG("Error: Invalid format detected.\n");
			return RMF_ERROR;
	}
	

	switch(settings->samplingFreq)
	{
		case racFreq_e16000:
			sampling_rate = 16000; break;
		case racFreq_e22050:
			sampling_rate = 22050; break;
		case racFreq_e24000:
			sampling_rate = 24000; break;
		case racFreq_e32000:
			sampling_rate = 32000; break;
		case racFreq_e44100:
			sampling_rate = 44100; break;
		case racFreq_e48000:
			sampling_rate = 48000; break;
		default: //unsupported sampling rate
			UT_LOG("Error: Invalid samping rate detected.\n");
			return RMF_ERROR;
	}

	uint64_t computed_bytes_received = seconds * num_channels * sampling_rate * bits_per_sample / 8;
	double percentage_received = (double)bytes_received / (double)computed_bytes_received * 100;
	UT_LOG("Actual bytes received: %" PRIu64 ", Expected bytes received: %" PRIu64 ", Computed percentage: %f\n",
		bytes_received, computed_bytes_received, percentage_received);
	if((90.0 <= percentage_received) && (110.0 >= percentage_received))
		return RMF_SUCCESS;
	else
	{	
		UT_LOG("Error: data delivery does not meet tolerance!");
		return RMF_ERROR;
	}
}

/**
* @brief Test simultaneous primary and auxiliary audio capture
*
* Launch simultaneous capture sessions of primary and auxiliary audio and verify that status and settings are returned correctly
*
* **Test Group ID:** Module (L2): 02@n
* **Test Case ID:** 1@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_module_test_specification.md](l2_module_test_specification.md)
*/
void test_l2_rmfAudioCapture_simultaneous_sessions (void)
{
	RMF_AudioCaptureHandle aux_handle, prim_handle;
	RMF_AudioCapture_Status status;
	RMF_AudioCapture_Settings settings, current_settings;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 1;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	memset(&settings, 0, sizeof(settings)); // To fill padding bytes with zero, allows comparison of structs with memcmp.
	memset(&current_settings, 0, sizeof(current_settings));

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	VALIDATE_SUCCESSFUL_OPEN(result);

	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	UT_ASSERT_EQUAL(0, status.started);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(aux_handle, &settings); //Started auxiliary audio capture.
	VALIDATE_SUCCESSFUL_START(result, aux_handle);

	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Aborting test - unable to open primary capture interface. Error code: %d", result);
		result = RMF_AudioCapture_Stop(aux_handle);
		result = RMF_AudioCapture_Close(aux_handle);
		UT_FAIL_FATAL("Aborting test - unable to open primary capture interface");
	}
	result = RMF_AudioCapture_Start(prim_handle, &settings); //Started primary audio capture
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Aborting test - unable to start primary capture. Error code: %d", result);
		result = RMF_AudioCapture_Stop(aux_handle);
		result = RMF_AudioCapture_Close(aux_handle);
		result = RMF_AudioCapture_Close(prim_handle);
		UT_FAIL_FATAL("Aborting test - unable to open primary capture interface");
	}

	usleep(100 * 1000); // Small delay before checking status so that the internal structures have a chance to update.

	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	result = test_l1_validate_status_active(&status);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetStatus (prim_handle, &status);
	result = test_l1_validate_status_active(&status);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetCurrentSettings(aux_handle, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = test_l1_compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetCurrentSettings(prim_handle, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = test_l1_compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Stop(aux_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	usleep(100 * 1000); // Small delay before checking status so that the internal structures have a chance to update.

	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	UT_ASSERT_EQUAL(0, status.started);

	result = RMF_AudioCapture_Stop(prim_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus (prim_handle, &status);
	UT_ASSERT_EQUAL(0, status.started);

	result = RMF_AudioCapture_Close(prim_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(aux_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Ensure satisfactory data transfer for auxiliary capture
*
* Start capture of auxiliary audio and verify that HAL triggers data callback with enough frequency and
* data in order to keep up with the expected data rate, tested over 10 seconds
*
* **Test Group ID:** Module (L2): 02@n
* **Test Case ID:** 2@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_module_test_specification.md](l2_module_test_specification.md)
*/
void test_l2_rmfAudioCapture_auxiliary_data_check (void)
{
	RMF_AudioCaptureHandle handle;
	RMF_AudioCapture_Settings settings;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 2;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
	VALIDATE_SUCCESSFUL_OPEN(result);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	uint64_t aux_bytes_received = 0;
	test_l2_prepare_start_settings_for_data_tracking(&settings, (void *)&aux_bytes_received);
	
	result = RMF_AudioCapture_Start(handle, &settings);
	VALIDATE_SUCCESSFUL_START(result, handle);

	sleep(MEASUREMENT_WINDOW_SECONDS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = test_l2_validate_bytes_received(&settings, MEASUREMENT_WINDOW_SECONDS, aux_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	UT_LOG("\n Out  %s\n",__FUNCTION__);
}



/**
* @brief Ensure satisfactory data transfer for primary capture
*
* Start capture of primary audio and verify that HAL triggers data callback with enough frequency and
* data in order to keep up with the expected data rate, tested over 10 seconds
*
* **Test Group ID:** Module (L2): 02@n
* **Test Case ID:** 3@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_module_test_specification.md](l2_module_test_specification.md)
*/
void test_l2_rmfAudioCapture_primary_data_check (void)
{
	RMF_AudioCaptureHandle handle;
	RMF_AudioCapture_Settings settings;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 3;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	VALIDATE_SUCCESSFUL_OPEN(result);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	uint64_t prim_bytes_received = 0;
	test_l2_prepare_start_settings_for_data_tracking(&settings, (void *)&prim_bytes_received);
	
	result = RMF_AudioCapture_Start(handle, &settings);
	VALIDATE_SUCCESSFUL_START(result, handle);
	
	sleep(MEASUREMENT_WINDOW_SECONDS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = test_l2_validate_bytes_received(&settings, MEASUREMENT_WINDOW_SECONDS, prim_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	UT_LOG("\n Out  %s\n",__FUNCTION__);
}



/**
* @brief Ensure satisfactory data transfer for simultaneous primary and auxiliary capture
*
* Start capture of simultaneous primary and auxiliary capture and verify that HAL triggers data callback with enough frequency and
* data in order to keep up with the expected data rate, tested over 10 seconds
*
* **Test Group ID:** Module (L2): 02@n
* **Test Case ID:** 4@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_module_test_specification.md](l2_module_test_specification.md)
*/
void test_l2_rmfAudioCapture_combined_data_check (void)
{
	RMF_AudioCaptureHandle aux_handle, prim_handle;
	RMF_AudioCapture_Settings aux_settings, prim_settings;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 4;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	VALIDATE_SUCCESSFUL_OPEN(result);

	result = RMF_AudioCapture_GetDefaultSettings(&aux_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	uint64_t aux_bytes_received = 0;
	uint64_t prim_bytes_received = 0;
	prim_settings = aux_settings;
	test_l2_prepare_start_settings_for_data_tracking(&aux_settings, (void *)&aux_bytes_received);
	test_l2_prepare_start_settings_for_data_tracking(&prim_settings, (void *)&prim_bytes_received);
	
	result = RMF_AudioCapture_Start(aux_handle, &aux_settings); //Started auxiliary capture
	VALIDATE_SUCCESSFUL_START(result, aux_handle);

	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Aborting test - unable to open primary capture interface. Error code: %d", result);
		result = RMF_AudioCapture_Stop(aux_handle);
		result = RMF_AudioCapture_Close(aux_handle);
		UT_FAIL_FATAL("Aborting test - unable to open primary capture interface.");
	}
	result = RMF_AudioCapture_Start(prim_handle, &prim_settings); //Started primary capture
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Aborting test - unable to start primary capture. Error code: %d", result);
		result = RMF_AudioCapture_Stop(aux_handle);
		result = RMF_AudioCapture_Close(aux_handle);
		result = RMF_AudioCapture_Close(prim_handle);
		UT_FAIL_FATAL("Aborting test - unable to start primary capture.");
	}
	sleep(MEASUREMENT_WINDOW_SECONDS);

	result = RMF_AudioCapture_Stop(prim_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(aux_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = test_l2_validate_bytes_received(&aux_settings, MEASUREMENT_WINDOW_SECONDS, aux_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = test_l2_validate_bytes_received(&prim_settings, MEASUREMENT_WINDOW_SECONDS, prim_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(prim_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(aux_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS)

	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l2_rmfAudioCapture_register ( void )
{
	/* add a suite to the registry */
	pSuite = UT_add_suite( "[L2 rmfAudioCapture]", NULL, NULL );
	if ( NULL == pSuite )
	{
		return -1;
	}	

	UT_add_test( pSuite, "test_l2_rmfAudioCapture_primary_data_check" ,test_l2_rmfAudioCapture_primary_data_check );
	if(true == test_l1_is_aux_capture_supported())
	{
		UT_add_test( pSuite, "test_l2_rmfAudioCapture_simultaneous_sessions" ,test_l2_rmfAudioCapture_simultaneous_sessions );
		UT_add_test( pSuite, "test_l2_rmfAudioCapture_auxiliary_data_check" ,test_l2_rmfAudioCapture_auxiliary_data_check );
		UT_add_test( pSuite, "test_l2_rmfAudioCapture_combined_data_check" ,test_l2_rmfAudioCapture_combined_data_check );
	}
	return 0;
} 

/** @} */ // End of RMF_AudioCapture RDK-V RMF AudioCapture
