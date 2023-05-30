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

#include <ut.h>
#include <ut_log.h>

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

	
	UT_add_test( pSuite, "test_l2_rmfAudioCapture" ,test_l2_rmfAudioCapture );

	return 0;
} 

/** @} */ // End of RMF_AudioCapture RDK-V RMF AudioCapture