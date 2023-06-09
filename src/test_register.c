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
* @file test_register.c
*
* ## Module's Role
* Registration of tests for AudioCapture HAL
* This is to ensure that the API meets the operational requirements of the module across all vendors.
*
* **Pre-Conditions:**  Launch video playback before commencing tests.@n
* **Dependencies:** None@n
*
* Ref to API Definition specification documentation : [halSpec.md](../../../docs/halSpec.md)
*/

#include <ut.h>

/**
 * @brief Register test functionality
 * 
 */

/* L1 Testing Functions */
extern int test_l1_rmfAudioCapture_register( void );


/* L2 Testing Functions */
extern int test_l2_rmfAudioCapture_register( void );

int UT_register_APIDEF_l1_tests( void )
{
	int registerFailed = 0;

	registerFailed |= test_l1_rmfAudioCapture_register();

	return registerFailed;
}

/* Register UT Functions */
int UT_register_APIDEF_l2_tests( void )
{
	int registerFailed = 0;

	registerFailed |= test_l2_rmfAudioCapture_register();

	return registerFailed;
}

/** @} */ // End of RMF AudioCapture Tests
/** @} */ // End of RMF_AudioCapture RMF AudioCapture
/** @} */ // End of HPK
