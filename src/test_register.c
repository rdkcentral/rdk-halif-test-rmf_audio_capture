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

#include <ut.h>

/* L1 Testing Functions */
extern int test_l1_rmfAudioCapture_register();
/* L2 Testing Functions */
extern int test_rmfAudioCapture_l2_register(void);
/* L3 Testing Functions */
extern int test_rmfAudioCapture_l3_register(void);

int UT_register_tests ( void )
{
    int registerFailed=0;

    /* Check if tests are registered successfully */
    registerFailed = test_l1_rmfAudioCapture_register();
    if (registerFailed == 0)
    {
        UT_LOG("test_l1_rmfAudioCapture_register() returned success\n");
    }
    else
    {
        UT_FAIL("test_l1_rmfAudioCapture_register() returned failure\n");
        return 1;
    }

    registerFailed = test_rmfAudioCapture_l2_register();
    if (registerFailed == 0)
    {
        UT_LOG("test_rmfAudioCapture_l2_register() returned success\n");
    }
    else
    {
        UT_FAIL("test_rmfAudioCapture_l2_register() returned failure\n");
        return 1;
    }

    registerFailed = test_rmfAudioCapture_l3_register();
    if (registerFailed == 0)
    {
        UT_LOG("test_rmfAudioCapture_l3_register() returned success\n");
    }
    else
    {
        UT_FAIL("test_rmfAudioCapture_l3_register() returned failure\n");
        return 1;
    }

    return registerFailed;
}
