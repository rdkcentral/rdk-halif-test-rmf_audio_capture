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
 * @defgroup RMF_AudioCapture_HALTESTS_Register RMF Audio Capture HAL Tests Register File
 * @{
 * @parblock
 *
 * ### Registration of tests for RMF Audio Capture HAL :
 *
 * Registration of tests for Audio Capture HAL.
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
* @file test_register.c
*
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

/** @} */ // End of RMF Audio Capture HAL Tests Register File
/** @} */ // End of RMF Audio Capture HAL Tests
/** @} */ // End of RMF Audio Capture Module
/** @} */ // End of HPK
