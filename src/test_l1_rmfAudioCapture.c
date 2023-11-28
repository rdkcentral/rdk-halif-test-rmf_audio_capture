/**
*  If not stated otherwise in this file or this component's LICENSE
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
 * @addtogroup HPK Hardware Porting Kit
 * @{
 *
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
 * @defgroup RMF_AudioCapture_HALTESTS_L1 RMF Audio Capture HAL Tests L1 File
 * @{
 * @parblock
 *
 * ### L1 Tests for RMF Audio Capture HAL :
 *
 * Level 1 unit test cases for all APIs of Audio Capture HAL
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
* @file test_l1_rmfAudioCapture.c
*
*/


#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ut.h>
#include <ut_log.h>
#include "rmfAudioCapture.h"

static int gTestGroup = 1;
static int gTestID = 1;

bool test_l1_is_aux_capture_supported()
{
	const char * config = getenv("AC_AUX_CAPTURE_SUPPORTED");
	if((NULL != config) && (0 == strncasecmp(config, "TRUE", 4)))
		return true;
	else
		return false;
}

static bool test_l1_create_suite_of_positive_tests()
{
	const char * config = getenv("AC_GENERATE_POSITIVE_ONLY_SUITE");
	if((NULL != config) && (0 == strncasecmp(config, "TRUE", 4)))
		return true;
	else
		return false;
}

rmf_Error test_l1_dummy_data_cb(void *cbBufferReadyParm, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
	return RMF_SUCCESS;
}

void test_l1_prepare_dummy_start_settings(RMF_AudioCapture_Settings * settings)
{
	settings->cbBufferReady = test_l1_dummy_data_cb;
	settings->cbStatusChange = NULL;
}

static rmf_Error test_l1_validate_settings(RMF_AudioCapture_Settings * settings)
{
	rmf_Error result = RMF_SUCCESS;

	if((racFormat_e16BitStereo  > settings->format) || (racFormat_eMax <= settings->format))
	{
		UT_LOG("Error: invalid format detected.");
		return RMF_ERROR;
	}

	if((racFreq_e16000  > settings->samplingFreq) || (racFreq_eMax <= settings->samplingFreq))
	{
		UT_LOG("Error: invalid samping frequency detected.");
		return RMF_ERROR;
	}
	return result;
}

rmf_Error test_l1_validate_status_active(RMF_AudioCapture_Status * status)
{
	rmf_Error result = RMF_SUCCESS;

	if(1 != status->started)
	{
		UT_LOG("Error: wrong status - started should be 1.");
		return RMF_ERROR;
	}

	if((racFormat_e16BitStereo  > status->format) || (racFormat_eMax <= status->format))
	{
		UT_LOG("Error: invalid format detected.");
		return RMF_ERROR;
	}

	if((racFreq_e16000  > status->samplingFreq) || (racFreq_eMax <= status->samplingFreq))
	{
		UT_LOG("Error: invalid samping frequency detected.");
		return RMF_ERROR;
	}
	return result;
}

rmf_Error test_l1_compare_settings(RMF_AudioCapture_Settings * left, RMF_AudioCapture_Settings * right)
{
	if(0 == memcmp(left, right, sizeof(RMF_AudioCapture_Settings)))
		return RMF_SUCCESS;
	else
	{
		UT_LOG("Error: settings mistmatch!");
		return RMF_ERROR;
	}
}

/**
* @brief Tests positive scenarios with primary audio for RMF_AudioCapture_Open_Type()
*
* Probe various input scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 001@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open_Type()` to check open->close->open sequence | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_primary (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 1;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	if(RMF_SUCCESS != result)
		UT_LOG("Failed open->close->open sequence.");

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios with primary audio for RMF_AudioCapture_Open_Type()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 002@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` with invalid handle | handle must be NULL, type=primary | RMF_INVALID_PARM | Should pass |
* | 02 | Call `RMF_AudioCapture_Open_Type()` with junk type and valid handle | handle is a valid pointer, type=xyz | RMF_INVALID_PARM | Should pass |
* | 04 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type=primary | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Open_Type()` when primary is already open | handle must be a valid pointer, type=primary | RMF_INVALID_STATE | Should pass |
* | 06 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/

void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_primary (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 2;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(NULL, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Open_Type(&handle, "junk");
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);
	if(RMF_INVALID_STATE != result)
		UT_LOG("Failed to reject a second primary capture request.");

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test positive scenarios for RMF_AudioCapture_Open()
*
* Probe various input scenarios for RMF_AudioCapture_Open() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 003@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open()` to check open->close->open sequence | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Open_Type()` to ensure open_type works after open | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Open()` to ensure open works after open_type | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 3;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	if(RMF_SUCCESS != result)
		UT_LOG("Failed open->close->open sequence.");
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);


	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Open()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 004@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` with invalid handle | handle must be NULL | RMF_INVALID_PARM | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open()` when already open | handle must be a valid pointer | RMF_INVALID_STATE | Should pass |
* | 04 | Call `RMF_AudioCapture_Open_Type()` when already open | handle must be a valid pointer, type is primary | RMF_INVALID_STATE | Should pass |
* | 05 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Open_Type()` to open primary capture | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Open()` when already open | handle must be a valid pointer | RMF_INVALID_STATE | Should pass |
* | 08 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 4;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);
	if(RMF_INVALID_STATE != result)
		UT_LOG("Failed to reject a second primary capture request.");
	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);
	if(RMF_INVALID_STATE != result)
		UT_LOG("Failed to reject a second primary capture request.");
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test simplified positive scenarios for RMF_AudioCapture_Close()
*
* Probe various input scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 005@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 5;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test complex positive scenarios for RMF_AudioCapture_Close()
*
* Probe various input scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 006@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` with old handle to confirm that it was invalidated | previous handle, settings=default settings from previous step, dummy data callback, status callback NULL | RMF_INVALID_HANDLE | Should pass |
* | 07 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Start()` to start audio capture | settings=default settings , but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 09 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Start()` to start audio capture | settings=default settings , but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 11 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 12 | Call `RMF_AudioCapture_Close()` to release resources, test open->start->stop->start->stop->close | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close_complex (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;

	gTestID = 6;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Capture start failed with error code: %d", result);
		result = RMF_AudioCapture_Close(handle);
		UT_FAIL("Aborting test - unable to start capture.");
	}

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test simple negative scenarios for RMF_AudioCapture_Close()
*
* Force various edge/error scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 007@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Close()` with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Close()` with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Close()` with outdated handle | handle obtained in last open call | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close (void)
{
	RMF_AudioCaptureHandle handle = NULL;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 7;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Close(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Close(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test complex negative scenarios for RMF_AudioCapture_Close()
*
* Force various edge/error scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 008@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` in STARTED state | current handle | RMF_INVALID_STATE | Should pass |
* | 05 | Call `RMF_AudioCapture_Stop()` to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close_complex (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;

	gTestID = 8;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Capture start failed with error code: %d", result);
		result = RMF_AudioCapture_Close(handle);
		UT_FAIL("Aborting test - unable to start capture.");
	}

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test simple positive scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Probe various input scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 009@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` with valid parameters and validate it | valid pointer for settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` again with valid parameters and validate it | valid pointer for settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` with current handle | Release resource after test | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;

	gTestID = 9;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test complex positive scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Probe various input scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 010@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` after start and validate settings | valid settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` to start with minimum viable settings | current handle, settings = default settings, but increase delayCompensation_ms by 1000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_GetDefaultSettings()` after start and validate settings | valid settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid, delayCompensation_ms must be the original value | Should pass |
* | 05 | Call `RMF_AudioCapture_Stop()` to stop the capture | current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_GetDefaultSettings()` after start and validate settings | valid settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid, delayCompensation_ms must be the original value | Should pass |
* | 07 | Call `RMF_AudioCapture_Close()` with current handle | Release resource after test | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings_complex (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;
	settings.delayCompensation_ms = 0; //Set baseline as 0 before calling getDefaultSettings.

	gTestID = 10;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	
	test_l1_prepare_dummy_start_settings(&settings);
	settings.delayCompensation_ms += 1000; // Increase delay compensation before start to introduce a deviation from the default parameters.
	
	result = RMF_AudioCapture_Start(handle, &settings);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Capture start failed with error code: %d", result);
		result = RMF_AudioCapture_Close(handle);
		UT_FAIL("Aborting test - unable to start capture.");
	}

	RMF_AudioCapture_Settings settings_v2;
	settings_v2.delayCompensation_ms = 0; //Set baseline as 0 before calling getDefaultSettings.
	result = RMF_AudioCapture_GetDefaultSettings(&settings_v2);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings_v2);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_ASSERT_FALSE(settings.delayCompensation_ms == settings_v2.delayCompensation_ms);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	settings_v2.delayCompensation_ms = 0; //Set baseline as 0 before calling getDefaultSettings.
	result = RMF_AudioCapture_GetDefaultSettings(&settings_v2);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings_v2);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_ASSERT_FALSE(settings.delayCompensation_ms == settings_v2.delayCompensation_ms);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test simple negative scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 011@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_GetDefaultSettings()` when not in open/started state | valid settings | RMF_INVALID_STATE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` with invalid parameters | settings=NULL | RMF_INVALID_PARM | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_GetDefaultSettings()` when not in open/started state | valid settings | RMF_INVALID_STATE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;

	gTestID = 11;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}
/**
* @brief Test complex negative scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 012@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` with valid parameters | valid pointer for settings | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` to start with minimum viable settings | current handle, settings = default settings, add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_GetDefaultSettings()` with current handle and invalid parameters | current handle, settings=NULL | RMF_INVALID_PARM | Should pass |
* | 05 | Call `RMF_AudioCapture_Stop()` to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_GetDefaultSettings()` with invalid parameters | settings=NULL | RMF_INVALID_PARM | Should pass |
* | 07 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/

void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings_complex (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;

	gTestID = 12;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	
	test_l1_prepare_dummy_start_settings(&settings); 
	
	result = RMF_AudioCapture_Start(handle, &settings);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Capture start failed with error code: %d", result);
		result = RMF_AudioCapture_Close(handle);
		UT_FAIL("Aborting test - unable to start capture.");
	}
	result = RMF_AudioCapture_GetDefaultSettings(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetDefaultSettings(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test simple positive scenarios for RMF_AudioCapture_Start()
*
* Probe various input scenarios for RMF_AudioCapture_Start() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 013@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` with caller-modified settings derived from above | settings - increase delayCompensation_ms by 2000, dummy data callback, empty status callback | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that passed parameters were set | valid handle | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 08 | Call `RMF_AudioCapture_Stop()` to stop audio capture | valid handle | RMF_SUCCESS | Should pass |
* | 09 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 11 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 12 | Call `RMF_AudioCapture_Start()` with default settings to check start after stop->close->open | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_Stop()` to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 14 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Start (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings, current_settings;
	memset(&settings, 0, sizeof(settings)); // To fill padding bytes with zero, allows comparison of structs with memcmp.
	memset(&current_settings, 0, sizeof(current_settings));

	gTestID = 13;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	
	test_l1_prepare_dummy_start_settings(&settings);
	
	result = RMF_AudioCapture_Start(handle, &settings);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Capture start failed with error code: %d", result);
		result = RMF_AudioCapture_Close(handle);
		UT_FAIL("Aborting test - unable to start capture.");
	}

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	settings.delayCompensation_ms += 2000;
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test negative scenarios for RMF_AudioCapture_Start()
*
* Force various edge/error scenarios for RMF_AudioCapture_Start() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 014@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Start()` with invalid handle | handle is NULL, settings is NULL | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Start()` with invalid handle and settings | handle is NULL, settings is NULL | RMF_INVALID_HANDLE | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` with invalid handle | handle is NULL, settings = settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_HANDLE | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` with invalid settings | current handle, settings is NULL | RMF_INVALID_PARM | Should pass |
* | 07 | Call `RMF_AudioCapture_Start()` with parameter outside bounds | current handle, settings = default settings, but format = racFormat_eMax | RMF_INVALID_PARM | Should pass |
* | 08 | Call `RMF_AudioCapture_Start()` with parameter outside bounds | current handle, settings = default settings, but samplingFreq = racFreq_eMax | RMF_INVALID_PARM | Should pass |
* | 09 | Call `RMF_AudioCapture_Start()` with parameter outside bounds | current handle, settings = default settings, but cbBufferReady = NULL | RMF_INVALID_PARM | Should pass |
* | 10 | Call `RMF_AudioCapture_Start()` with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 11 | Call `RMF_AudioCapture_Start()` again to test API response when already in STARTED state | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_STATE | Should pass |
* | 12 | Call `RMF_AudioCapture_Stop()` to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 14 | Call `RMF_AudioCapture_Start()` with outdated handle | handle obtained in last open call, settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Start (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings, bad_settings;

	gTestID = 14;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Start(NULL, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	// step 4
	result = RMF_AudioCapture_Start(NULL, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	
	result = RMF_AudioCapture_Start(NULL, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	result = RMF_AudioCapture_Start(handle, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	// Step 7
	bad_settings = settings;
	bad_settings.format = racFormat_eMax; // bad format
	result = RMF_AudioCapture_Start(handle, &bad_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	bad_settings = settings;
	bad_settings.samplingFreq = racFreq_eMax; // bad frequency
	result = RMF_AudioCapture_Start(handle, &bad_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	bad_settings = settings;
	bad_settings.cbBufferReady = NULL; // bad callback
	result = RMF_AudioCapture_Start(handle, &bad_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	// Step 11
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_Stop()
*
* Probe various input scenarios for RMF_AudioCapture_Stop() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 015@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` to restart defaults | settings=default settings , but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Stop()` to stop capture, tests start->stop->start->stop sequence | valid handle | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 09 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 11 | Call `RMF_AudioCapture_Stop()` to stop capture, tests start->stop->close->open->start->stop sequence | valid handle | RMF_SUCCESS | Should pass |
* | 12 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Stop (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;

	gTestID = 15;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	// Step 5
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	// Step 8
	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Stop()
*
* Force various edge/error scenarios for RMF_AudioCapture_Stop() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 016@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Stop()` with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Stop()` to test API response when not in STARTED state | current handle | RMF_INVALID_STATE | Should pass |
* | 04 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Stop()` with invalid handle | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 07 | Call `RMF_AudioCapture_Stop()` to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Stop()` repeatedly, violating state machine | current handle | RMF_INVALID_STATE | Should pass |
* | 09 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Stop()` with outdated handle | handle obtained in last open call | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Stop (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;

	gTestID = 16;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Stop(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	// Step 6
	result = RMF_AudioCapture_Stop(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Stop(NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_GetStatus()
*
* Probe various input scenarios for RMF_AudioCapture_GetStatus() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 017@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetStatus()` to check status of open interface | current handle | return RMF_SUCCESS, RMF_AudioCapture_Status.started must be 0 | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Start()` to start audio capture | current handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_GetStatus()` to check current status of started interface | current handle, valid settings | return RMF_SUCCESS, RMF_AudioCapture_Status.started must be 1, format and samplingFreq must have valid values | Should pass |
* | 06 | Call `RMF_AudioCapture_Stop()` to stop the interface | current handle | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_GetStatus()` to check current status of stopped/open interface | current handle, valid settings | return RMF_SUCCESS, RMF_AudioCapture_Status.started must be 0 | Should pass |
* | 08 | Call `RMF_AudioCapture_Close()` to release resources after test | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetStatus (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;
	RMF_AudioCapture_Status status;

	gTestID = 17;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetStatus (handle, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(0, status.started);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus (handle, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(1, status.started);
	result = test_l1_validate_status_active(&status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	//Step 6
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus (handle, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(0, status.started);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_GetStatus()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetStatus() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 018@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_GetStatus()` before open | handle=NULL | return RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetStatus()` with invalid handle after open | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 04 | Call `RMF_AudioCapture_GetStatus()` with valid handle and invalid status after open | current handle, status=NULL | RMF_INVALID_PARM | Should pass |
* | 05 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` to start audio capture | current handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_GetStatus()` with invalid handle after start | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 08 | Call `RMF_AudioCapture_GetStatus()` with valid handle and invalid status after start | current handle, status=NULL | RMF_INVALID_PARM | Should pass |
* | 09 | Call `RMF_AudioCapture_Stop()` to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_GetStatus()` with invalid handle after stop | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 11 | Call `RMF_AudioCapture_GetStatus()` with valid handle and invalid status after stop | current handle, status=NULL | RMF_INVALID_PARM | Should pass |
* | 12 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_GetStatus()` with outdated handle after close | previously valid handle | RMF_INVALID_HANDLE | Should pass |
* | 14 | Call `RMF_AudioCapture_GetStatus()` with outdated handle and invalid status after close | previously valid handle, status=NULL | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetStatus (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings;
	RMF_AudioCapture_Status status;

	gTestID = 18;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_GetStatus (NULL, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	//Step 2
	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetStatus(NULL, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	result = RMF_AudioCapture_GetStatus(handle, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	//Step 5
	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus(NULL, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	result = RMF_AudioCapture_GetStatus(handle, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	//Step 9
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus(NULL, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	result = RMF_AudioCapture_GetStatus(handle, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	//Step 12
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus(handle, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	result = RMF_AudioCapture_GetStatus(handle, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test positive scenarios for RMF_AudioCapture_GetCurrentSettings()
*
* Probe various input scenarios for RMF_AudioCapture_GetCurrentSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 019@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Start()` with settings obtained above plus some essential modifications | current handle, settings = default settings, but increase delayCompensation_ms by 1000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that the settings that were applied in start call are currently in effect | current handle, valid settings | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 05 | Call `RMF_AudioCapture_Stop()` to stop the capture| current handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Start()` with settings obtained above plus some essential modifications | current handle, settings = default settings, but increase delayCompensation_ms by 2000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that the settings that were applied in start call are currently in effect | current handle, valid settings | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 09 | Call `RMF_AudioCapture_Stop()` to stop the capture | current handle | RMF_SUCCESS | Should pass |
* | 10 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetCurrentSettings (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings, current_settings;

	gTestID = 19;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	memset(&settings, 0, sizeof(settings)); // To fill padding bytes with zero, allows comparison of structs with memcmp.
	memset(&current_settings, 0, sizeof(current_settings));
	
	result = RMF_AudioCapture_Open(&handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	//Step 2
	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	settings.delayCompensation_ms += 1000; // Increase delay compensation before start to introduce a deviation from the default parameters.
	test_l1_prepare_dummy_start_settings(&settings);
	
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	//Step 4
	result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	//Step 6
	settings.delayCompensation_ms = 0; //Set baseline as 0 before calling getDefaultSettings.
	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	settings.delayCompensation_ms += 2000; // Increase delay compensation before start to introduce a deviation from the default parameters.
	
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	//Step 8
	result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_GetCurrentSettings()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetCurrentSettings() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 020@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | handle=NULL, settings is a valid pointer | RMF_INVALID_HANDLE | Should pass |
* | 02 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | current handle, valid settings | RMF_INVALID_STATE | Should pass |
* | 04 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Start()` with settings obtained above plus some essential modifications | current handle, settings = default settings, but add dummy data callback | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_GetCurrentSettings()` with invalid handle | handle=NULL | RMF_INVALID_HANDLE | Should pass |
* | 07 | Call `RMF_AudioCapture_GetCurrentSettings()` with valid handle, but settings NULL | current handle, settings=NULL | RMF_INVALID_PARM | Should pass |
* | 08 | Call `RMF_AudioCapture_Stop()` to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 09 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | current handle, valid settings | RMF_INVALID_STATE | Should pass |
* | 10 | Call `RMF_AudioCapture_GetCurrentSettings()` with valid handle, but settings NULL | current handle, settings=NULL | RMF_INVALID_PARM | Should pass |
* | 11 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | handle=NULL, settings is a valid pointer | RMF_INVALID_HANDLE | Should pass |
* | 12 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 13 | Call `RMF_AudioCapture_GetCurrentSettings()` when not in STARTED state | previously valid handle, valid settings | RMF_INVALID_HANDLE | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetCurrentSettings (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;
	RMF_AudioCapture_Settings settings, current_settings;
	
	gTestID = 20;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_GetCurrentSettings(NULL, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	//Step 2
	result = RMF_AudioCapture_Open(&handle);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	//Step 4
	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_validate_settings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);	
	result = RMF_AudioCapture_Start(handle, &settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetCurrentSettings(NULL, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	result = RMF_AudioCapture_GetCurrentSettings(handle, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	//Step 8
	result = RMF_AudioCapture_Stop(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);
	result = RMF_AudioCapture_GetCurrentSettings(handle, NULL);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);
	result = RMF_AudioCapture_GetCurrentSettings(NULL, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);

	//Step 12
	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetCurrentSettings(handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_HANDLE);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test positive scenarios with auxiliary audio for RMF_AudioCapture_Open_Type()
*
* Probe various input scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 021@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type is auxiliary | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Open_Type()` to check open->close->open sequence | handle must be a valid pointer, type is auxiliary | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_auxiliary (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 21;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");


	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios with auxiliary audio for RMF_AudioCapture_Open_Type()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 022@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` with invalid handle | handle must be NULL, type=auxiliary | RMF_INVALID_PARM | Should pass |
* | 04 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer, type=auxiliary | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Open_Type()` when aux is already open | handle must be a valid pointer, type=auxiliary | RMF_INVALID_STATE | Should pass |
* | 06 | Call `RMF_AudioCapture_Close()` to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_auxiliary (void)
{
	RMF_AudioCaptureHandle handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 22;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(NULL, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_PARM);

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Open_Type(&handle, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_Close(handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}



/**
* @brief Test positive scenarios with primary and auxiliary audio for RMF_AudioCapture_Open_Type()
*
* Probe various input scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 023@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` to open aux interface | handle must be a valid pointer, type is auxiliary | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Open_Type()` to open primary interface while aux is open | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 03 | Call `RMF_AudioCapture_Close()` to close primary interface | current primary audio handle | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Open()` to check interchangeability of Open and OpenType for primary audio | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Close()` to close aux interface | current aux audio handle | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Open_Type()` to open aux interface while primary is open | handle must be a valid pointer, type is auxiliary | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Close()` to close aux interface | current aux audio handle | RMF_SUCCESS | Should pass |
* | 08 | Call `RMF_AudioCapture_Close()` to close primary interface | current primary audio handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_mixed (void)
{
	RMF_AudioCaptureHandle aux_handle, prim_handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 23;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	if(RMF_SUCCESS != result) //Early abort if opening a capture interface does not work.
	{
		UT_LOG("Aborting test - unable to open primary capture interface. Error code: %d", result);
		result = RMF_AudioCapture_Close(aux_handle);
		UT_FAIL("Aborting test - unable to open capture.");
	}

	result = RMF_AudioCapture_Close(prim_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open(&prim_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(aux_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(aux_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(prim_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

/**
* @brief Test negative scenarios with primary and auxiliary audio for RMF_AudioCapture_Open_Type()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open_Type() and verify that it executes in accordance with the specification.
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 024@n
* @n
* **Pre-Conditions:** AudioCapture interface is not open already.@n
* **Dependencies:** None@n
* **User Interaction:** None@n

* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` to open aux interface | handle must be a valid pointer, type is auxiliary | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_Open_Type()` to open aux interface while aux is already open | handle must be a valid pointer, type is auxiliary | RMF_INVALID_STATE | Should pass |
* | 03 | Call `RMF_AudioCapture_Open_Type()` to open primary interface | handle must be a valid pointer, type is primary | RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Open_Type()` to open primary interface when primary is already open | handle must be a valid pointer, type is primary | RMF_INVALID_STATE | Should pass |
* | 05 | Call `RMF_AudioCapture_Open()` to open primary interface when primary is already open | handle must be a valid pointer | RMF_INVALID_STATE | Should pass |
* | 06 | Call `RMF_AudioCapture_Close()` to close primary interface | current primary audio handle | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_Close()` to close aux interface | current aux audio handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_mixed (void)
{
	RMF_AudioCaptureHandle aux_handle, prim_handle;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 24;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Aborting test - unable to start primary capture. Error code: %d", result);
		result = RMF_AudioCapture_Close(aux_handle);
		UT_FAIL("Aborting test - unable to start primary capture.");
	}
	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);
	result = RMF_AudioCapture_Open(&prim_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_INVALID_STATE);

	result = RMF_AudioCapture_Close(prim_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_Close(aux_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	UT_LOG("\n Out  %s\n",__FUNCTION__);
}


/**
* @brief Test simultaneous primary and auxiliary audio capture
*
* Launch simultaneous capture sessions of primary and auxiliary audio and verify that status and settings are returned correctly
*
* **Test Group ID:** Basic 01@n
* **Test Case ID:** 025@n
*
* **Test Procedure:**
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :-------: | ------------- | --------- | --------------- | ----- |
* | 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer; type is auxiliary | RMF_SUCCESS | Should pass |
* | 02 | Call `RMF_AudioCapture_GetStatus()` to check status of open interface | current handle | returns RMF_SUCCESS, RMF_AudioCapture_Status.started must be 0 | Should pass |
* | 03 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | returns RMF_SUCCESS | Should pass |
* | 04 | Call `RMF_AudioCapture_Start()` to start audio capture | current handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 05 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer; type is primary | RMF_SUCCESS | Should pass |
* | 06 | Call `RMF_AudioCapture_Start()` to start audio capture | current primary audio handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 07 | Call `RMF_AudioCapture_GetStatus()` to check current status of started auxiliary capture | current aux handle, valid settings | returns RMF_SUCCESS, RMF_AudioCapture_Status.started must be 1, format and samplingFreq must have valid values | Should pass |
* | 08 | Call `RMF_AudioCapture_GetStatus()` to check current status of started primary capture | current primary audio handle; valid settings | returns RMF_SUCCESS, RMF_AudioCapture_Status.started must be 1, format and samplingFreq must have valid values | Should pass |
* | 09 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that the settings that were applied in start call are currently in effect | current aux handle, valid ttings | returns RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 10 | Call `RMF_AudioCapture_GetCurrentSettings()` to confirm that the settings that were applied in start call are currently in effect | current primary audio ndle, valid settings | returns RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 11 | Call `RMF_AudioCapture_Stop()` to stop the primary capture | current primary audio handle | RMF_SUCCESS | Should pass |
* | 12 | Call `RMF_AudioCapture_GetStatus()` to check current status of stopped/open interface | current primary audio handle, valid settings | returns RMF_SUCCESS, RMF_AudioCapture_Status.started must be 0 | Should pass |
* | 13 | Call `RMF_AudioCapture_Stop()` to stop the aux capture | current aux handle | RMF_SUCCESS | Should pass |
* | 14 | Call `RMF_AudioCapture_GetStatus()` to check current status of stopped/open interface | current aux handle, valid settings | returns RMF_SUCCESS, RMF_AudioCapture_Status.started must be 0 | Should pass |
* | 15 | Call `RMF_AudioCapture_Close()` to release primary audio resources after test | current primary handle | RMF_SUCCESS | Should pass |
* | 16 | Call `RMF_AudioCapture_Close()` to release aux audio resources after test | current aux handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_simultaneous_sessions (void)
{
	RMF_AudioCaptureHandle aux_handle, prim_handle;
	RMF_AudioCapture_Status status;
	RMF_AudioCapture_Settings settings, current_settings;
	rmf_Error result = RMF_SUCCESS;

	gTestID = 25;
	UT_LOG("\n In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

	memset(&settings, 0, sizeof(settings)); // To fill padding bytes with zero, allows comparison of structs with memcmp.
	memset(&current_settings, 0, sizeof(current_settings));

	result = RMF_AudioCapture_Open_Type(&aux_handle, RMF_AC_TYPE_AUXILIARY);
	if(RMF_SUCCESS != result)
		UT_FAIL("Aborting test - unable to open capture.");

	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(0, status.started);

	result = RMF_AudioCapture_GetDefaultSettings(&settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	test_l1_prepare_dummy_start_settings(&settings);
	result = RMF_AudioCapture_Start(aux_handle, &settings); //Started auxiliary audio capture.
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Capture start failed with error code: %d", result);
		result = RMF_AudioCapture_Close(aux_handle);
		UT_FAIL("Aborting test - unable to start capture.");
	}

	result = RMF_AudioCapture_Open_Type(&prim_handle, RMF_AC_TYPE_PRIMARY);
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Aborting test - unable to open primary capture interface. Error code: %d", result);
		result = RMF_AudioCapture_Stop(aux_handle);
		result = RMF_AudioCapture_Close(aux_handle);
		UT_FAIL("Aborting test - unable to open primary capture interface");
	}
	result = RMF_AudioCapture_Start(prim_handle, &settings); //Started primary audio capture
	if(RMF_SUCCESS != result)
	{
		UT_LOG("Aborting test - unable to start primary capture. Error code: %d", result);
		result = RMF_AudioCapture_Stop(aux_handle);
		result = RMF_AudioCapture_Close(aux_handle);
		result = RMF_AudioCapture_Close(prim_handle);
		UT_FAIL("Aborting test - unable to open primary capture interface");
	}

	usleep(100 * 1000); // Small delay before checking status so that the internal structures have a chance to update.

	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	result = test_l1_validate_status_active(&status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetStatus (prim_handle, &status);
	result = test_l1_validate_status_active(&status);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetCurrentSettings(aux_handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_GetCurrentSettings(prim_handle, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = test_l1_compare_settings(&settings, &current_settings);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Stop(aux_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	usleep(100 * 1000); // Small delay before checking status so that the internal structures have a chance to update.

	result = RMF_AudioCapture_GetStatus (aux_handle, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(0, status.started);

	result = RMF_AudioCapture_Stop(prim_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);
	result = RMF_AudioCapture_GetStatus (prim_handle, &status);
	UT_ASSERT_EQUAL_NOT_FATAL(0, status.started);

	result = RMF_AudioCapture_Close(prim_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	result = RMF_AudioCapture_Close(aux_handle);
	UT_ASSERT_EQUAL_NOT_FATAL(result, RMF_SUCCESS);

	UT_LOG("\n Out  %s\n",__FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Create a special test suite with only positive test cases.
 * 
 * This test suite will come in handy to exercise some of the major test cases in situations where the HAL is prone to crashing when subjected to 
 * the rigors of negative testing. Defining the environment variable AC_GENERATE_POSITIVE_ONLY_SUITE=TRUE will create a positive-only L1 test suite
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_rmfAudioCapture_register_positive_only_suite ( void )
{
	/* add a suite to the registry */
	pSuite = UT_add_suite( "[L1 rmfAudioCapture positive-only]", NULL, NULL );
	if ( NULL == pSuite )
	{
		return -1;
	}	

	UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_primary_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_primary );
	UT_add_test( pSuite, "RMF_AudioCapture_Open_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open );
	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close );

	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings );

	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_positive_complex" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close_complex );

	UT_add_test( pSuite, "RMF_AudioCapture_Start_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Start );
	UT_add_test( pSuite, "RMF_AudioCapture_Stop_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Stop );

	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_positive_complex" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings_complex );

	UT_add_test( pSuite, "RMF_AudioCapture_GetStatus_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetStatus );

	UT_add_test( pSuite, "RMF_AudioCapture_GetCurrentSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetCurrentSettings );

	if(true == test_l1_is_aux_capture_supported())
	{
		UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_auxiliary_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_auxiliary );
		UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_mixed_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_mixed );
		UT_add_test( pSuite, "RMF_AudioCapture_simultaneous_sessions_positive" ,test_l1_rmfAudioCapture_simultaneous_sessions );
	}
	return 0;
} 



/**
 * @brief Register the main test(s) for this module
 *
 * @return int - 0 on success, otherwise failure
 */
int test_l1_rmfAudioCapture_register ( void )
{
	/* add a suite to the registry */
	pSuite = UT_add_suite( "[L1 rmfAudioCapture]", NULL, NULL );
	if ( NULL == pSuite )
	{
		return -1;
	}	

	UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_primary_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_primary );
	UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_primary_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_primary );
	UT_add_test( pSuite, "RMF_AudioCapture_Open_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open );
	UT_add_test( pSuite, "RMF_AudioCapture_Open_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open );
	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close );
	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close );

	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings );
	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings );


	UT_add_test( pSuite, "RMF_AudioCapture_Start_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Start );
	UT_add_test( pSuite, "RMF_AudioCapture_Start_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Start );
	UT_add_test( pSuite, "RMF_AudioCapture_Stop_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Stop );
	UT_add_test( pSuite, "RMF_AudioCapture_Stop_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Stop );


	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_positive_complex" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close_complex );
	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_negative_complex" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close_complex );

	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_positive_complex" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings_complex );
	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_negative_complex" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings_complex );


	UT_add_test( pSuite, "RMF_AudioCapture_GetStatus_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetStatus );
	UT_add_test( pSuite, "RMF_AudioCapture_GetStatus_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetStatus );

	UT_add_test( pSuite, "RMF_AudioCapture_GetCurrentSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetCurrentSettings );
	UT_add_test( pSuite, "RMF_AudioCapture_GetCurrentSettings_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetCurrentSettings );

	if(true == test_l1_is_aux_capture_supported())
	{
		UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_auxiliary_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_auxiliary );
		UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_auxiliary_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_auxiliary );

		UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_mixed_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open_Type_mixed );
		UT_add_test( pSuite, "RMF_AudioCapture_Open_Type_mixed_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open_Type_mixed );
		
		UT_add_test( pSuite, "RMF_AudioCapture_simultaneous_sessions" ,test_l1_rmfAudioCapture_simultaneous_sessions );
	}

	if(true == test_l1_create_suite_of_positive_tests())
	{
		test_l1_rmfAudioCapture_register_positive_only_suite();
	}

	return 0;
} 

/** @} */ // End of RMF Audio Capture HAL Tests L1 File
/** @} */ // End of RMF Audio Capture HAL Tests
/** @} */ // End of RMF Audio Capture Module
/** @} */ // End of HPK
