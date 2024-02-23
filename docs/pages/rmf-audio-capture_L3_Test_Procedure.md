# L3 Test Procedure Document
## Table of Contents

  - [Overview](#overview)
  - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
  - [Level 3 Test Procedure](#level-3-test-procedure)
    - [Requirements to support L3 from UT-Core](#requirements-to-support-l3-from-ut-core)
    - [Raft L3 Testing Requirements](#raft-l3-testing-requirements)
    - [Internal CaptureCheckData](#internal-capturecheckdata)
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

### Requirements to support L3 from UT-Core
The following functions require being implemented into the UT-Core module to support control of the interface from a higher level.

|Test Name|Description|
|--|--|
|`RMF_AudioCapture_Open`|Open default configuration into a global variable and leave open|
|`RMF_AudioCapture_Open_Type`|Open type primary/auxiliary  configuration into a global variable |
|`RMF_AudioCapture_GetStatus`|get status information into a global variable |
|`RMF_AudioCapture_GetDefaultSettings`|get Default Settings into a global variable|
|`RMF_AudioCapture_GetCurrentSettings`|Open default configuration into a global variable |
|`RMF_AudioCapture_Start`|start default configuration into a global variable |
|`RMF_AudioCapture_Stop`|stop  |
|`RMF_AudioCapture_Close`|close default configuration  |

### Raft L3 Testing Requirements

- use these ut-core functions to achieve requirements

|Title|Details|
|--|--|
|Test Name|`CheckAudioDecodingCorrectFormat`|
|Description| Initiate the capture of primary/auxiliary audio data using specified input streams, then verify that the audio decoding produces the correct format. Continuously loop through different known input streams|
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
| 05 | Call `RMF_AudioCapture_Stop()` to stop capture | valid handle | RMF_SUCCESS | Should pass |
| 06 | Call `RMF_AudioCapture_Close()` to release resources | current handle | RMF_SUCCESS | Should pass |
| 07 | Call ` loop through `  | loop through different known input streams | NA | Should pass |

### Internal CaptureCheckData


|Title|Details|
|--|--|
|Test Name|`CaptureCheckData`|
|Description| Validate Captured audio data|
|Test Group| 03 |
|Priority| high |

**Pre-Conditions :**
Launch audio in the background before starting test.

**Dependencies :** 
Configuration YAML file contains all info on fixed streams and the format of each stream

**User Interaction :** None


| Variation  | Description | Test Data |
| -- | --------- | ---------- | 
| 01 | Call `CheckAudioDecodingCorrectFormat` | Capture Audio with Primary/Auxiliary or both type| 
| 02 | Capture `Audio data` to  workspace|  | returns 
| 03 | Check `Underflow ` Start / Stop the audio feed | Needs to be big enough to avoid overflow (expected service time * byte rate) | 
| 04 | Check `Overflow` | TBC: Run the audio data at a higher output rate. | 
| 05 | Check `Fifo Level` | Change the fifo level and check that the callbacks operate correctly |
| 06 | Check `delayCompensation_ms `  | Check the callbacks are delayed | 
| 07 | Plot data `Capture data for a specific time` | TBC: Generate test plot ready for consumption, fifo level, overflow, underflow, etc | 
| |  | Plot the callback rates, volume etc | |
| |  | Do the callback rates adjust based on delay compensation? | |
| |  | PyChart generate a plot of the data and output a png | |
| |  | Check against pass/fail thresholds which are set in the test configuration `yaml` | |
| |  | Compare data against the input source data | |

#### Get Status function dump

- RMF_AudioCapture_GetStatus() - Log information on fifo & threshold to plot in graph
- Fifo data output, to plot
- threshold data, to plot
- FIFO size
- format
- samplingFreq
- delayCompensation_ms

### Input Stream Requirements
6 * 6 streams requirement, to ensure testing of all the required formats = 36 streams
| parameters   | Description |
| ------- | --------- |
| racFormat_e16BitStereo| Stereo, 16 bits per sample interleaved into a 32-bit word | 
| racFormat_e24BitStereo| Stereo, 24 bits per sample.  The data is aligned to 32-bits left-justified.  Left and right channels will interleave one sample per 32-bit word | 
| racFormat_e16BitMonoLeft| Mono, 16 bits per sample interleaved into a 32-bit word. Left channel samples only | 
| racFormat_e16BitMonoRight| Mono, 16 bits per sample interleaved into a 32-bit word. Right channel samples only | 
| racFormat_e16BitMono| Mono, 16 bits per sample interleaved into a 32-bit word. Left and Right channels mixed | 
| racFormat_e24Bit5_1| 5.1 Multichannel, 24 bits per sample.  The data is aligned to 32-bits, left-justified. Channels will interleave one | 
|racFreq_e16000 |  16KHz |
|racFreq_e22050 |  22.05KHz |
|racFreq_e24000 |  24KHz  |
|racFreq_e32000 | 32KHz  |
|racFreq_e44100 |  44.1KHz  |
|racFreq_e48000 |   48KHz  |

