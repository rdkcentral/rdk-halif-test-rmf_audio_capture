# RMF Audio Capture HAL Documentation

## Table of Contents

- [Overview](#overview)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Definitions](#definitions)
- [References](#references)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [RMF Audio Capture HAL Functionality](#rmf-audio-capture-hal-functionality)
- [Test Functionality](#test-functionality)
- [Level2 Test Procedure for Capture audio data](#level2-test-procedure-for-capture-audio-data)
  - [Emulator Requirement for Capture stream of raw audio data](#emulator-requirement-for-capture-stream-of-raw-audio-data)
  - [Control Plane Requirement for Capture stream of raw audio data](#control-plane-requirement-for-capture-stream-of-raw-audio-data)
- [Level3 Test Procedure for Capture audio data with Different Audio input streams](#level3-test-procedure-for-capture-audio-data-with-different-audio-input-streams)
  - [Emulator Requirement for Capture audio data with Different Audio input streams](#emulator-requirement-for-capture-audio-data-with-different-audio-input-streams)
  - [Control Plane Requirement for Capture audio data with Different Audio input streams](#control-plane-requirement-for-capture-audio-data-with-different-audio-input-streams)
- [Level3 Test Procedure for Validate captured audio data](#level3-test-procedure-for-validate-captured-audio-data)
  - [Emulator Requirement for Validate captured audio data](#emulator-requirement-for-validate-captured-audio-data)
  - [Control Plane Requirement for Validate captured audio data](#control-plane-requirement-for-validate-captured-audio-data)

## Overview

Test Suit Specification document for RMF audio capture to cover L2, L3 specification and Test Procedure

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `HAL.h`  \- Abstracted defined API to control the hardware
- `HAL.c`  \- Implementation wrapper layer created by the `OEM` or `SoC` Vendor.
- `RDK`  \- Reference Design Kit for All Devices
- `RDK-V`  \- Reference Design Kit for Video Devices
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture (Sky is also an OEM)
- `SoC`  \- System on a Chip
- `FIFO` \- FIFO size in bytes.  This value is a total FIFO size to hold all channels of data. If not set, a default size will be used.  Needs to be big enough to avoid overflow (expected service time * byte rate)
- `delayCompensation_ms` \- Delay compensation in milli seconds. This parameter is used to maintain AV sync when using latency-prone audio outputs like Bluetooth. It's the number of ms to delay video by in order to stay in sync with Bluetooth audio

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
- RMF Audio Capture `HAL` Specification Document \- <https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/main/docs/pages/rmf-audio-capture_halSpec.md>
- RMF Audio Capture `HAL` Interface file \- <https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/main/include/rmfAudioCapture.h>

## RMF Audio Capture HAL Functionality

1. The audio capture functionality should be capable of capturing primary audio and, optionally, auxiliary audio (such as alternate language tracks). When auxiliary audio is supported, the `HAL` must be able to handle concurrent capture sessions for both primary and auxiliary audio.
2. Capture a stream of raw audio data in `PCM` with various audio formats and parameters.
3. Captured audio data must be validated.

## Test Functionality

|#|Test Functionality|Description|Level2|Level3|
|-|------------------|-----------|------|------|
|1 |Capture audio data |Capture a stream of raw audio data in `PCM` |Y|N|
|2 |Capture audio data with Different Audio formats/parameters |Capture a stream of raw audio data in `PCM` with various audio formats and parameters |N|Y|
|3 |Validate captured audio data |Captured audio data must be validated|N|Y|
  
## [Level2](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/feature/issues3_Test_Suit_Specification/docs/pages/rmf-audio-capture_L2_Test_Procedure.md) Test Procedure for Capture audio data

|#|Test Functionality|Description|
|-|------------------|-----------|
|1|Capture audio data | Capture a stream of raw audio data in `PCM` with type is primary |
| | |Capture a stream of raw audio data in `PCM` with type is auxiliary|
| | |Capture a stream of raw audio data in `PCM` with type is primary and auxiliary simultaneous if platform support|

### Emulator Requirement for Capture stream of raw audio data

- Boot with control configuration with support primary type[TBC]
- Boot with control configuration with support primary and auxiliary type[TBC]

### Control Plane Requirement for Capture stream of raw audio data

- check callbacks are happening[TBC]

## [Level3](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/feature/issues3_Test_Suit_Specification/docs/pages/rmf-audio-capture_L3_Test_Procedure.md) Test Procedure for Capture audio data with Different Audio input streams

|#|Test Functionality|Description|
|---|------------------|-----------|
|1  |  Capture a audio data with Different Audio formats/parameters | Capture a stream of raw audio data in `PCM` with predefined input Audio streams(defined in L3)|

### Emulator Requirement for Capture audio data with Different Audio input streams

- None

### Control Plane Requirement for Capture audio data with Different Audio input streams

- None

## [Level3](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/feature/issues3_Test_Suit_Specification/docs/pages/rmf-audio-capture_L3_Test_Procedure.md) Test Procedure for Validate captured audio data

|#|Test Functionality|Description|
|--|-----------------|-----------|
|1| Validate captured audio data | Capture a stream of raw audio data and check `Underflow` |
| | | Capture a stream of raw audio data and check `Overflow` |
| | | Capture a stream of raw audio data and check `Fifo` Level |
| | | Capture a stream of raw audio data and check `delayCompensation_ms` |

### Emulator Requirement for Validate captured audio data

- None

### Control Plane Requirement for Validate captured audio data

- None
