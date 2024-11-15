#!/usr/bin/env python3
#** *****************************************************************************
# *
# * If not stated otherwise in this file or this component's LICENSE file the
# * following copyright and licenses apply:
# *
# * Copyright 2024 RDK Management
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *
# http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *
#* ******************************************************************************

import os
import sys
import time

dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse
from raft.framework.core.logModule import logModule
from rmfAudioClasses.rmfAudio import rmfAudioClass

class rmfAudioHelperClass(utHelperClass):
    """
    A helper class to support RMF audio capture testing functionalities, including device setup,
    configuration loading, asset management, and user interactions for manual test validation.

    This class is initialized with test-specific parameters and includes methods to:
    - Download test artifacts
    - Run prerequisite and post-requisite commands
    - Interact with the RMF audio capture device for manual checks
    - Manage test assets on the Device Under Test (DUT)
    """

    def __init__(self, testName:str, qcId:str, log:logModule=None ):
        """
        Initializes the rmfAudioHelperClass with test-specific details, including test name,
        configuration paths, and logging setup.

        Args:
            testName (str): Name of the test.
            qcId (str): Quality control ID for the test.
            log (logModule, optional): Logging module instance. Defaults to None.
        """
        self.testName  = ""
        self.testSetupPath = os.path.join(dir_path, "rmfAudio_L3_testSetup.yml")
        self.moduleName = "rmfaudiocapture"
        self.rackDevice = "dut"

        super().__init__(testName, qcId, log)

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)

        # Open Session for hal test
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")

        cmds = self.testSetup.get("assets").get("device").get(self.testName).get("postcmd")
        if cmds is not None:
            for cmd in cmds:
                self.hal_session.write(cmd)

        # Set up paths and URLs for device test setup
        deviceTestSetup = self.cpe.get("test")

         # Create user response Class
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.moduleConfigProfileFile = os.path.join(dir_path, deviceTestSetup.get("profile"))
        self.targetWorkspace = self.cpe.get("target_directory")
        self.targetWorkspace = os.path.join(self.targetWorkspace, self.moduleName)

    def testDownloadAssets(self):
        """
        Downloads the test artifacts and streams listed in the test setup configuration.

        This function retrieves audio streams and other necessary files and
        saves them on the DUT (Device Under Test).

        Args:
            None
        """

        # List of streams with path
        self.testStreams = []

        self.deviceDownloadPath = self.cpe.get("target_directory")

        test = self.testSetup.get("assets").get("device").get(self.testName)

        # Download test artifacts to device
        url = test.get("artifacts")
        if url is not None:
            self.downloadToDevice(url, self.deviceDownloadPath, self.rackDevice)

        # Download test streams to device
        url =  test.get("streams")
        if url is not None:
            self.downloadToDevice(url, self.deviceDownloadPath, self.rackDevice)
            for streampath in url:
                self.testStreams.append(os.path.join(self.deviceDownloadPath, os.path.basename(streampath)))

    def testCleanAssets(self):
        """
        Removes the downloaded assets and test streams from the DUT after test execution.

        Args:
            None
        """
        #self.deleteFromDevice(self.testStreams)

    def testRunPrerequisites(self):
        """
        Executes prerequisite commands listed in the test setup configuration file on the DUT.

        Args:
            None
        """
        # Run commands as part of test prerequisites
        test = self.testSetup.get("assets").get("device").get(self.testName)
        cmds = test.get("execute")
        if cmds is not None:
            for cmd in cmds:
                self.writeCommands(cmd)

    def testPrepareFunction(self):
        """
        Prepares the environment and assets required for the test.

        This function:
        - Downloads the required assets.
        - Runs the prerequisite commands.
        - Creates dsHdmiInClass

        Returns:
            bool
        """

        # Download the assets listed in test setup configuration file
        self.testDownloadAssets()

        # Run Prerequisites listed in the test setup configuration file
        self.testRunPrerequisites()

        # Create the rmfaudiocapture class
        self.testrmfAudio = rmfAudioClass(self.moduleConfigProfileFile, self.hal_session)

        return True

    def testEndFunction(self, powerOff=True):
        # Clean the assets downloaded to the device
        self.testCleanAssets()

        # Clean up the rmfAudio instance
        del self.testrmfAudio

    def testExceptionCleanUp (self):
        # Clean the assets downloaded to the device
        self.testCleanAssets()