# RMF Audio Capture HAL L3 Python Test Procedure

## Table of Contents

  - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
  - [Setting Up Test Environment](#setting-up-test-environment)
  - [Streams Required](#streams-required)
  - [Test Cases](#test-cases)
    - [rmfAudio\_test01\_primaryDataCapture.py](#rmfaudio_test01_primarydatacapturepy)
    - [rmfAudio\_test02\_primaryJitterTest.py](#rmfaudio_test02_primaryjittertestpy)
    - [rmfAudio\_test03\_independentDataCheck.py](#rmfaudio_test03_independentdatacheckpy)
    - [rmfAudio\_test04\_auxiliaryDataCapture.py](#rmfaudio_test04_auxiliarydatacapturepy)
    - [rmfAudio\_test05\_combinedDataCapture.py](#rmfaudio_test05_combineddatacapturepy)
    - [rmfAudio\_test06\_auxiliaryJitterTest.py](#rmfaudio_test06_auxiliaryjittertestpy)
    - [rmfAudio\_test07\_combinedJitterTest.py](#rmfaudio_test07_combinedjittertestpy)
    - [rmfAudio\_L3\_Runall.py](#rmfaudio_l3_runallpy)

## Acronyms, Terms and Abbreviations

- `HAL`    - Hardware Abstraction Layer
- `L3`     - Level 3 Testing
- `DUT`    - Device Under Test
- `RAFT`   - Rapid Automation Framework for Testing
- `YAML`   - YAML Ain't Markup Language

## Setting Up Test Environment

To execute `HAL` `L3` Python test cases, need a Python environment. Follow these steps mentioned in [HPK Public Documentation](https://github.com/rdkcentral/rdk-hpk-documentation/blob/main/README.md)

### Update Configuration Files

#### Rack Configuration File

Example Rack configuration File: [example_rack_config.yml](../../../../ut/host/tests/configs/example_rack_config.yml)

For more details refer [RAFT](https://github.com/rdkcentral/python_raft/blob/1.0.0/README.md) and [example_rack_config.yml](https://github.com/rdkcentral/python_raft/blob/1.0.0/examples/configs/example_rack_config.yml)

In this file, update the configuration to define the console sessions for the `DUT` and the outbound settings:

| Console Session | Description                                   |
| --------------- | --------------------------------------------- |
| default         | Downloads the streams required for test cases |
| ssh_player      | Plays the stream required for test case       |
| ssh_hal_test    | Executes the `HAL` binary for the test case   |

```yaml
rackConfig:
  - dut:
      ip: "XXX.XXX.XXX.XXX"  # IP Address of the device
      description: "stb device under test"
      platform: "stb"
      consoles:
        - default:
            type: "ssh"
            port: 10022
            username: "root"
            ip: "XXX.XXX.XXX" # IP address of the device
            password: ' '
        - ssh_player:
            type: "ssh"
            port: 10022
            username: "root"
            ip: "XXX.XXX.XXX" # IP address of the device
            password: ' '
        - ssh_hal_test:
            type: "ssh"
            port: 10022
            username: "root"
            ip: "XXX.XXX.XXX" # IP address of the device
            password: ' '
      outbound:
        download_url: "tftp://tftp-server.com/rack1/slot1/"    # Download location for the CPE device
        upload_url: "sftp://server-address/home/workspace/tftp/rack1/slot1/" # Upload location
        upload_url_base_dir: "sftp://server-address/home/workspace/tftp/rack1/slot1"
        httpProxy:   # Local proxy if required
        workspaceDirectory: './logs/workspace'   # Local working directory
```

#### Device Configuration File

Example Device configuration File: [deviceConfig.yml](ut/host/tests/configs/deviceConfig.yml)

For more details refer [RAFT](https://github.com/rdkcentral/python_raft/blob/1.0.0/README.md) and [example_device_config.yml](https://github.com/rdkcentral/python_raft/blob/1.0.0/examples/configs/example_device_config.yml)

Update below fileds in the device configuration file:
- Set the folder path for `target_directory` where `HAL` binaries will be copied onto the device.
- Specify the device profile path in `test/profile`
- Update `streams_download_url` with the URL from which the streams will be downloaded
- Ensure the `platform` should match with the `DUT` `platform` in [Rack Configuration](#rack-configuration-file)

```yaml
deviceConfig:
    cpe1:
        platform: "linux"
        model: "uk"
        soc_vendor: "intel"
        target_directory: "/tmp/"  # Target Directory on device
        prompt: "" # Prompt string on console
        test:
            profile: "../../../profiles/rmfAudioCaptureAuxSupported.yaml"
            streams_download_url: "<URL_Path>" #URL path from which the streams are downloaded to the device
```

#### Test Setup Configuration File

Example Test Setup configuration File: [rmfAudio_L3_testSetup.yml](../../../../ut/host/tests/rmfAudio_L3_TestCases/rmfAudio_L3_testSetup.yml)

Provide the streams for each test case. This path is appended with `streams_download_url` entry from [Device Configuration File](#device-configuration-file)

If a test case requires multiple streams or needs to be validated using several streams, ensure that all necessary streams are added sequentially for that specific test case.

```yaml
rmfaudiocapture:
  description: "RMF Audio Capture test setup"
  assets:
    device:
      test01_primaryDataCapture:
        postcmd:  #Setting this env. variable is required to run with mock implementation
          - "export INPUT_PRIMARY=<PATH on Device>/Sin_120s_48k_stereo.wav"
        streams:
          - "streams/Sin_120s_48k_stereo.wav"
      test02_primaryJitterTest:
        postcmd:  #Setting this env. variable is required to run with mock implementation
          - "export INPUT_PRIMARY=<PATH on Device>/Sin_120s_48k_stereo.wav"
        streams:
          - "streams/Sin_120s_48k_stereo.wav"
      test03_independentDataCheck:
        postcmd:  #Setting this env. variable is required to run with mock implementation
          - "export INPUT_PRIMARY=<PATH on Device>/Sin_10s_48k_stereo.wav"
          - "export INPUT_AUXILIARY=<PATH on Device>/Triangle_10s_480k_stereo.wav"
        streams:
          - "streams/Sin_10s_48k_stereo.wav"
          - "streams/Triangle_10s_480k_stereo.wav"
      test04_auxiliaryDataCapture:
        postcmd:  #Setting this env. variable is required to run with mock implementation
          - "export INPUT_AUXILIARY=<PATH on Device>/Triangle_10s_480k_stereo.wav"
        streams: 
          - "streams/Triangle_10s_480k_stereo.wav"
```

#### Test Configuration

Example Test Setup configuration File: [rmfAudio_testConfig.yml](../../../../ut/host/tests/rmfAudioClasses/rmfAudio_testConfig.yml)

Update the execute command according to the device path where `HAL` binaries are copied.

```yaml
rmfaudiocapture:
    description: "RMF Audio Capture testing profile / menu system for UT"
    test:
        artifacts:
        #List of artifacts folders, test class copies the content of folder to the target device workspace
          - "../../../bin/"
        # exectute command, this will appended with the target device workspace path
        execute: "run.sh"
        type: UT-C # C (UT-C Cunit) / C++ (UT-G (g++ ut-core gtest backend))
```

## Run Test Cases

Once the environment is set up, you can execute the test cases with the following command

```bash
python <TestCaseName.py> --config </PATH>/ut/host/tests/configs/example_rack_config.yml --deviceConfig </PATH>/ut/host/tests/configs/deviceConfig.yml
```

## Streams Required

Refer [rmf-audio-capture_L3_Low-Level_TestSpecification.md](./rmf-audio-capture_L3_Low-Level_TestSpecification.md#audio-streams-requirement) for the stream details

## Test Cases

### rmfAudio_test01_primaryDataCapture.py

#### User Input Required - test01

**No**

#### Acceptance Criteria - test01

Play **Stream #1** and confirm that the captured audio matches with played reference stream.

#### Expected Results - test01

The test plays a reference stream, captures audio data on primary capture interface and verifies that the captured data matches with reference stream.

Success Criteria

- Audio data should be captured on primary capture interface and written to a wav output file.
- Captured audio should match with played reference stream.

#### Test Steps - test01

- Run the Test:

  - Select and run the Python script: **`rmfAudio_test01_primaryDataCapture.py`**

- Download and Setup:

  The test will automatically download all necessary artifacts and streams, and copy them to the target directory on the device.

- Primary capture interface data Verification:

    The test will play the designated audio stream and start audio capture on primary capture interface.

  - The test will validate captured audio data with reference audio stream with a given threshold.
  - If the files are a match within given threshold, the step is marked as PASS.
  - If the files are not a match within given threshold, the step is marked as FAIL.

- Completion and result:

  Upon playing a designated audio stream and capturing audio data through primary capture interface, the test will conclude and present a final result: PASS or FAIL based on file comparison between capture audio data and played stream.

### rmfAudio_test02_primaryJitterTest.py

#### User Input Required - test02

**No**

#### Acceptance Criteria - test02

Play **Stream #1** and confirm that there is no jitter detected on primary capture interface.

#### Expected Results - test02

The test plays a reference stream, checks bytes of audio data received on primary capture interface and verifies there is no jitter detected on primary capture interface.

#### Test Steps - test02
- Run the Test:

  - Select and run the Python script: **`rmfAudio_test02_primaryJitterTest.py`**

- Download and Setup:

  The test will automatically download all necessary artifacts and streams, and copy them to the target directory on the device.

- Primary capture jitter Verification:

    The test will play the designated audio stream and start monitoring bytes received on primary capture interface.

  - The test will monitor bytes received on primary audio capture continuously against a set threshold.
  - If bytes captured are greater than given threshold, the step is marked as PASS.
  - If bytes captured are less than given threshold, the step is marked as FAIL.

- Completion and Result:

  Upon playing a designated audio stream and monitoring bytes received on primary data interface, the test will conclude and present a final result: PASS or FAIL based on whether bytes captured were within a given threshold.

### rmfAudio_test03_independentDataCheck.py

#### User Input Required - test03

**No**

#### Acceptance Criteria - test03

Play **Stream #1** and **Stream #2** and verify that primary and auxiliary data captures work independent of each other.

#### Expected Results - test03

The test plays a reference stream each on primary and auxiliary interfaces, then issues a series of start and stop calls in a mixed sequence that verifies that primary and audio capture sessions are truly independent of each other and free of side-effects when the other is started or stopped.

#### Test Steps - test03

- Run the Test:

  - Select and run the Python script: **`rmfAudio_test03_independentDataCheck.py`**

- Download and Setup:

  The test will automatically download all necessary artifacts and streams, and copy them to the target directory on the device.

- Independent data check Verification:

    The test will play the designated audio stream on both primary and auxiliary interfaces and start monitoring bytes received before and after each start and stop call.

  - The test will check bytes received on primary and auxiliary audio capture during series of start and stop calls in a mixed sequence. 
  - If bytes captured shows that the captures are independent of each other, the step is marked as PASS.
  - If bytes captured shows that the captures are dependent on each other, the step is marked as FAIL.

- Completion and Result:

  Upon playing a designated audio stream each on primary and auxiliary interfaces, the test will conclude and present a final result: PASS or FAIL based on whether bytes captured shows that the captures are independent of each other

### rmfAudio_test04_auxiliaryDataCapture.py

#### User Input Required - test04

**No**

#### Acceptance Criteria - test04

Play **Stream #2** and confirm that the captured audio matches with played reference stream.

#### Expected Results - test04

The test plays a reference stream, captures audio data on auxiliary capture interface and verifies that the captured data matches with reference stream.

Success Criteria

- Audio data should be captured on auxiliary capture interface and written to a wav output file.
- Captured audio should match with played reference stream.

#### Test Steps - test04

- Run the Test:

  - Select and run the Python script: **`rmfAudio_test04_auxiliaryDataCapture.py`**

- Download and Setup:

  The test will automatically download all necessary artifacts and streams, and copy them to the target directory on the device.

- Auxiliary capture interface data Verification:

    The test will play the designated audio stream and start audio capture on auxiliary capture interface.

  - The test will validate captured audio data with reference audio stream with a given threshold.
  - If the files are a match within given threshold, the step is marked as PASS.
  - If the files are not a match within given threshold, the step is marked as FAIL.

- Completion and result:

  Upon playing a designated audio stream and capturing audio data through auxiliary capture interface, the test will conclude and present a final result: PASS or FAIL based on file comparison between capture audio data and played stream.

### rmfAudio_test05_combinedDataCapture.py

#### User Input Required - test05

**No**

#### Acceptance Criteria - test05

Play **Stream #1** and **Stream #2** and confirm that the captured audio matches with played reference stream.

#### Expected Results - test05

The test plays a reference stream, captures audio data on both primary and auxiliary capture interfaces and verifies that the captured data matches with corresponding reference stream.

Success Criteria

- Audio data should be captured on primary and auxiliary capture interfaces simulataneously and written to a wav output file.
- Captured audio should match with corresponding played reference stream.

#### Test Steps - test05

- Run the Test:

  - Select and run the Python script: **`rmfAudio_test05_combinedDataCapture.py`**

- Download and Setup:

  The test will automatically download all necessary artifacts and streams, and copy them to the target directory on the device.

- Combined audio capture data Verification:

    The test will play the designated audio streams and start audio capture simulataneously on primary and auxiliary capture interfaces.

  - The test will validate captured audio data with corresponding reference audio stream with a given threshold.
  - If the files are a match within given threshold, the step is marked as PASS.
  - If the files are not a match within given threshold, the step is marked as FAIL.

- Completion and result:

  Upon playing a designated audio stream and capturing audio data simultaneously through primary and auxiliary capture interfaces, the test will conclude and present a final result: PASS or FAIL based on file comparison between capture audio data and played stream.

### rmfAudio_test06_auxiliaryJitterTest.py

#### User Input Required - test06

**No**

#### Acceptance Criteria - test06

Play **Stream #2** and confirm that there is no jitter detected on auxiliary capture interface.

#### Expected Results - test06

The test plays a reference stream, checks bytes of audio data received on auxiliary capture interface and verifies there is no jitter detected on auxiliary capture interface.

#### Test Steps - test06
- Run the Test:

  - Select and run the Python script: **`rmfAudio_test06_auxiliaryJitterTest.py`**

- Download and Setup:

  The test will automatically download all necessary artifacts and streams, and copy them to the target directory on the device.

- Auxiliary capture jitter Verification:

    The test will play the designated audio stream and start monitoring bytes received on auxiliary capture interface.

  - The test will monitor bytes received on auxiliary capture continuously against a set threshold.
  - If bytes captured are greater than given threshold, the step is marked as PASS.
  - If bytes captured are less than given threshold, the step is marked as FAIL.

- Completion and Result:

  Upon playing a designated audio stream and monitoring bytes received on auxiliary capture interface, the test will conclude and present a final result: PASS or FAIL based on whether bytes captured were within a given threshold.

### rmfAudio_test07_combinedJitterTest.py

#### User Input Required - test07

**No**

#### Acceptance Criteria - test07

Play **Stream #1** and **Stream #2** and confirm that there is no jitter detected on primary and auxiliary capture interfaces.

#### Expected Results - test07

The test plays a reference stream, checks bytes of audio data received simulataneously on primary and auxiliary capture interface and verifies there is no jitter detected on both audio capture interfaces.

#### Test Steps - test07
- Run the Test:

  - Select and run the Python script: **`rmfAudio_test07_combinedJitterTest.py`**

- Download and Setup:

  The test will automatically download all necessary artifacts and streams, and copy them to the target directory on the device.

- Primary and auxiliary combined capture jitter Verification:

    The test will play the designated audio streams and start monitoring bytes received on primary and auxiliary capture interfaces.

  - The test will monitor bytes received on primary and auxiliary audio captures continuously against a set threshold.
  - If bytes captured are greater than given threshold, the step is marked as PASS.
  - If bytes captured are less than given threshold, the step is marked as FAIL.

- Completion and Result:

  Upon playing a designated audio stream and monitoring bytes received on primary and auxiliary capture interfaces, the test will conclude and present a final result: PASS or FAIL based on whether bytes captured were within a given threshold.

### rmfAudio_L3_Runall.py

This python file runs all the tests

```bash
python rmfAudio_L3_Runall.py --config </PATH>/ut/host/tests/configs/example_rack_config.yml --deviceConfig </PATH>/ut/host/tests/configs/deviceConfig.yml
```