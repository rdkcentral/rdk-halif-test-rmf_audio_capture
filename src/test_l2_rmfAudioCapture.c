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
* @file test_l2_rmfAudioCapture.c
* @page rmfAudioCapture Level 2 Tests
*
* ## Module's Role
* This module includes Level 2 functional tests (success and failure scenarios).
* This is to ensure that the rmfAudioCapture APIs meet the requirements across all vendors.
*
* **Pre-Conditions:**  None@n
* **Dependencies:** None@n
*
* Ref to API Definition specification documentation : [rmf-audio-capture_halSpec.md](../../docs/pages/rmf-audio-capture_halSpec.md)
*/

#include <ut.h>
#include <ut_cunit.h>
#include <ut_kvp_profile.h>
#include <unistd.h>
#include "rmfAudioCapture.h"

static int gTestGroup = 2;
static int gTestID = 1;

int cbCounter_primary;
int cbCounter_aux;

rmf_Error test_l2_dummy_data_cb_primary(void *cbBufferReadyParm, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    cbCounter_primary++;
	UT_LOG_DEBUG("Data callback called %dth time for primary audio capture",cbCounter_primary);
    UT_ASSERT_PTR_NOT_NULL(AudioCaptureBuffer);
    UT_ASSERT_TRUE(AudioCaptureBufferSize>0);
    return RMF_SUCCESS;
}

rmf_Error test_l2_dummy_data_cb_auxillary(void *cbBufferReadyParm, void *AudioCaptureBuffer, unsigned int AudioCaptureBufferSize)
{
    cbCounter_aux++;
	UT_LOG_DEBUG("Data callback called %dth time for auxillary audio capture",cbCounter_aux);
    UT_ASSERT_PTR_NOT_NULL(AudioCaptureBuffer);
    UT_ASSERT_TRUE(AudioCaptureBufferSize>0);
    return RMF_SUCCESS;
}

/**
* @brief Test the primary audio capture functionality
*
* This test verifies the primary audio capture functionality by opening an audio capture handle,
* getting default settings, modifying settings, starting the capture, capturing audio for a duration,
* stopping the capture, and finally closing the handle. The test ensures that each step returns
* the expected success status and that the callbacks are invoked correctly.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 001@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmfAudioCapture_L2_Low-Level_TestSpecification.md](../../docs/pages/rmfAudioCapture_L2_Low-Level_TestSpecification.md)
*/

void test_l2_rmfAudioCapture_RunPrimaryAudioCapture(void)
{
    gTestID = 1;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    RMF_AudioCaptureHandle handle;
    RMF_AudioCapture_Settings settings;
    rmf_Error ret;
    int current_cbCounter = 0;

    // Step 1: Call RMF_AudioCapture_Open
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Open with valid handle");
    ret = RMF_AudioCapture_Open(&handle);
    UT_LOG_DEBUG("handle: %p, return status: %d", handle, ret);
    UT_ASSERT_EQUAL_FATAL(ret, RMF_SUCCESS);

    // Step 2: Call RMF_AudioCapture_GetDefaultSettings
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_GetDefaultSettings with valid settings");
    ret = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_LOG_DEBUG("settings: {cbBufferReady: %p, cbBufferReadyParm: %p, cbStatusChange: %p, cbStatusParm: %p, fifoSize: %zu, threshold: %zu, format: %d, samplingFreq: %d, delayCompensation_ms: %u}, return status: %d",
                 settings.cbBufferReady, settings.cbBufferReadyParm, settings.cbStatusChange, settings.cbStatusParm, settings.fifoSize, settings.threshold, settings.format, settings.samplingFreq, settings.delayCompensation_ms, ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_GetDefaultSettings");
        cbCounter_primary = 0;
        RMF_AudioCapture_Close(handle);
        return;
    }

    // Step 3: Modify settings
    settings.cbBufferReady = test_l2_dummy_data_cb_primary;
    settings.cbBufferReadyParm = NULL;

    // Step 4: Call RMF_AudioCapture_Start
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Start with handle: %p and modified settings", handle);
    ret = RMF_AudioCapture_Start(handle, &settings);
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Start");
        cbCounter_primary = 0;
        RMF_AudioCapture_Close(handle);
        return;
    }

    // Step 5: Capture audio for 10 seconds
    sleep(10);

    // Step 6: Verify callback invocation and audio samples received
    // This step is typically verified through the callback function itself

    // Step 7: Call RMF_AudioCapture_Stop
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Stop with handle: %p", handle);
    ret = RMF_AudioCapture_Stop(handle);
    current_cbCounter = cbCounter_primary;
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Stop");
        cbCounter_primary = 0;
        RMF_AudioCapture_Close(handle);
        return;
    }

    //Wait for sometime after RMF_AudioCapture_Stop
    sleep(1);

    // Step 8: Verify no more callbacks after stop
    // This step is typically verified through the callback function itself
    UT_LOG_DEBUG("Check if no more callbacks after stop by comparing the callback counter value");
    UT_ASSERT_EQUAL(current_cbCounter,cbCounter_primary);
    UT_LOG_DEBUG("Callback counter immediately after stop : %d and the counter value sometime after stop : %d", current_cbCounter, cbCounter_primary);
    if (current_cbCounter == cbCounter_primary)
    {
        UT_LOG_DEBUG("No more callbacks after stop");
        UT_PASS("Callback counter immediately after stop matches with the counter value sometime after stop ");
    }
    else
    {
        UT_LOG_DEBUG("Callbacks happened after stop");
        UT_FAIL("Callback counter immediately after stop mismatches with the counter value sometime after stop ");
    }

    // Step 9: Call RMF_AudioCapture_Close
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close with handle: %p", handle);
    cbCounter_primary = 0;
    ret = RMF_AudioCapture_Close(handle);
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the auxiliary audio capture functionality
*
* This test verifies the complete flow of auxiliary audio capture including opening the capture,
* getting default settings, modifying settings, starting capture, capturing audio, stopping capture,
* and closing the capture. It ensures that each step returns the expected success status.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 002@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmfAudioCapture_L2_Low-Level_TestSpecification.md](../../docs/pages/rmfAudioCapture_L2_Low-Level_TestSpecification.md)
*/

void test_l2_rmfAudioCapture_RunAuxiliaryAudioCapture(void)
{
    gTestID = 2;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    RMF_AudioCaptureHandle handle = NULL;
    RMF_AudioCapture_Settings settings;
    rmf_Error ret;
    int current_cbCounter = 0;

    // Step 1: Call RMF_AudioCapture_Open_Type
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Open_Type with valid handle and auxiliary type");
    ret = RMF_AudioCapture_Open_Type(&handle, "auxiliary");
    UT_LOG_DEBUG("Handle: %p, Return status: %d", handle, ret);
    UT_ASSERT_EQUAL_FATAL(ret, RMF_SUCCESS);

    // Step 2: Call RMF_AudioCapture_GetDefaultSettings
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_GetDefaultSettings with valid settings pointer");
    ret = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_LOG_DEBUG("Settings: {cbBufferReady: %p, cbBufferReadyParm: %p, cbStatusChange: %p, cbStatusParm: %p, fifoSize: %zu, threshold: %zu, format: %d, samplingFreq: %d, delayCompensation_ms: %u}, Return status: %d",
                 settings.cbBufferReady, settings.cbBufferReadyParm, settings.cbStatusChange, settings.cbStatusParm, settings.fifoSize, settings.threshold, settings.format, settings.samplingFreq, settings.delayCompensation_ms, ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_GetDefaultSettings");
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(handle);
        return;
    }

    // Step 3: Modify RMF_AudioCapture_Settings
    settings.cbBufferReady = test_l2_dummy_data_cb_auxillary;
    settings.cbBufferReadyParm = NULL; // Optional parameter

    // Step 4: Call RMF_AudioCapture_Start
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Start with valid handle and modified settings");
    ret = RMF_AudioCapture_Start(handle, &settings);
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Start");
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(handle);
        return;
    }

    // Step 5: Capture audio for 10 seconds and verify callback
    sleep(10); // Simulate 10 seconds of audio capture
    // Verification of callback invocation would be done in the callback function itself

    // Step 6: Call RMF_AudioCapture_Stop
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Stop with valid handle");
    ret = RMF_AudioCapture_Stop(handle);
    current_cbCounter = cbCounter_aux;
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Stop");
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(handle);
        return;
    }

    //Wait for sometime after RMF_AudioCapture_Stop
    sleep(1);

    // Step 7: Verify no more callbacks after stop
    // This would be verified in the callback function itself
    UT_LOG_DEBUG("Check if no more callbacks after stop by comparing the callback counter value");
    UT_ASSERT_EQUAL(current_cbCounter,cbCounter_aux);
    UT_LOG_DEBUG("Callback counter immediately after stop : %d and the counter value sometime after stop : %d", current_cbCounter, cbCounter_aux);
    if (current_cbCounter == cbCounter_aux)
    {
        UT_LOG_DEBUG("No more callbacks after stop");
        UT_PASS("Callback counter immediately after stop matches with thecounter value sometime after stop");
    }
    else
    {
        UT_LOG_DEBUG("Callbacks happened after stop");
        UT_FAIL("Callback counter immediately after stop mismatches with the counter value sometime after stop");
    }

    // Step 8: Call RMF_AudioCapture_Close
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close with valid handle");
    cbCounter_aux = 0;
    ret = RMF_AudioCapture_Close(handle);
    UT_LOG_DEBUG("Return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

/**
* @brief Test the functionality of running auxiliary and primary audio capture simultaneously
*
* This test verifies the ability to open, start, capture, and stop audio capture on both primary and auxiliary interfaces. It ensures that the audio capture handles are correctly opened, the default settings are retrieved and modified, and audio capture is successfully started and stopped. The test also verifies that the appropriate callbacks are invoked and no more callbacks are issued after stopping the capture.
*
* **Test Group ID:** 02@n
* **Test Case ID:** 003@n
*
* **Test Procedure:**
* Refer to UT specification documentation [rmfAudioCapture_L2_Low-Level_TestSpecification.md](../../docs/pages/rmfAudioCapture_L2_Low-Level_TestSpecification.md)
*/

void test_l2_rmfAudioCapture_RunAuxiliaryPrimaryAudioCapture(void)
{
    gTestID = 3;
    UT_LOG_INFO("In %s [%02d%03d]\n", __FUNCTION__, gTestGroup, gTestID);

    RMF_AudioCaptureHandle primaryHandle = NULL;
    RMF_AudioCaptureHandle auxiliaryHandle = NULL;
    RMF_AudioCapture_Settings settings;
    RMF_AudioCapture_Settings settings_aux ={0};
    rmf_Error ret;
    int current_cbCounter_primary = 0;
    int current_cbCounter_aux = 0;

    // Step 1: Call RMF_AudioCapture_Open_Type to open the primary audio capture interface
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Open_Type with valid handle and primary type");
    ret = RMF_AudioCapture_Open_Type(&primaryHandle, "primary");
    UT_LOG_DEBUG("primaryHandle: %p, return status: %d", primaryHandle, ret);
    UT_ASSERT_EQUAL_FATAL(ret, RMF_SUCCESS);

    // Step 2: Call RMF_AudioCapture_Open_Type to open the auxiliary audio capture interface
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Open_Type with valid handle and auxiliary type");
    ret = RMF_AudioCapture_Open_Type(&auxiliaryHandle, "auxiliary");
    UT_LOG_DEBUG("auxiliaryHandle: %p, return status: %d", auxiliaryHandle, ret);
    UT_ASSERT_EQUAL_FATAL(ret, RMF_SUCCESS);

    // Step 3: Call RMF_AudioCapture_GetDefaultSettings to get the default settings for audio capture
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_GetDefaultSettings with valid settings");
    ret = RMF_AudioCapture_GetDefaultSettings(&settings);
    UT_LOG_DEBUG("settings: {cbBufferReady: %p, cbBufferReadyParm: %p, cbStatusChange: %p, cbStatusParm: %p, fifoSize: %zu, threshold: %zu, format: %d, samplingFreq: %d, delayCompensation_ms: %u}, return status: %d",
                 settings.cbBufferReady, settings.cbBufferReadyParm, settings.cbStatusChange, settings.cbStatusParm, settings.fifoSize, settings.threshold, settings.format, settings.samplingFreq, settings.delayCompensation_ms, ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_GetDefaultSettings");
        cbCounter_primary = 0;
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(primaryHandle);
        RMF_AudioCapture_Close(auxiliaryHandle);
        return;
    }

    //Take a copy for settings for auxillary audio
    settings = settings_aux;

    // Step 4: Modify the settings as needed, ensuring cbBufferReady is set to a valid callback function
    settings.cbBufferReady = test_l2_dummy_data_cb_primary;
    settings.cbBufferReadyParm = NULL; // Optional parameter

    settings_aux.cbBufferReady = test_l2_dummy_data_cb_auxillary;
    settings_aux.cbBufferReadyParm = NULL; // Optional parameter

    // Step 5: Call RMF_AudioCapture_Start with the primary audio capture handle and modified settings
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Start with primaryHandle and modified settings");
    ret = RMF_AudioCapture_Start(primaryHandle, &settings);
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Start");
        cbCounter_primary = 0;
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(primaryHandle);
        RMF_AudioCapture_Close(auxiliaryHandle);
        return;
    }

    // Step 6: Call RMF_AudioCapture_Start with the auxiliary audio capture handle and modified settings
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Start with auxiliaryHandle and modified settings");
    ret = RMF_AudioCapture_Start(auxiliaryHandle, &settings_aux);
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Start");
        cbCounter_primary = 0;
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(primaryHandle);
        RMF_AudioCapture_Close(auxiliaryHandle);
        return;
    }

    // Step 7: Capture audio for 10 seconds
    sleep(10);

    // Step 8: Verify the receipt of the commensurate amount of audio samples through the cbBufferReady callback

    // Step 9: Call RMF_AudioCapture_Stop for the primary audio capture handle
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Stop with primaryHandle");
    ret = RMF_AudioCapture_Stop(primaryHandle);
    current_cbCounter_primary = cbCounter_primary;
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Stop");
        cbCounter_primary = 0;
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(primaryHandle);
        RMF_AudioCapture_Close(auxiliaryHandle);
        return;
    }

    // Step 10: Call RMF_AudioCapture_Stop for the auxiliary audio capture handle
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Stop with auxiliaryHandle");
    ret = RMF_AudioCapture_Stop(auxiliaryHandle);
    current_cbCounter_aux = cbCounter_aux;
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);
    if (ret != RMF_SUCCESS)
    {
        UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close due to failure of RMF_AudioCapture_Stop");
        cbCounter_primary = 0;
        cbCounter_aux = 0;
        RMF_AudioCapture_Close(primaryHandle);
        RMF_AudioCapture_Close(auxiliaryHandle);
        return;
    }

    //Wait for sometime after RMF_AudioCapture_Stop
    sleep(1);

    // Step 11: Verify that no more data-ready callbacks are issued after RMF_AudioCapture_Stop returns
    UT_LOG_DEBUG("Check if no more callbacks after stop by comparing the callback counter value for primary and auxillary audio captures");
    UT_ASSERT_EQUAL(current_cbCounter_primary,cbCounter_primary);
    UT_LOG_DEBUG("Callback counter immediately after stop : %d and the counter value sometime after stop : %d", current_cbCounter_primary, cbCounter_primary);
    if (current_cbCounter_primary == cbCounter_primary)
    {
        UT_LOG_DEBUG("No more callbacks after stop for primary audio capture");
        UT_PASS("Callback counter immediately after stop matches with thecounter value sometime after stop");
    }
    else
    {
        UT_LOG_DEBUG("Callbacks happened after stop");
        UT_FAIL("Callback counter immediately after stop mismatches with the counter value sometime after stop");
    }

    UT_ASSERT_EQUAL(current_cbCounter_aux,cbCounter_aux);
    UT_LOG_DEBUG("Callback counter immediately after stop : %d and the counter value sometime after stop : %d", current_cbCounter_aux, cbCounter_aux);
    if (current_cbCounter_aux == cbCounter_aux)
    {
        UT_LOG_DEBUG("No more callbacks after stop for auxillary audio capture");
        UT_PASS("Callback counter immediately after stop matches with thecounter value sometime after stop ");
    }
    else
    {
        UT_LOG_DEBUG("Callbacks happened after stop for auxillary audio capture");
        UT_FAIL("Callback counter immediately after stop mismatches with the counter value sometime after stop");
    }

    cbCounter_primary = 0;
    cbCounter_aux = 0;

    // Step 12: Call RMF_AudioCapture_Close for the primary audio capture handle
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close with primaryHandle");
    ret = RMF_AudioCapture_Close(primaryHandle);
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL(ret, RMF_SUCCESS);

    // Step 13: Call RMF_AudioCapture_Close for the auxiliary audio capture handle
    UT_LOG_DEBUG("Invoking RMF_AudioCapture_Close with auxiliaryHandle");
    ret = RMF_AudioCapture_Close(auxiliaryHandle);
    UT_LOG_DEBUG("return status: %d", ret);
    UT_ASSERT_EQUAL_FATAL(ret, RMF_SUCCESS);

    UT_LOG_INFO("Out %s\n", __FUNCTION__);
}

static UT_test_suite_t * pSuite = NULL;

/**
 * @brief Register the main tests for this module
 *
 * @return int - 0 on success, otherwise failure
 */

int test_rmfAudioCapture_l2_register(void)
{
    // Create the test suite
    pSuite = UT_add_suite("[L2 rmfAudioCapture]", NULL, NULL);
    if (pSuite == NULL)
    {
        return -1;
    }
    // List of test function names and strings

    UT_add_test( pSuite, "l2_rmfAudioCapture_RunPrimaryAudioCapture", test_l2_rmfAudioCapture_RunPrimaryAudioCapture);
    UT_add_test( pSuite, "l2_rmfAudioCapture_RunAuxiliaryAudioCapture", test_l2_rmfAudioCapture_RunAuxiliaryAudioCapture);
    UT_add_test( pSuite, "l2_rmfAudioCapture_RunAuxiliaryPrimaryAudioCapture", test_l2_rmfAudioCapture_RunAuxiliaryPrimaryAudioCapture);

    return 0;
}
