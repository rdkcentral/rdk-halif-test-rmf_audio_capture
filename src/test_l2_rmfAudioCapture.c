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
 * @addtogroup HPK Hardware Porting Kit
 * @{
 *
 */

/**
 * @addtogroup RMF_AudioCapture RMF AudioCapture
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
 * @defgroup RMF_AudioCapture_HALTESTS_L2 RMF Audio Capture HAL Tests L2 File
 * @{
 * @parblock
 *
 * ### L2 Tests for RMF Audio Capture HAL :
 *
 * Level 2 module tests will perform module level testing by exercising the full set of APIs to validate various use cases.
 * This is to ensure that the API meets the operational requirements of the module across all vendors.
 *
 * **Pre-Conditions:**  None@n
 * **Dependencies:** None@n
 *
 * Refer to API Definition specification documentation : [halSpec.md](../../docs/pages/halSpec.md)
 *
 * @endparblock
 *
 */


/**
* @file test_l2_rmfAudioCapture.c
*
*/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <ut.h>
#include <ut_log.h>
#include "rmfAudioCapture.h"

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
	return 0;
} 

/** @} */ // End of RMF Audio Capture HAL Tests L2 File
/** @} */ // End of RMF Audio Capture HAL Tests
/** @} */ // End of RMF Audio Capture Module
/** @} */ // End of HPK

