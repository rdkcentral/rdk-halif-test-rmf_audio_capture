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
 * @addtogroup HPK HPK
 * @{
 *
 */

/**
 * @addtogroup RMF_AudioCapture RMF Audio Capture
 * @{
 *
 */

/**
 * @addtogroup RMF Audio Capture Tests
 * @{
 *
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

#define MEASUREMENT_WINDOW_SECONDS 10
extern void prepare_dummy_start_settings(RMF_AudioCapture_Settings * settings);
extern rmf_Error dummy_data_cb(void *cbBufferReadyParm, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize);
extern rmf_Error validate_status_active(RMF_AudioCapture_Status * status);
extern rmf_Error compare_settings(RMF_AudioCapture_Settings * left, RMF_AudioCapture_Settings * right);

static rmf_Error counting_data_cb(void *context_blob, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
	uint64_t * counter = (uint64_t *) context_blob;
	*counter += AudioCaptureBufferSize;
	return RMF_SUCCESS;
}

static void prepare_start_settings_for_data_tracking(RMF_AudioCapture_Settings * settings, void * context_blob)
{
	settings->cbBufferReady = counting_data_cb;
	settings->cbStatusChange = NULL;
	settings->cbBufferReadyParm = context_blob;
}

static rmf_Error validate_bytes_received(RMF_AudioCapture_Settings * settings, uint32_t seconds, uint64_t bytes_received)
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
		default: //Unsupported format
			return RMF_ERROR;
	}

	uint64_t computed_bytes_received = seconds * num_channels * sampling_rate * bits_per_sample / 8;
	uint64_t percentage_received = bytes_received / computed_bytes_received * 100;
	if((90 <= percentage_received) && (110 >= percentage_received))
		return RMF_SUCCESS;
	else
		return RMF_ERROR;
}

/**
* @brief TODO: Describe the object of the test
*
* TODO: Add the description of what is tested and why in this test
*
* **Test Group ID:** TODO: Add the group this test belongs to - Basic (for L1): 01 / Module (L2): 02 / Stress (L2): 03)@n
* **Test Case ID:** TODO: Add the ID of the test case so that it can be logically tracked in the logs@n
*
* **Test Procedure:**
* Refer to UT specification documentation [l2_module_test_specification.md](l2_module_test_specification.md)
*/
void test_l2_rmfAudioCapture (void)
{
	UT_FAIL(This function needs to be implemented!); 
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

	memset(&settings, 0, sizeof(settings)); // To fill padding bytes with zero, allows comparison of structs with memcmp.
	memset(&current_settings, 0, sizeof(current_settings));

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	UT_ASSERT_EQUAL(0, status.started);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(aux_handle, &settings); //Started auxiliary audio capture.
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Start(aux_handle, &settings); //Started primary audio capture
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	result = validate_status_active(&status);
	UT_ASSERT_EQUAL(0, result);

	result = RMF_AudioCapture_GetStatus (prim_handle, &status);
	result = validate_status_active(&status);
	UT_ASSERT_EQUAL(0, result);

	result = RMF_AudioCapture_GetCurrentSettings(aux_handle, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetCurrentSettings(prim_handle, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Stop(aux_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
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

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	uint64_t aux_bytes_received = 0;
	prepare_start_settings_for_data_tracking(&settings, (void *)&aux_bytes_received);
	
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	sleep(MEASUREMENT_WINDOW_SECONDS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = validate_bytes_received(&settings, MEASUREMENT_WINDOW_SECONDS, aux_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
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

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	uint64_t prim_bytes_received = 0;
	prepare_start_settings_for_data_tracking(&settings, (void *)&prim_bytes_received);
	
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	sleep(MEASUREMENT_WINDOW_SECONDS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = validate_bytes_received(&settings, MEASUREMENT_WINDOW_SECONDS, prim_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
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

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetDefaultSettings(&aux_settings);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	uint64_t aux_bytes_received = 0;
	uint64_t prim_bytes_received = 0;
	prim_settings = aux_settings;
	prepare_start_settings_for_data_tracking(&aux_settings, (void *)&aux_bytes_received);
	prepare_start_settings_for_data_tracking(&prim_settings, (void *)&prim_bytes_received);
	
	result = RMF_AudioCapture_Start(aux_handle, &aux_settings); //Started auxiliary capture
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);


	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Start(prim_handle, &prim_settings); //Started primary capture
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	sleep(MEASUREMENT_WINDOW_SECONDS);

	result = RMF_AudioCapture_Stop(prim_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(aux_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = validate_bytes_received(&aux_settings, MEASUREMENT_WINDOW_SECONDS, aux_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = validate_bytes_received(&prim_settings, MEASUREMENT_WINDOW_SECONDS, prim_bytes_received);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(prim_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(aux_handle);
	UT_ASSERT_EQUAL(result, RMF_SUCCESS)
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

	
	UT_add_test( pSuite, "test_l2_rmfAudioCapture_simultaneous_sessions" ,test_l2_rmfAudioCapture_simultaneous_sessions );
	UT_add_test( pSuite, "test_l2_rmfAudioCapture_primary_data_check" ,test_l2_rmfAudioCapture_primary_data_check );
	UT_add_test( pSuite, "test_l2_rmfAudioCapture_auxiliary_data_check" ,test_l2_rmfAudioCapture_auxiliary_data_check );
	UT_add_test( pSuite, "test_l2_rmfAudioCapture_combined_data_check" ,test_l2_rmfAudioCapture_combined_data_check );

	return 0;
} 

/** @} */ // End of RMF AudioCapture Tests
/** @} */ // End of RMF_AudioCapture RMF AudioCapture
/** @} */ // End of HPK
