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

from rmfAudioClasses.rmfAudio import rmfAudioClass
from raft.framework.plugins.ut_raft import utHelperClass
from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utUserResponse import utUserResponse

class rmfAudio_test06_auxiliaryJitterTest(utHelperClass):
    """
    Test class to open capture auxiliary Audio Data and check if there is jitter detected.

    This class uses the `rmfAudioClass` to run auxiliary data capture,
    downloading necessary test assets, setting up RMF Audio Capture
    and performing verification of audio capture rate.
    """

    # Class variables
    testName  = "test06_auxiliaryJitterTest"
    testSetupPath = dir_path + "/rmfAudio_L3_testSetup.yml"
    moduleName = "rmfaudiocapture"
    rackDevice = "dut"

    def __init__(self):
        """
        Initializes the test class with test name, setup configuration, and sessions for the device.

        Args:
            None
        """
        super().__init__(self.testName, '1')

        # Load test setup configuration
        self.testSetup = ConfigRead(self.testSetupPath, self.moduleName)

        # Open Session for hal test
        self.hal_session = self.dut.getConsoleSession("ssh_hal_test")

        cmds = self.testSetup.get("assets").get("device").get(self.testName).get("postcmd")
        if cmds is not None:
            for cmd in cmds:
                self.hal_session.write(cmd)

         # Create user response Class
        self.testUserResponse = utUserResponse()

        # Get path to device profile file
        self.deviceProfile = dir_path + "/" + self.cpe.get("test").get("profile")

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
        self.deleteFromDevice(self.testStreams)

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

    def testFunction(self):
        """
        The main test function that verifies auxiliary data capture with input audio.

        This function:
        - Downloads the required assets.
        - Runs the prerequisite commands.
        - Starts auxiliary Data capture.
        - Tracks bytes received for Audio data on auxiliary interface.
        - Monitors bytes received in regular intervals

        Returns:
            bool: Final result of the test.
        """

        # Download the assets listed in test setup configuration file
        self.testDownloadAssets()

        # Run Prerequisites listed in the test setup configuration file
        self.testRunPrerequisites()

        # Create the rmfAudio class
        self.testrmfAudio = rmfAudioClass(self.deviceProfile, self.hal_session)

        self.log.testStart(self.testName, '6')

        # 1 for primary data capture (default), 2 for auxiliary data capture.
        capture_type = 2
        jitter_test_duration = 120 # jitter test duration in seconds
        threshold = 16384 # 16K
        jitter_interval = 100000 # 100ms
        settings_update = 0 # settings_update=0 (no updates to default settings), capture_format=1, sampling_rate=1, fifo_size=1, threshold=1
        # For settings update for example capture_format, set settings_update to 1, followed by value for capture_format (1 for racFormat_e16BitStereo)
        # sample : self.testrmfAudio.updateSettings(capture_type, 1, 1)
        test_type = 1 # test_type : 1 for byte counting tests (default), 2 for data tracking tests where audio data is captured.

        self.log.stepStart('Auxiliary jitter test')
        self.testrmfAudio.openHandle(capture_type)
        # Retain default Settings in this sample
        self.testrmfAudio.updateSettings(capture_type, settings_update)
        self.testrmfAudio.selectTestType(capture_type, test_type)
        self.testrmfAudio.startCapture(capture_type)

        time.sleep(0.1)
        self.testrmfAudio.startJitterTest(capture_type, threshold, jitter_interval, jitter_test_duration)
        
        time.sleep(jitter_test_duration)
        result = self.testrmfAudio.checkJitterTestResult(capture_type)
        self.testrmfAudio.stopCapture(capture_type)
        self.testrmfAudio.closeHandle(capture_type)
        self.log.stepResult(result, 'Auxiliary jitter test')

        # Clean the assets downloaded to the device
        self.testCleanAssets()

        # Terminate rmfAudio Module
        self.testrmfAudio.terminate()

        # Clean up the rmfAudio instance
        del self.testrmfAudio

        return result

if __name__ == '__main__':
    test = rmfAudio_test06_auxiliaryJitterTest()
    test.run(False)