# L2 Test Procedure Document

## Table of Contents

- [Overview](#overview)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Level 2 Test Procedure](#level-2-test-procedure)
  - [capture audio data for primary type](#capture-audio-data-for-primary-type)
  - [capture audio data for auxiliary type](#capture-audio-data-for-auxiliary-type)
  - [capture audio data with simultaneous primary and auxiliary](#capture-audio-data-with-simultaneous-primary-and-auxiliary)

## Overview

This document describes the level 2 testing Procedure for the RMF AudioCapture module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `HAL.h`  \- Abstracted defined API to control the hardware
- `HAL.c`  \- Implementation wrapper layer created by the `OEM` or `SoC` Vendor.
- `RDK`  \- Reference Design Kit for All Devices
- `RDK-V`  \- Reference Design Kit for Video Devices
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture (Sky is also an OEM)
- `SoC`  \- System on a Chip

## Level 2 Test Procedure

The following functions are expecting to test the module operates correctly :

### capture audio data for primary type

|Title|Details|
|-----|-------|
|Function Name|`test_l2_rmfAudioCapture_primary_type`|
|Description| Start capture of primary audio and verify that `HAL` triggers data callback |
|Test Group| 02 |
|Test Case ID| 1 |
|Priority| high |

**Pre-Conditions :** None

**Dependencies :** None

**User Interaction :** None

#### Test Procedure

| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | Call `RMF_AudioCapture_Open()` to open interface | handle must be a valid pointer | RMF_SUCCESS | Should pass |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | returns RMF_SUCCESS | Should pass |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, data callback will increment a static byte counter every time it runs, status callback NULL | RMF_SUCCESS | Should pass |
| 04 | Call `RMF_AudioCapture_Stop()` to stop capture after 10 seconds | valid handle | RMF_SUCCESS | Should pass |
| 05 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |

### capture audio data for auxiliary type

|Title|Details|
|--|--|
|Function Name|`test_l2_rmfAudioCapture_auxiliary_type`|
|Description| Start capture of auxiliary audio and verify that `HAL` triggers data callback with enough frequency and data in order to keep up with the expected data rate, tested over 10 seconds |
|Test Group| 02 |
|Test Case ID| 2 |
|Priority| high |

**Pre-Conditions :**
Device must support auxiliary audio.

**Dependencies :** None

**User Interaction :** None

#### Test Procedure:

| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer; type is auxiliary | RMF_SUCCESS | Should pass |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | returns RMF_SUCCESS | Should pass |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start audio capture | settings=default settings from previous step, data callback will increment a static byte counter every time it runs, status callback NULL | RMF_SUCCESS | Should pass |
| 04 | Call `RMF_AudioCapture_Stop()` to stop capture after 10 seconds | valid handle | RMF_SUCCESS | Should pass |
| 05 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |

### capture audio data with simultaneous primary and auxiliary

|Title|Details|
|--|--|
|Function Name|`test_l2_rmfAudioCapture_combined_`|
|Description| Start capture of both primary and auxiliary audio and verify that `HAL` triggers data callback with enough frequency and data in order to keep up with the expected data rate, tested over 10 seconds |
|Test Group| 02 |
|Test Case ID| 3 |
|Priority| high |

**Pre-Conditions :**
Device must support auxiliary audio.

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
