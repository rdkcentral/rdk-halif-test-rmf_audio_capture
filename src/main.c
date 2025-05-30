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

#ifndef HALIF_TEST_TAG_VERSION
#define HALIF_TEST_TAG_VERSION "Not Defined"
#endif

extern int UT_register_tests( void );

int main(int argc, char** argv)
{
    printf("\n\n==========================================================================\n");
    printf("\n\t\tRMF Audio Capture HALIF Test Version: \033[0;32m%s\033[0m\n",HALIF_TEST_TAG_VERSION);
    printf("\n==========================================================================\n\n");
    int registerFailed = 0;
    /* Register tests as required, then call the UT-main to support switches and triggering */
    UT_init( argc, argv );

    registerFailed = UT_register_tests();
    if (registerFailed == 1)
    {
        UT_FAIL(" Failed to register hal tests");
        return -1;
    }
    
    /* Begin test executions */
    UT_run_tests();
    return 0;
}
