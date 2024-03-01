# L2 Test Specification and Procedure Document

## Table of Contents

- [Overview](#overview)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Level 2 Test Specification](#level-2-test-specification)
- [Level 2 Test Procedure](#level-2-test-procedure)
  - [Start RMF Audio for Primary source](#start-rmf-audio-for-primary-source)
  - [Start RMF Audio for Auxiliary source](#start-rmf-audio-for-auxiliary-source)
  - [Start RMF Audio with simultaneous primary and auxiliary](#start-rmf-audio-with-simultaneous-primary-and-auxiliary)

## Overview

This document describes the Level 2 Test Specification and Procedure for the RMF AudioCapture module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer
- `UT`  \- Unit Test(s)

## Level 2 Test Specification

The following test case are defined based on profile configuration :

|#|Test Functionality|Description|Goal|
|-|------------------|-----------|----|
|1|[Start RMF Audio for Primary source](#start-rmf-audio-for-primary-source)|Start RMF Audio capture module for Primary source |No errors or data callbacks|
|2|[Start RMF Audio for Auxiliary source](#start-rmf-audio-for-auxiliary-source)|Start RMF Audio capture module for aux source|No errors or data callbacks|
|3|[Start RMF Audio with simultaneous primary and auxiliary](#start-rmf-audio-with-simultaneous-primary-and-auxiliary)|Start RMF Audio capture module for primary and auxiliary simultaneous |No errors or data callbacks|

## Level 2 Test Procedure

### Start RMF Audio for Primary source

|Title|Details|
|-----|-------|
|Function Name|`L2_rmfAudioCapture_primary_no_data`|
|Description| Start module for primary source and verify no errors and no callbacks are triggered |
|Test ID| 02 (Advanced) |
|Priority| High |

**Pre-Conditions :** None

**Dependencies :** None

**User Interaction :** None

| Variation / Steps | Description | Test Data | Expected Result |
| ------------------| --------- | ---------- | -------------- |
| 01 | Call `RMF_AudioCapture_Open()` to open interface | Valid handle | RMF_SUCCESS |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | RMF_SUCCESS |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start module | settings=default settings from previous step, No callback will trigger | RMF_SUCCESS |
| 04 | Ensure no callback occurs | return test failure if callbacks are triggered  |  |
| 05 | Call `RMF_AudioCapture_Stop()` to stop after 10 seconds | valid handle | RMF_SUCCESS |
| 06 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS |

### Start RMF Audio for Auxiliary source

|Title|Details|
|--|--|
|Function Name|`L2_rmfAudioCapture_aux_no_data`|
|Description| Start module for auxiliary source and verify no errors and no callbacks are triggered|
|Test ID| 02 (Advanced) |
|Priority| High |

**Pre-Conditions :**
Device must support auxiliary audio.

**Dependencies :** None

**User Interaction :** None

| Variation / Steps | Description | Test Data | Expected Result |
| ------------------| --------- | ---------- | -------------- |
| 01 | Call `RMF_AudioCapture_Open_Type()` to open a interface with auxiliary source | Valid handle | RMF_SUCCESS |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | RMF_SUCCESS |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start module | settings=default settings from previous step, No callback will trigger | RMF_SUCCESS |
| 04 | Ensure no callback occurs | return test failure if callbacks are triggered  |  |
| 05 | Call `RMF_AudioCapture_Stop()` to stop after 10 seconds | valid handle | RMF_SUCCESS |
| 06 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS |

### Start RMF Audio with simultaneous primary and auxiliary

|Title|Details|
|--|--|
|Function Name|`L2_rmfAudioCapture_combined_no_data`|
|Description| Start module of both primary and auxiliary audio source and verify  no errors and no callbacks are triggered|
|Test ID| 02 (Advanced) |
|Priority| High |

**Pre-Conditions :**
Device must support auxiliary audio.

**Dependencies :** None

**User Interaction :** None

| Variation / Steps | Description | Test Data | Expected Result |
| ------------------| --------- | ---------- | -------------- |
| 01 | Call `RMF_AudioCapture_Open_Type()` to open a interface with primary source | Valid handle | RMF_SUCCESS |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings for primary | valid settings | RMF_SUCCESS |
| 03 | Call `RMF_AudioCapture_Start()` with settings obtained above to start module for primary | settings=default settings from previous step, No callback will trigger for primary | RMF_SUCCESS |
| 04 | Ensure no callback occurs for primary| return test failure if callbacks are triggered for primary |  |
| 06 | Call `RMF_AudioCapture_Open_Type()` to open a interface with auxiliary source | Valid handle | RMF_SUCCESS |
| 07 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings for aux | valid settings | RMF_SUCCESS |
| 08 | Call `RMF_AudioCapture_Start()` with settings obtained above to start module  for aux| settings=default settings from previous step, No callback will trigger for aux| RMF_SUCCESS |
| 09 | Ensure no callback occurs for aux| return test failure if callbacks are triggered for aux|  |
| 10 | Call `RMF_AudioCapture_Stop()` to stop primary source after 10 seconds | valid handle | RMF_SUCCESS |
| 11 | Call `RMF_AudioCapture_Stop()` to stop aux source after 10 seconds | valid handle | RMF_SUCCESS |
| 12 | Call `RMF_AudioCapture_Close()` to release resources primary | current handle | RMF_SUCCESS |
| 13 | Call `RMF_AudioCapture_Close()` to release resources aux | current handle | RMF_SUCCESS |
