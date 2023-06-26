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


#ifndef __TEST_LX_RMFAUDIOCAPTURE_HELPER_H__
#define __TEST_LX_RMFAUDIOCAPTURE_HELPER_H__

#define VALIDATE_SUCCESSFUL_OPEN(result) do{\
	if(RMF_SUCCESS != result)\
	{\
		UT_LOG("Aborting test - unable to open capture interface. Error code: %d", result);\
		UT_FAIL_FATAL("Aborting test - unable to open capture.");\
	}\
	} while(0)

#define VALIDATE_SUCCESSFUL_START(result, handle) do{\
	if(RMF_SUCCESS != result)\
	{\
		UT_LOG("Aborting test - unable to start capture interface. Error code: %d", result);\
		result = RMF_AudioCapture_Close(handle);\
		UT_FAIL_FATAL("Aborting test - unable to start capture.");\
	}\
	} while(0)

#define SOFT_ASSERT_EQUAL(left, right) do{\
	if(left != right)\
	{\
		UT_LOG("Soft assert failed.");\
		UT_FAIL("Soft assert failed.");\
	}\
	} while(0)

#endif //__TEST_LX_RMFAUDIOCAPTURE_HELPER_H__
