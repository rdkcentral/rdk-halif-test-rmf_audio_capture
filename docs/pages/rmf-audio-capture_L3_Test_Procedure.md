# L3 Test Procedure Document

## Table of Contents

- [Overview](#overview)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Level 3 Test Procedure](#level-3-test-procedure)
  - [Python Requirement](#python-requirement)
  - [Requirements to support L3 from UT-Core](#requirements-to-support-l3-from-ut-core)
  - [Input Stream Requirements](#input-stream-requirements)

## Overview

This document describes the level 3 testing Procedure for the RMF_AudioCapture module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer, may include some common components
- `HAL.h`  \- Abstracted defined API to control the hardware
- `HAL.c`  \- Implementation wrapper layer created by the `OEM` or `SoC` Vendor.
- `RDK`  \- Reference Design Kit for All Devices
- `RDK-V`  \- Reference Design Kit for Video Devices
- `UT`  \- Unit Test(s)
- `OEM`  \- Original Equipment Manufacture (Sky is also an OEM)
- `SoC`  \- System on a Chip

## Level 3 Test Procedure

## Python Requirement

|Title|Details|
|-----|-------|
|Test Name|`CaptureCheckData`|
|Description| Validate Captured audio data|
|Test Group| 03 |
|Priority| high |

**Pre-Conditions :**
Launch audio in the background before starting test.

**Dependencies :**
Configuration YAML file contains all info on fixed streams and the format of each stream

**User Interaction :** None

|Steps|Description|Test Data|
|-----|-----------|---------|
| 01 | Call `CheckAudioDecodingCorrectFormat` | Capture Audio with Primary/Auxiliary|
| 02 | Call  `AudioCapture_Open_Type` to  capture data| Call UT function AudioCapture_Open_Type to capture the data|
| 03 | Check `Underflow` Start / Stop the audio feed | TBC:Needs to be big enough to avoid overflow (expected service time * byte rate) |
| 04 | Check `Overflow` | TBC: Run the audio data at a higher output rate. |
| 05 | Check `Fifo Level` | Change the fifo level and check that the callbacks operate correctly |
| 06 | Check `delayCompensation_ms`  | Check the callbacks are delayed |
| 07 | Plot data `Capture data for a specific time` | TBC: Generate test plot ready for consumption, fifo level, overflow, underflow, etc |
| |  | Plot the callback rates, volume etc |
| |  | Do the callback rates adjust based on delay compensation? |
| |  | PyChart generate a plot of the data and output a png |
| |  | Check against pass/fail thresholds which are set in the test configuration `yaml` |
| |  | Compare data against the input source data |

#### Get Status function dump

- RMF_AudioCapture_GetStatus() - Log information on fifo & threshold to plot in graph
- Fifo data output, to plot
- threshold data, to plot
- FIFO size
- format
- samplingFreq
- delayCompensation_ms

### Requirements to support L3 from UT-Core

The following functions require being implemented into the UT-Core module to support control of the interface from a higher level.

|Test Name|Description|
|---------|-----------|
|`AudioCapture_Open_Type`|Open type primary/auxiliary for capture the audio data|
|`AudioCapture_Close`|close default configuration|

|Title|Details|
|-----|-------|
|Test Name|`AudioCapture_Open_Type`|
|Description| Initiate the capture of primary/auxiliary audio data |
|Test Group| 03 |
|Priority| high |

**Pre-Conditions :**
Launch audio in the background before starting test.

**Dependencies :** 
Configuration YAML file contains all info on fixed streams and the format of each stream

**User Interaction :** None

| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01 | Call `RMF_AudioCapture_Open_Type()` to open interface | handle must be a valid pointer; type is Primary/Auxiliary | RMF_SUCCESS | Should pass |
| 02 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings | valid settings | RMF_SUCCESS | Should pass |
| 03 | Call `RMF_AudioCapture_Start()` to start audio capture | settings=default settings from previous step, data callback will increment a static byte counter every time it runs | RMF_SUCCESS | Should pass |
| 04 | Call `RMF_AudioCapture_GetStatus()` to get status | verify the status params with input stream | RMF_SUCCESS | Should pass |

|Title|Details|
|-----|-------|
|Test Name|`AudioCapture_Close`|
|Description| close the capture of primary/auxiliary audio data |
|Test Group| 03 |
|Priority| high |

**Pre-Conditions :**
AudioCapture_Open_Type() before invoking this function

**Dependencies :**
Configuration YAML file contains all info on fixed streams and the format of each stream

**User Interaction :** None

| Variation / Steps | Description | Test Data | Expected Result | Notes|
| -- | --------- | ---------- | -------------- | ----- |
| 01| Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
| 02 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |

### Input Stream Requirements

6 * 6 streams requirement, to ensure testing of all the required formats = 36 streams
| Stream Name   | Description |
| ------- | --------- |
|e16BitStereo_16KHz|Stereo, 16 bits per sample, 16KHz sample rate|
|e16BitStereo_22_05KHz|Stereo, 16 bits per sample, 22.05KHz sample rate|
|e16BitStereo_24KHz|Stereo, 16 bits per sample, 24KHz sample rate|
|e16BitStereo_32KHz|Stereo, 16 bits per sample, 32KHz sample rate|
|e16BitStereo_44_1KHz|Stereo, 16 bits per sample, 44.1KHz sample rate|
|e16BitStereo_48KHz|Stereo, 16 bits per sample, 48KHz sample rate|
|e24BitStereo_16KHz|Stereo, 24 bits per sample, 16KHz sample rate|
|e24BitStereo_22_05KHz|Stereo, 24 bits per sample, 22.05KHz sample rate|
|e24BitStereo_24KHz|Stereo, 24 bits per sample, 24KHz sample rate|
|e24BitStereo_32KHz|Stereo, 24 bits per sample, 32KHz sample rate|
|e24BitStereo_44_1KHz|Stereo, 24 bits per sample, 44.1KHz sample rate|
|e24BitStereo_48KHz|Stereo, 24 bits per sample, 48KHz sample rate|
|e16BitMonoLeft_16KHz|Mono (Left Channel), 16 bits per sample, 16KHz sample rate|
|e16BitMonoLeft_22_05KHz|Mono (Left Channel), 16 bits per sample, 22.05KHz sample rate|
|e16BitMonoLeft_24KHz|Mono (Left Channel), 16 bits per sample, 24KHz sample rate|
|e16BitMonoLeft_32KHz|Mono (Left Channel), 16 bits per sample, 32KHz sample rate|
|e16BitMonoLeft_44_1KHz|Mono (Left Channel), 16 bits per sample, 44.1KHz sample rate|
|e16BitMonoLeft_48KHz|Mono (Left Channel), 16 bits per sample, 48KHz sample rate|
|e16BitMonoRight_16KHz|Mono (Right Channel), 16 bits per sample, 16KHz sample rate|
|e16BitMonoRight_22_05KHz|Mono (Right Channel), 16 bits per sample, 22.05KHz sample rate|
|e16BitMonoRight_24KHz|Mono (Right Channel), 16 bits per sample, 24KHz sample rate|
|e16BitMonoRight_32KHz|Mono (Right Channel), 16 bits per sample, 32KHz sample rate|
|e16BitMonoRight_44_1KHz|Mono (Right Channel), 16 bits per sample, 44.1KHz sample rate|
|e16BitMonoRight_48KHz|Mono (Right Channel), 16 bits per sample, 48KHz sample rate|
|e16BitMono_16KHz|Mono (Mixed Left & Right), 16 bits per sample, 16KHz sample rate|
|e16BitMono_22_05KHz|Mono (Mixed Left & Right), 16 bits per sample, 22.05KHz sample rate|
|e16BitMono_24KHz|Mono (Mixed Left & Right), 16 bits per sample, 24KHz sample rate|
|e16BitMono_32KHz|Mono (Mixed Left & Right), 16 bits per sample, 32KHz sample 
|e16BitMono_44_1KHz|Mono (Mixed Left & Right), 16 bits per sample, 44.1KHz sample rate|
|e16BitMono_48KHz|Mono (Mixed Left & Right), 16 bits per sample, 48KHz sample rate|
|e24Bit5_1_16KHz|  5.1 Multichannel, 24 bits per sample,16KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|e24Bit5_1_22_05KHz|  5.1 Multichannel, 24 bits per sample,22.05KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|e24Bit5_1_24KHz|  5.1 Multichannel, 24 bits per sample,24KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|e24Bit5_1_32KHz|  5.1 Multichannel, 24 bits per sample,32KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|e24Bit5_1_44_1KHz|  5.1 Multichannel, 24 bits per sample,44.1KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|e24Bit5_1_48KHz|  5.1 Multichannel, 24 bits per sample,48KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
