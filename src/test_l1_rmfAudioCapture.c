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
* @file test_l1_rmfAudioCapture.c
* @page AudioCapture HAL L1 tests
*
* ## Module's Role
* Level 1 unit test cases for all APIs of AudioCapture HAL.
* This is to ensure that the API meets the operational requirements of the module across all vendors.
*
* **Pre-Conditions:**  None@n
* **Dependencies:** None@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/

#include <string.h>
#include <stdlib.h>

#include <ut.h>
#include <ut_log.h>

/**
* @brief Test positive scenarios for RMF_AudioCapture_Open()
*
* Probe various input scenarios for RMF_AudioCapture_Open() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_Close() to close interface | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Open() to check open->close->open sequence | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_Close() to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Open()
*
* Force various edge/error scenarios for RMF_AudioCapture_Open() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() with invalid handle | handle must be NULL | RMF_INVALID_PARM | Should fail |
* | 02 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Open() when already open | handle must be a valid pointer | RMF_INVALID_STATE | Should fail |
* | 04 | Call RMF_AudioCapture_Close() to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open (void)
{
	UT_FAIL(This function needs to be implemented!); 
}
//TODO: Rearrange TCs. For L1, order matters. APIs should be called in the order of their trust worthiness. Do not use APIs that haven't been tested yet.
// That means do not call getstatus() in a test for open() unless getstatus has completed UT before. Strictly necessary calls are ok, i. e., you can call
// open before start. These compound test cases need to be consolidated under a different function that runs after the individual APIs are tested.

/**
* @brief Test positive scenarios for RMF_AudioCapture_GetStatus()
*
* Probe various input scenarios for RMF_AudioCapture_GetStatus() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetStatus() to check status of open interface | current handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 0 | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() to start audio capture | current handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to check current status of started interface | current handle, valid settings | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 1, format and samplingFreq must have valid values | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop the interface | current handle | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to check current status of stopped/open interface | current handle, valid settings | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 0 | Should pass |
* | 07 | Call RMF_AudioCapture_Close() to release resources after test | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetStatus (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_GetStatus()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetStatus() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_GetStatus() before open | handle=NULL | return RMF_INVALID_HANDLE | Should fail |
* | 02 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_GetStatus() with invalid handle after open | handle=NULL | RMF_INVALID_HANDLE | Should fail |
* | 04 | Call RMF_AudioCapture_GetStatus() with valid handle and invalid status after open | current handle, status=NULL | RMF_INVALID_PARM | Should fail |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() to start audio capture | current handle, settings = default settings + dummy buffer ready callback | RMF_SUCCESS | Should pass |
* | 06 | Call RMF_AudioCapture_GetStatus() with invalid handle after start | handle=NULL | RMF_INVALID_HANDLE | Should fail |
* | 07 | Call RMF_AudioCapture_GetStatus() with valid handle and invalid status after start | current handle, status=NULL | RMF_INVALID_PARM | Should fail |
* | 08 | Call RMF_AudioCapture_Stop() to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 09 | Call RMF_AudioCapture_GetStatus() with invalid handle after stop | handle=NULL | RMF_INVALID_HANDLE | Should fail |
* | 10 | Call RMF_AudioCapture_GetStatus() with valid handle and invalid status after stop | current handle, status=NULL | RMF_INVALID_PARM | Should fail |
* | 11 | Call RMF_AudioCapture_Close() to close interface | current handle | RMF_SUCCESS | Should pass |
* | 12 | Call RMF_AudioCapture_GetStatus() with outdated handle after stop | previously valid handle | RMF_INVALID_HANDLE | Should fail |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetStatus (void)
{
	UT_FAIL(This function needs to be implemented!); 
}


/**
* @brief Test positive scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Probe various input scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() with valid parameters and validate it | valid pointer for settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid | Should pass |
* | 03 | Call RMF_AudioCapture_Start() to start with minimum viable settings | current handle, settings = default settings, but increase delayCompensation_ms by 1000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetDefaultSettings() after start and validate settings | valid settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid, delayCompensation_ms must be the original value | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop the capture | current handle | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetDefaultSettings() after start and validate settings | valid settings | return RMF_SUCCESS, RMF_AudioCapture_Settings.format and samplingFreq must be valid, delayCompensation_ms must be the original value | Should pass |
* | 07 | Call RMF_AudioCapture_Close() with current handle | Release resource after test | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_GetDefaultSettings()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetDefaultSettings() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_GetDefaultSettings() when not in open/started state | valid settings | RMF_INVALID_STATE | Should fail |
* | 02 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_GetDefaultSettings() with invalid parameters | settings=NULL | RMF_INVALID_PARM | Should fail |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() with valid parameters | valid pointer for settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() to start with minimum viable settings | current handle, settings = default settings, add dummy data callback | RMF_SUCCESS | Should pass |
* | 05 | Call RMF_AudioCapture_GetDefaultSettings() with invalid parameters | settings=NULL | RMF_INVALID_PARM | Should fail |
* | 06 | Call RMF_AudioCapture_Stop() to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 05 | Call RMF_AudioCapture_GetDefaultSettings() with invalid parameters | settings=NULL | RMF_INVALID_PARM | Should fail |
* | 08 | Call RMF_AudioCapture_Close() to close interface | current handle | RMF_SUCCESS | Should pass |
* | 09 | Call RMF_AudioCapture_GetDefaultSettings() when not in open/started state | valid settings | RMF_INVALID_STATE | Should fail |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_GetCurrentSettings()
*
* Probe various input scenarios for RMF_AudioCapture_GetCurrentSettings() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with settings obtained above plus some essential modifications | current handle, settings = default settings, but increase delayCompensation_ms by 1000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() to confirm that the settings that were applied in start call are currently in effect | current handle, valid settings | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop the capture| current handle | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with settings obtained above plus some essential modifications | current handle, settings = default settings, but increase delayCompensation_ms by 2000, add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() to confirm that the settings that were applied in start call are currently in effect | current handle, valid settings | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop the capture | current handle | RMF_SUCCESS | Should pass |
* | 08 | Call RMF_AudioCapture_Close() to close interface | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetCurrentSettings (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_GetCurrentSettings()
*
* Force various edge/error scenarios for RMF_AudioCapture_GetCurrentSettings() and verify that it executes in accordance with the specification.
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
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() when not in STARTED state | handle=NULL, settings is a valid pointer | RMF_INVALID_HANDLE | Should fail |
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() when not in STARTED state | current handle, valid settings | RMF_INVALID_STATE | Should fail |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | settings must be a valid pointer | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with settings obtained above plus some essential modifications | current handle, settings = default settings, but add dummy data callback | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() with invalid handle | handle=NULL | RMF_INVALID_HANDLE | Should fail |
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() with valid handle, but settings NULL | current handle, settings=NULL | RMF_INVALID_PARM | Should fail |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture | current handle | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() when not in STARTED state | current handle, valid settings | RMF_INVALID_STATE | Should fail |
* | 07 | Call RMF_AudioCapture_Close() to close interface | current handle | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetCurrentSettings() when not in STARTED state | previously valid handle, valid settings | RMF_INVALID_HANDLE | Should fail |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetCurrentSettings (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_Start()
*
* Probe various input scenarios for RMF_AudioCapture_Start() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 1, format and samplingFreq must have valid values | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 06 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 07 | Call RMF_AudioCapture_Start() with caller-modified settings derived from above | settings - increase delayCompensation_ms by 2000, dummy data callback, empty status callback | RMF_SUCCESS | Should pass |
* | 08 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 1, format and samplingFreq must have valid values | Should pass |
* | 09 | Call RMF_AudioCapture_GetCurrentSettings() to confirm that passed parameters were set | valid handle | return RMF_SUCCESS, settings parameter must match what was set in previous start call | Should pass |
* | 10 | Call RMF_AudioCapture_Stop() to stop audio capture | valid handle | RMF_SUCCESS | Should pass |
* | 16 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
* | 17 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 18 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 19 | Call RMF_AudioCapture_Start() with default settings to check start after stop->close->open | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 20 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 1, format and samplingFreq must have valid values | Should pass |
* | 21 | Call RMF_AudioCapture_Stop() to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 22 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Start (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Start()
*
* Force various edge/error scenarios for RMF_AudioCapture_Start() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Start() with invalid handle | handle is NULL, settings is NULL | RMF_INVALID_HANDLE | Should fail |
* | 02 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 18 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with invalid handle | handle is NULL, settings is NULL | RMF_INVALID_HANDLE | Should fail |
* | 03 | Call RMF_AudioCapture_Start() with invalid handle | current handle, settings is NULL | RMF_INVALID_PARM | Should fail |
* | 04 | Call RMF_AudioCapture_Start() with parameter outside bounds | current handle, settings = default settings, but format = racFormat_eMax | RMF_INVALID_PARM | Should fail |
* | 05 | Call RMF_AudioCapture_Start() with parameter outside bounds | current handle, settings = default settings, but samplingFreq = racFreq_eMax | RMF_INVALID_PARM | Should fail |
* | 06 | Call RMF_AudioCapture_Start() with parameter outside bounds | current handle, settings = default settings, but cbBufferReady = NULL | RMF_INVALID_PARM | Should fail |
* | 03 | Call RMF_AudioCapture_Start() with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 08 | Call RMF_AudioCapture_Start() again to test API response when already in STARTED state | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_STATE | Should fail |
* | 11 | Call RMF_AudioCapture_Stop() to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 12 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
* | 13 | Call RMF_AudioCapture_Start() with outdated handle | handle obtained in last open call, settings=default settings, but dummy data callback, status callback NULL | RMF_INVALID_HANDLE | Should fail |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Start (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_Stop()
*
* Probe various input scenarios for RMF_AudioCapture_Stop() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 1, format and samplingFreq must have valid values | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 0 | Should pass |
* | 03 | Call RMF_AudioCapture_Start() to restart defaults | settings=default settings , but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 1, format and samplingFreq must have valid values | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture, tests start->stop->start->stop sequence | valid handle | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 0 | Should pass |
* | 22 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 1, format and samplingFreq must have valid values | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture, tests start->stop->close->open->start->stop sequence | valid handle | RMF_SUCCESS | Should pass |
* | 04 | Call RMF_AudioCapture_GetStatus() to confirm state change | valid handle | return RMF_SUCCESS, RMF_AudioCapture_Status.status must be 0 | Should pass |
* | 22 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Stop (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Stop()
*
* Force various edge/error scenarios for RMF_AudioCapture_Stop() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Stop() with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should fail |
* | 02 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 08 | Call RMF_AudioCapture_Stop() to test API response when not in STARTED state | current handle | RMF_INVALID_STATE | Should fail |
* | 18 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 11 | Call RMF_AudioCapture_Stop() with invalid handle | handle=NULL | RMF_INVALID_PARM | Should fail |
* | 11 | Call RMF_AudioCapture_Stop() to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 08 | Call RMF_AudioCapture_Stop() repeatedly, violating state machine | current handle | RMF_INVALID_STATE | Should fail |
* | 12 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
* | 01 | Call RMF_AudioCapture_Stop() with outdated handle | handle obtained in last open call | RMF_INVALID_HANDLE | Should fail |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Stop (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test positive scenarios for RMF_AudioCapture_Close()
*
* Probe various input scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 22 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 02 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with settings obtained above to start audio capture | settings=default settings from previous step, dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 22 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with old handle to confirm that it was invalidated | previous handle, settings=default settings from previous step, dummy data callback, status callback NULL | RMF_INVALID_HANDLE | Should fail |
* | 01 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() to start audio capture | settings=default settings , but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() to start audio capture | settings=default settings , but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 05 | Call RMF_AudioCapture_Stop() to stop capture | valid handle | RMF_SUCCESS | Should pass |
* | 22 | Call RMF_AudioCapture_Close() to release resources, test open->start->stop->start->stop->close | current handle | RMF_SUCCESS | Should pass |
*/
void test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

/**
* @brief Test negative scenarios for RMF_AudioCapture_Close()
*
* Force various edge/error scenarios for RMF_AudioCapture_Close() and verify that it executes in accordance with the specification.
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
* | 01 | Call RMF_AudioCapture_Close() with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should fail |
* | 02 | Call RMF_AudioCapture_Open() to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
* | 01 | Call RMF_AudioCapture_Close() with invalid handle | handle is NULL | RMF_INVALID_HANDLE | Should fail |
* | 18 | Call RMF_AudioCapture_GetDefaultSettings() to get default settings | valid settings | return RMF_SUCCESS | Should pass |
* | 03 | Call RMF_AudioCapture_Start() with default settings to start audio capture | current handle, settings=default settings, but dummy data callback, status callback NULL | RMF_SUCCESS | Should pass |
* | 01 | Call RMF_AudioCapture_Close() in STARTED state | current handle | RMF_INVALID_STATE | Should fail |
* | 11 | Call RMF_AudioCapture_Stop() to stop audio capture | current handle | RMF_SUCCESS | Should pass |
* | 12 | Call RMF_AudioCapture_Close() to release resources | current handle | RMF_SUCCESS | Should pass |
* | 01 | Call RMF_AudioCapture_Close() with outdated handle | handle obtained in last open call | RMF_INVALID_HANDLE | Should fail |
*/
void test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close (void)
{
	UT_FAIL(This function needs to be implemented!); 
}

static UT_test_suite_t * pSuite = NULL;

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

	UT_add_test( pSuite, "RMF_AudioCapture_Open_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Open );
	UT_add_test( pSuite, "RMF_AudioCapture_Open_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Open );
	UT_add_test( pSuite, "RMF_AudioCapture_GetStatus_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetStatus );
	UT_add_test( pSuite, "RMF_AudioCapture_GetStatus_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetStatus );
	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetDefaultSettings );
	UT_add_test( pSuite, "RMF_AudioCapture_GetDefaultSettings_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetDefaultSettings );
	UT_add_test( pSuite, "RMF_AudioCapture_GetCurrentSettings_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_GetCurrentSettings );
	UT_add_test( pSuite, "RMF_AudioCapture_GetCurrentSettings_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_GetCurrentSettings );
	UT_add_test( pSuite, "RMF_AudioCapture_Start_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Start );
	UT_add_test( pSuite, "RMF_AudioCapture_Start_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Start );
	UT_add_test( pSuite, "RMF_AudioCapture_Stop_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Stop );
	UT_add_test( pSuite, "RMF_AudioCapture_Stop_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Stop );
	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_positive" ,test_l1_rmfAudioCapture_positive_RMF_AudioCapture_Close );
	UT_add_test( pSuite, "RMF_AudioCapture_Close_L1_negative" ,test_l1_rmfAudioCapture_negative_RMF_AudioCapture_Close );
	

	return 0;
} 
