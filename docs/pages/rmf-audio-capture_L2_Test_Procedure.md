# L2 Test Procedure Document

## Table of Contents

  - [Overview](#overview)
    - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
    - [Definitions](#definitions)
    - [References](#references)
  - [Level 2 Test Procedure](#level-2-test-procedure)
    - [Ensure satisfactory data transfer for primary audio capture](#ensure-satisfactory-data-transfer-for-primary-audio-capture)
    - [Ensure satisfactory data transfer for auxiliary audio capture](#ensure-satisfactory-data-transfer-for-auxiliary-audio-capture)
    - [Ensure satisfactory data transfer with simultaneous primary and auxiliary capture](#ensure-satisfactory-data-transfer-with-simultaneous-primary-and-auxiliary-capture)

## Overview

This document describes the level 2 testing suite for the RMF_AudioCapture module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `HAL.h`  \- Abstracted defined API to control the hardware
- `HAL.c`  \- Implementation wrapper layer created by the `OEM` or `SoC` Vendor.
- `RDK`  \- Reference Design Kit for All Devices
- `RDK-V`  \- Reference Design Kit for Video Devices
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture (Sky is also an OEM)
- `SoC`  \- System on a Chip

### Definitions

- `Broadcom` \- `SoC` manufacturer <https://www.broadcom.com/>
- `Amlogic` \- `SoC` manufacturer <https://en.wikipedia.org/wiki/Amlogic>
- `Realtek` \- `SoC` manufacturer <https://en.wikipedia.org/wiki/Realtek>
- `Soc Vendor` \- Definition to encompass multiple vendors
- `Unit Tests` \- C Function tests that run on the target hardware
- `Common Testing Framework` \- Off the shelf 3rd Party Testing Framework, or framework that does not require infrastructure to control it. That's not to say it cannot be controlled via infrastructure if required. Examples of which are.
  - `GTest` \- Google Test Suit <https://google.github.io/googletest>
  - `CUnit` \- C Testing Suit <http://cunit.sourceforge.net/>
  - `Unity` \- C Embedded Testing Suit <http://www.throwtheswitch.org/unity>
  - `ut-core` \- Common Testing Framework <https://github.com/comcast-sky/rdk-components-ut-core>, which wraps a open-source framework that can be expanded to the requirements for future comcast-sky framework.

### References

- `Feedback Loops` \- <https://www.softwaretestingnews.co.uk/4-methods-to-improve-your-feedback-loops-and-supercharge-your-testing-process/>
- `Doxygen` \- SourceCode documentation tool - <https://www.doxygen.nl/index.html>
- `Black Box Testing` \- <https://en.wikipedia.org/wiki/Black-box_testing>

## Level 2 Test Procedure

The following functions are expecting to test the module operates correctly :

### Ensure satisfactory data transfer for primary audio capture

|Title|Details|
|--|--|
|Function Name|`test_l2_rmfAudioCapture_primary_data_check`|
|Description| Start capture of primary audio and verify that `HAL` triggers data callback with enough frequency and data in order to keep up with the expected data rate, tested over 10 seconds |
|Test Group| 02 |
|Test Case ID| 1 |
|Priority| high |

**Pre-Conditions :**
Launch video in the background before starting test.

**Dependencies :** None

**User Interaction :** None

#### Test Procedure :


| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | returns RMF_SUCCESS | Should pass |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, data callback will increment a static byte counter every time it runs, status callback NULL | RMF_SUCCESS | Should pass |
| 04 | Call `RMF_AudioCapture_Stop()` to stop capture after 10 seconds | valid handle | RMF_SUCCESS | Should pass |
| 05 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
| 06 | Compare actual total bytes logged by data callback with expected total. Expected total = 10 * byte-rate computed from audio parameters in default settings | byte rate = num. channels * bytes per channel * sampling frequency | Actual bytes received must be within 10% margin of error of expected | Should pass | 



### Ensure satisfactory data transfer for auxiliary audio capture

|Title|Details|
|--|--|
|Function Name|`test_l2_rmfAudioCapture_auxiliary_data_check`|
|Description| Start capture of auxiliary audio and verify that `HAL` triggers data callback with enough frequency and data in order to keep up with the expected data rate, tested over 10 seconds |
|Test Group| 02 |
|Test Case ID| 2 |
|Priority| high |

**Pre-Conditions :**
Device must support auxiliary audio. Launch video in the background before starting test.

**Dependencies :** None

**User Interaction :** None

#### Test Procedure :


| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer; type is auxiliary | RMF_SUCCESS | Should pass |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | returns RMF_SUCCESS | Should pass |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, data callback will increment a static byte counter every time it runs, status callback NULL | RMF_SUCCESS | Should pass |
| 04 | Call `RMF_AudioCapture_Stop()` to stop capture after 10 seconds | valid handle | RMF_SUCCESS | Should pass |
| 05 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
| 06 | Compare actual total bytes logged by data callback with expected total. Expected total = 10 * byte-rate computed from audio parameters in default settings | byte rate = num. channels * bytes per channel * sampling frequency | Actual bytes received must be within 10% margin of error of expected | Should pass | 



### Ensure satisfactory data transfer with simultaneous primary and auxiliary capture

|Title|Details|
|--|--|
|Function Name|`test_l2_rmfAudioCapture_combined_data_check`|
|Description| Start capture of both primary and auxiliary audio and verify that `HAL` triggers data callback with enough frequency and data in order to keep up with the expected data rate, tested over 10 seconds |
|Test Group| 02 |
|Test Case ID| 3 |
|Priority| high |

**Pre-Conditions :**
Device must support auxiliary audio. Launch video in the background before starting test.

**Dependencies :** None

**User Interaction :** None

#### Test Procedure :


| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer; type is auxiliary | RMF_SUCCESS | Should pass |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | returns RMF_SUCCESS | Should pass |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, data callback will increment a static byte counter every time it runs, status callback NULL | RMF_SUCCESS | Should pass |
| 04 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer; type is primary | RMF_SUCCESS | Should pass |
| 05 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, data callback will increment a static byte counter every time it runs, status callback NULL | RMF_SUCCESS | Should pass |
| 06 | Call `RMF_AudioCapture_Stop()` to stop capture after 10 seconds | valid primary handle | RMF_SUCCESS | Should pass |
| 07 | Call `RMF_AudioCapture_Stop()` to stop capture after 10 seconds | valid auxiliary handle | RMF_SUCCESS | Should pass |
| 08 | Call `RMF_AudioCapture_Close()` to release resources | current primary handle | RMF_SUCCESS | Should pass |
| 09 | Call `RMF_AudioCapture_Close()` to release resources | current auxiliary handle | RMF_SUCCESS | Should pass |
| 10 | Compare actual total bytes logged by data callback with expected total. Expected total = 10 * byte-rate computed from audio parameters in default settings | byte rate = num. channels * bytes per channel * sampling frequency | Actual bytes received must be within 10% margin of error of expected | Should pass | 
