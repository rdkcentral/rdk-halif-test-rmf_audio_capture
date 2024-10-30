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

class rmfAudio_test03_independentDataCheck(utHelperClass):
    """
    Test class to test primary and auxiliary captures run independently.

    This class uses the `rmfAudioClass` to test primary and auxiliary data capture,
    downloading necessary test assets, setting up RMF Audio Capture
    and performing both capture interfaces run independently.
    """

    # Class variables
    testName  = "test03_independentDataCheck"
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
        The main test function that verifies primary and auxiliary data captures are independent.

        This function:
        - Downloads the required assets.
        - Runs the prerequisite commands.
        - Starts Primary and Auxiliary Data captures.
        - Tracks bytes received for both interfaces
        - Writes the captured audio data to a wav file.
        - Stops, starts in a mixed sequence to make sure the bytes received shows capture is independent

        Returns:
            bool: Final result of the test.
        """

        # Download the assets listed in test setup configuration file
        self.testDownloadAssets()

        # Run Prerequisites listed in the test setup configuration file
        self.testRunPrerequisites()

        # Create the rmfAudio class
        self.testrmfAudio = rmfAudioClass(self.deviceProfile, self.hal_session)

        self.log.testStart(self.testName, '3')

        # 1 for primary data capture (default), 2 for auxiliary data capture.
        capture_type_primary = 1
        capture_type_auxiliary = 2
        sleep_duration = 2 #2 seconds to test
        settings_update = 0 # settings_update=0 (no updates to default settings), capture_format=1, sampling_rate=1, fifo_size=1, threshold=1
        # For settings update for example capture_format, set settings_update to 1, followed by value for capture_format (1 for racFormat_e16BitStereo)
        # sample : self.testrmfAudio.updateSettings(capture_type, 1, 1)
        test_type = 1 # test_type : 1 for byte counting tests (default), 2 for data tracking tests where audio data is captured.
        aux_support = self.testrmfAudio.checkAuxiliarySupport()

        if aux_support is True:
            self.log.stepStart('Independent data test - check auxiliary runs independently')
            self.testrmfAudio.openHandle(capture_type_primary)
            # Retain default Settings in this sample
            self.testrmfAudio.updateSettings(capture_type_primary, settings_update)
            self.testrmfAudio.selectTestType(capture_type_primary, test_type)

            self.testrmfAudio.openHandle(capture_type_auxiliary)
            # Retain default Settings in this sample
            self.testrmfAudio.updateSettings(capture_type_auxiliary, settings_update)
            self.testrmfAudio.selectTestType(capture_type_auxiliary, test_type)

            self.testrmfAudio.startCapture(capture_type_primary)
            self.testrmfAudio.startCapture(capture_type_auxiliary)
            
            time.sleep(sleep_duration)
            self.testrmfAudio.stopCapture(capture_type_primary)
            #Primary stopped, auxiliary running. Verify if bytes received for auxiliary has increased.
            primary_bytes_received_before, auxiliary_bytes_received_before = self.testrmfAudio.checkBytesReceived()
            time.sleep(sleep_duration)

            primary_bytes_received_after, auxiliary_bytes_received_after = self.testrmfAudio.checkBytesReceived()
            if (primary_bytes_received_before == primary_bytes_received_after) and (auxiliary_bytes_received_before < auxiliary_bytes_received_after):
                result = True
            else:
                result = False
            
            self.log.stepResult(result, 'Independent data test - check auxiliary runs independently')

            self.testrmfAudio.stopCapture(capture_type_auxiliary)
            self.log.stepStart('Independent data test - check primary runs independently')

            self.testrmfAudio.startCapture(capture_type_primary)
            self.testrmfAudio.startCapture(capture_type_auxiliary)

            time.sleep(sleep_duration)

            self.testrmfAudio.stopCapture(capture_type_auxiliary)
            #Primary running, auxiliary stopped. Verify if bytes received for primary has increased
            primary_bytes_received_before, auxiliary_bytes_received_before = self.testrmfAudio.checkBytesReceived()
            time.sleep(sleep_duration)

            primary_bytes_received_after, auxiliary_bytes_received_after = self.testrmfAudio.checkBytesReceived()
            if (primary_bytes_received_before < primary_bytes_received_after) and (auxiliary_bytes_received_before == auxiliary_bytes_received_after):
                result = True
            else:
                result = False

            self.log.stepResult(result, 'Independent data test - check primary runs independently')
            self.testrmfAudio.stopCapture(capture_type_primary)

            self.testrmfAudio.closeHandle(capture_type_primary)
            self.testrmfAudio.closeHandle(capture_type_auxiliary)
        else:
            print(f'Auxiliary support in configuration file is : {aux_support}. Not running test')
            
        # Clean the assets downloaded to the device
        self.testCleanAssets()

        # Clean up the rmfAudio instance
        del self.testrmfAudio

        return result

if __name__ == '__main__':
    test = rmfAudio_test03_independentDataCheck()
    test.run(False)