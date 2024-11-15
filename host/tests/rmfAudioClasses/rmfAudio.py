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

import yaml
import os
import sys
from enum import Enum, auto
import re
import librosa
import numpy as np
import time
import paramiko
import io

# Add parent directory to the system path for module imports
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(dir_path, "../"))

from raft.framework.plugins.ut_raft.configRead import ConfigRead
from raft.framework.plugins.ut_raft.utSuiteNavigator import UTSuiteNavigatorClass
from raft.framework.plugins.ut_raft.interactiveShell import InteractiveShell
from raft.framework.plugins.ut_raft.utBaseUtils import utBaseUtils

class rmfAudioClass():
    """
    RMF Audio Capture Class.

    This module provides common functionalities and extensions for RMF Audio Capture Module.
    """

    def __init__(self, moduleConfigProfileFile:str, session=None, targetWorkspace="/tmp" ):
        """
        Initializes the rmfAudioClass instance with configuration settings.

        Args:
            moduleConfigProfileFile (str): Path to the device profile configuration file.
            session: Optional; session object for the user interface.

        Returns:
            None
        """
        self.moduleName = "rmfaudiocapture"
        self.testConfigFile = os.path.join(dir_path, "rmfAudio_testConfig.yml")
        self.testSuite = "L3 rmfAudioCapture"

        # Load configurations for device profile and menu
        self.moduleConfigProfile = ConfigRead( moduleConfigProfileFile , self.moduleName)
        self.testConfig    = ConfigRead(self.testConfigFile, self.moduleName)
        self.testConfig.test.execute = os.path.join(targetWorkspace, self.testConfig.test.execute)
        self.utMenu        = UTSuiteNavigatorClass(self.testConfig, None, session)
        self.testSession   = session
        self.utils         = utBaseUtils()

        for artifact in self.testConfig.test.artifacts:
            filesPath = os.path.join(dir_path, artifact)
            self.utils.rsync(self.testSession, filesPath, targetWorkspace)

        # Start the user interface menu
        self.utMenu.start()

    def searchPattern(self, haystack, pattern):
        """
        Searches for the first occurrence of a specified pattern in the provided string.

        Args:
            haystack (str): The string to be searched.
            pattern (str): The regular expression pattern to search for.

        Returns:
            str: The first capturing group of the match if found; otherwise, None.

        Notes:
            - If no match is found, None is returned.
        """
        match = re.search(pattern, haystack)
        if match:
            return match.group()
        return None

    def checkAuxiliarySupport(self):
        """
        Check auxiliary interface support based on profile file

        Args:
            None

        Returns:
            bool : true/false based on auxsupport value in profile file yaml
        """
        return self.moduleConfigProfile.get("features").get("auxsupport")

    def openHandle(self, capture_type:int=1):
        """
        Opens the RMF audio capture interface and gets a handle.

        Args:
            capture_type (int, optional):
                - 1 for primary data capture (default).
                - 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]
        result = self.utMenu.select( self.testSuite, "Open RMF Audio Capture Handle", promptWithAnswers)

    def closeHandle(self, capture_type:int=1):
        """
        Closes RMF Audio Capture interface.

        Args:
            capture_type (int, optional):
                - 1 for primary data capture (default).
                - 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]
        result = self.utMenu.select(self.testSuite, "Close RMF Audio Capture Handle", promptWithAnswers)

    def updateSettings(self, capture_type:int=1, settings_update:int=0, capture_format:int=1, sampling_rate:int=5, fifo_size:int=65536, threshold:int=8192):
        """
        Update RMF_AudioCapture_Settings values if required.

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            settings_update (int, optional): Choice whether default settings needs to be updated (default is 0).
            capture_format (int, optional): RMF Audio Capture Format for audio data (default is racFormat_e16BitStereo i.e 1).
            sampling_rate (int, optional): RMF Audio Capture Sampling Rate (default is racFreq_e48000 i.e 5).
            fifo_size (int, optional): FIFO size in bytes (default is 65k, 65536)
            threshold (int, optional): FIFO data callback threshold in bytes (default is 8k, 8192)

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Do you want to update default settings ? (0 for No, 1 for Yes)",
                    "input": str(settings_update)
                }
        ]

        # If the user chooses update settings, prompt for settings update
        if settings_update == 1:
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Select the capture format to update, use -1 to retain default value :",
                    "input": str(capture_format)
                })
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Select the Sampling Rate, use -1 to retain default value :",
                    "input": str(sampling_rate)
                })
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Enter FIFO size in bytes, use -1 to retain default value :",
                    "input": str(fifo_size)
                })
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Enter data callback threshold in bytes, used to check jitter (max 1/4th of FIFO), use -1 to retain default value :",
                    "input": str(threshold)
                })

        result = self.utMenu.select(self.testSuite, "Update settings", promptWithAnswers)

    def selectTestType(self, capture_type:int=1, test_type:int=1, datacapture_duration:int=10):
        """
        Selects the type of test, to set up data callbacks for counting, capture tests

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            test_type (int, optional): Type of test about to be run : 1 for byte counting tests (default), 2 for data tracking tests where audio data is captured.
            datacapture_duration (int, optional) : Test duration in seconds to capture audio data (default is 10 seconds)

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Select the type of test: ",
                    "input": str(test_type)
                },
        ]

        if test_type == 2:
            promptWithAnswers.append(
                {
                    "query_type": "direct",
                    "query": "Enter test duration in seconds for data capture test :",
                    "input": str(datacapture_duration)
                })

        result = self.utMenu.select(self.testSuite, "Select the type of test", promptWithAnswers)

    def startCapture(self, capture_type:int=1):
        """
        Starts RMF Audio Capture

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Start RMF Audio Capture", promptWithAnswers)

    def checkBytesReceived(self):
        """
        Checks bytes received for both primary and auxiliary data captures

        Args:
            None

        Returns:
            int, int(optional) : Bytes received from primary data capture. Included auxiliary data capture if it is supported.
        """
        result = self.utMenu.select(self.testSuite, "Check Bytes Received")
        bytes_received_string = r"Bytes Received for \w+ capture (\d+)(?:[^0-9]*Bytes Received for \w+ capture (\d+))?"

        matches = re.findall(bytes_received_string, result)
        primary_capture = matches[0] if len(matches) > 0 else None
        auxiliary_capture = matches[1] if len(matches) > 1 else None
    
        return primary_capture, auxiliary_capture

    def writeWavFile(self, capture_type:int=1, file_path:str="/tmp/output.wav"):
        """ 
        Writes captured audio data to a wav file

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            file_path (string, optional): File name and path of the output wav file to create (default is "/tmp/output.wav").

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Enter file name and location to create output filename (example - /tmp/output.wav) :",
                    "input": str(file_path)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Write output wav file", promptWithAnswers)

    def startJitterTest(self, capture_type:int=1, threshold:int=16384, jitter_interval:int=100000, jitter_test_duration:int=120):
        """
        Starts jitter test with required parameters for test

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.
            threshold (int, optional): Threshold to check jitter against (default is 16k, 16384).
            jitter_interval (int, optional): Interval in microseconds to monitor buffer for jitter (default is 100ms, 100000).
            jitter_test_duration (int, optional): Test duration in seconds for jitter test (default is 120 seconds).

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                },
                {
                    "query_type": "direct",
                    "query": "Enter minimum threshold in bytes to check jitter : ",
                    "input": str(threshold)
                },
                {
                    "query_type": "direct",
                    "query": "Enter interval in microseconds to monitor buffer for jitter : ",
                    "input": str(jitter_interval)
                },
                {
                    "query_type": "direct",
                    "query": "Enter test duration in seconds for jitter test : ",
                    "input": str(jitter_test_duration)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Start Jitter test", promptWithAnswers)

    def checkJitterTestResult(self, capture_type:int=1):
        """
        Gets the result of jitter test

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            bool : true/false based on jitter detected strings from C-test
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Check jitter test result", promptWithAnswers)
        jitter_pattern = "Jitter Detected !"

        jitter_result = self.searchPattern(result, jitter_pattern)
        if jitter_result:
            return False
        else:
            return True

    def stopCapture(self, capture_type:int=1):
        """
        Stops audio capture

        Args:
            capture_type (int, optional): 1 for primary data capture (default), 2 for auxiliary data capture.

        Returns:
            None
        """
        promptWithAnswers = [
                {
                    "query_type": "direct",
                    "query": "Select the audio capture type :",
                    "input": str(capture_type)
                }
        ]

        result = self.utMenu.select(self.testSuite, "Stop RMF Audio Capture", promptWithAnswers)

    def extract_pitch(self, y):
        """
        Extracts the pitch from an audio signal using the YIN algorithm.
    
        This function applies the YIN algorithm to estimate the pitch 
        of the given audio signal over time. The pitch values are returned 
        as a NumPy array.

        Args:
            y (NumPy array) : The audio time series (waveform) as a 1D NumPy array.

        Returns:
            A NumPy array containing the estimated pitch values 
                (in Hz) for each frame. Frames with no detected pitch 
                will be represented as NaN.
        """
        # Apply the YIN algorithm to estimate the pitch
        pitches = librosa.yin(y, fmin=librosa.note_to_hz('C2'), fmax=librosa.note_to_hz('C7'))
        
        # Use NaN for frames with no detected pitch
        return np.array(pitches)

    def compare_pitches(self, pitch1, pitch2):
        """
        Compares two pitch arrays and calculates the correlation.
    
        This function aligns two pitch arrays to the same length and 
        computes the Pearson correlation coefficient between them.

        Args:
            pitch1, pitch2 (NumPy array) : contains the pitch values from two audio files

        Returns:
            correlation: The Pearson correlation coefficient between the two pitch 
                arrays, ranging from -1 to 1, where 1 indicates a perfect 
                positive correlation, -1 indicates a perfect negative 
                correlation, and 0 indicates no correlation.
        """
        # Align lengths
        min_length = min(len(pitch1), len(pitch2))
        pitch1 = pitch1[:min_length]
        pitch2 = pitch2[:min_length]

        # Calculate correlation or mean squared error
        correlation = np.corrcoef(pitch1, pitch2)[0, 1]
        return correlation

    def compareWavFiles(self, url, file_path):
        """
        Compares the pitch of two audio files and determines if they match.

        This function loads two audio files from remote location, extracts their pitch using the YIN algorithm,
        and calculates the correlation between the extracted pitches. If the correlation 
        exceeds a specified threshold, it concludes that the audio files match.

        Args:
            file1, file2: The paths to audio files for comparison.

        Returns:
            None
        """
        threshold=0.95 # Set threshold for comparison

        host = self.testSession.address
        port = self.testSession.port
        username = self.testSession.username
        password = self.testSession.password

        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(hostname=host, port=port, username=username, password=password)

        sftp = ssh.open_sftp()

        with sftp.open(url, 'r') as remote_file:
            ref_content = remote_file.read()

        with sftp.open(file_path, 'r') as remote_file:
            wav_content = remote_file.read()

        sftp.close()
        ssh.close()

        # Create a BytesIO object from the bytes
        wav_file_content = io.BytesIO(wav_content)
        ref_file_content = io.BytesIO(ref_content)

        # Load the audio files
        y1, sr1 = librosa.load(ref_file_content)
        y2, sr2 = librosa.load(wav_file_content)

        # Extract pitch from both audio files
        pitch1 = self.extract_pitch(y1)
        pitch2 = self.extract_pitch(y2)

        # Compare pitches
        correlation = self.compare_pitches(pitch1, pitch2)
        
        # Determine if they match based on the threshold
        if correlation >= threshold:
            print(f"The audio files match with a correlation of {correlation:.2f}")
            return True
        else:
            print(f"The audio files do not match (correlation: {correlation:.2f})")
            return False

    def __del__(self):
        """
        Cleans up and de-initializes the dsAudio helper by stopping the test menu.

        Args:
            None.

        Returns:
            None
        """
        self.utMenu.stop()

# Test and example usage code
if __name__ == '__main__':

    shell = InteractiveShell()
    shell.open()

    platformProfile = dir_path + "/../../../profiles/rmfAudioCaptureAuxSupported.yaml"
    # test the class assuming that it's optional
    test = rmfAudioClass(platformProfile, shell)

    test.openHandle(1)

    test.closeHandle(1)

    shell.close()
