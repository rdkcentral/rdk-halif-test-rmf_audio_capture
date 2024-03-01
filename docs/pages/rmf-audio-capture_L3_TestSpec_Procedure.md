# L3 Test Procedure Document

## Table of Contents

- [Overview](#overview)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Level 3 Test Procedure](#level-3-test-procedure)
  - [Test Procedure for Validate captured audio data](#test-procedure-for-validate-captured-audio-data)
  - [UT-Core: L3 Test Requirements](#ut-core-l3-test-requirements)
  - [Input Stream Info](#input-stream-info)

## Overview

This document describes the Level 3 test Specification and Procedure for the RMF AudioCapture module.

### Acronyms, Terms and Abbreviations

- `HAL` \- Hardware Abstraction Layer
- `UT`  \- Unit Test(s)
- `FIFO` \- FIFO size in bytes.  This value is a total FIFO size to hold all channels of data. If not set, a default size will be used.  Needs to be big enough to avoid overflow (expected service time * byte rate)
- `delayCompensation_ms` \- Delay compensation in milliseconds. This parameter is used to maintain AV sync when using latency-prone audio outputs like Bluetooth. It's the number of ms to delay video by in order to stay in sync with Bluetooth audio

## Level 3 Test Procedure

### Test Procedure for Validate captured audio data

During running of test below will captured:

- Output the status flags on regular interval.
- All captured data will be validated by python to ensure the data formate correct.
- TBC: Plot data Generate test plot ready for consumption, fifo level, overflow, underflow, etc

|#|Test Functionality|Description|
|--|-----------------|-----------|
|1| Captured audio data | Capture audio data with no errors and validated the capture stream |
|2| Check `Fifo` Level| Capture audio data and check `Fifo` size match the callback data size |
|3| Set new `Fifo` Level| Capture audio data and check `Fifo` size match the callback data size |
|4| Cause `Overflow` data| Capture audio data and check `Overflow` occurs as expected |
|5| Cause `Underflow` data| Capture audio data and check `Underflow` occurs as expected |
|6| Validate operation of `delayCompensation_ms` | Capture a audio data and check `delayCompensation_ms` operating correctly |

### UT-Core: L3 Test Requirements

The following functions require being implemented into the UT-Core module to support control of the interface from a higher level.

|Function Name|Description|
|---------|-----------|
|[L3_open_source](#l3_open_source)|Open source based on input from user|
|[L3_start_with_default](#l3_start_with_default)|Start with default |
|[L3_start_with_params](#l3_start_with_params)|Start with input configuration params from user|
|[L3_close_source](#l3_close_source)|close source based on input from user|

### L3_open_source

|Title|Details|
|-----|-------|
|Function Name |`L3_open_source`|
|Description| Open source based on input from user |
|Group ID| 04 (helper) |

|Steps|Description|
|-----|-----------|
| 01 | User input from predefined list of sources based on profile |
| 02 | Call `RMF_AudioCapture_Open_Type()` to open interface |

### L3_start_with_default

|Title|Details|
|-----|-------|
|Function Name |`L3_start_with_default`|
|Description| Start the audio capture with default settings |
|Group ID| 04 (helper) |

|Steps|Description|
|-----|-----------|
| 01 | Call `RMF_AudioCapture_GetDefaultSettings()` to get default settings |
| 02 | Call `RMF_AudioCapture_Start()` to start audio capture with default settings |
| 03 | Call `RMF_AudioCapture_GetStatus()` to get status |

### L3_start_with_params

|Title|Details|
|-----|-------|
|Function Name |`L3_start_with_params`|
|Description| Start the audio capture with input from user |
|Group ID| 04 (helper) |

|Steps|Description|
|-----|-----------|
| 01 | Based on the user input setting params start the audio capture |
| 02 | Call `RMF_AudioCapture_Start()` to start audio capture with user input params settings |
| 03 | Call `RMF_AudioCapture_GetStatus()` to get status |

### L3_close_source

|Title|Details|
|-----|-------|
|Function Name |`L3_close_source`|
|Description| close source based on input from user |
|Group ID| 04 (helper) |

|Steps|Description|
|-----|-----------|
| 01 | Based on the user input handle stop the audio capture |
| 02 | Call `RMF_AudioCapture_Stop()` to stop audio capture with user input handle |
| 03 | Call `RMF_AudioCapture_Close()` to close audio capture with user input handle |

### Input Stream Info

6 * 6 streams requirement, to ensure testing of all the required formats = 36 streams
| Stream Name   | Description |
| ------- | --------- |
|RMF_audio_stream_e16BitStereo_16KHz|Stereo, 16 bits per sample, 16KHz sample rate|
|RMF_audio_stream_e16BitStereo_22_05KHz|Stereo, 16 bits per sample, 22.05KHz sample rate|
|RMF_audio_stream_e16BitStereo_24KHz|Stereo, 16 bits per sample, 24KHz sample rate|
|RMF_audio_stream_e16BitStereo_32KHz|Stereo, 16 bits per sample, 32KHz sample rate|
|RMF_audio_stream_e16BitStereo_44_1KHz|Stereo, 16 bits per sample, 44.1KHz sample rate|
|RMF_audio_stream_e16BitStereo_48KHz|Stereo, 16 bits per sample, 48KHz sample rate|
|RMF_audio_stream_e24BitStereo_16KHz|Stereo, 24 bits per sample, 16KHz sample rate|
|RMF_audio_stream_e24BitStereo_22_05KHz|Stereo, 24 bits per sample, 22.05KHz sample rate|
|RMF_audio_stream_e24BitStereo_24KHz|Stereo, 24 bits per sample, 24KHz sample rate|
|RMF_audio_stream_e24BitStereo_32KHz|Stereo, 24 bits per sample, 32KHz sample rate|
|RMF_audio_stream_e24BitStereo_44_1KHz|Stereo, 24 bits per sample, 44.1KHz sample rate|
|RMF_audio_stream_e24BitStereo_48KHz|Stereo, 24 bits per sample, 48KHz sample rate|
|RMF_audio_stream_e16BitMonoLeft_16KHz|Mono (Left Channel), 16 bits per sample, 16KHz sample rate|
|RMF_audio_stream_e16BitMonoLeft_22_05KHz|Mono (Left Channel), 16 bits per sample, 22.05KHz sample rate|
|RMF_audio_stream_e16BitMonoLeft_24KHz|Mono (Left Channel), 16 bits per sample, 24KHz sample rate|
|RMF_audio_stream_e16BitMonoLeft_32KHz|Mono (Left Channel), 16 bits per sample, 32KHz sample rate|
|RMF_audio_stream_e16BitMonoLeft_44_1KHz|Mono (Left Channel), 16 bits per sample, 44.1KHz sample rate|
|RMF_audio_stream_e16BitMonoLeft_48KHz|Mono (Left Channel), 16 bits per sample, 48KHz sample rate|
|RMF_audio_stream_e16BitMonoRight_16KHz|Mono (Right Channel), 16 bits per sample, 16KHz sample rate|
|RMF_audio_stream_e16BitMonoRight_22_05KHz|Mono (Right Channel), 16 bits per sample, 22.05KHz sample rate|
|RMF_audio_stream_e16BitMonoRight_24KHz|Mono (Right Channel), 16 bits per sample, 24KHz sample rate|
|RMF_audio_stream_e16BitMonoRight_32KHz|Mono (Right Channel), 16 bits per sample, 32KHz sample rate|
|RMF_audio_stream_e16BitMonoRight_44_1KHz|Mono (Right Channel), 16 bits per sample, 44.1KHz sample rate|
|RMF_audio_stream_e16BitMonoRight_48KHz|Mono (Right Channel), 16 bits per sample, 48KHz sample rate|
|RMF_audio_stream_e16BitMono_16KHz|Mono (Mixed Left & Right), 16 bits per sample, 16KHz sample rate|
|RMF_audio_stream_e16BitMono_22_05KHz|Mono (Mixed Left & Right), 16 bits per sample, 22.05KHz sample rate|
|RMF_audio_stream_e16BitMono_24KHz|Mono (Mixed Left & Right), 16 bits per sample, 24KHz sample rate|
|RMF_audio_stream_e16BitMono_32KHz|Mono (Mixed Left & Right), 16 bits per sample, 32KHz sample 
|RMF_audio_stream_e16BitMono_44_1KHz|Mono (Mixed Left & Right), 16 bits per sample, 44.1KHz sample rate|
|RMF_audio_stream_e16BitMono_48KHz|Mono (Mixed Left & Right), 16 bits per sample, 48KHz sample rate|
|RMF_audio_stream_e24Bit5_1_16KHz|  5.1 Multichannel, 24 bits per sample,16KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|RMF_audio_stream_e24Bit5_1_22_05KHz|  5.1 Multichannel, 24 bits per sample,22.05KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|RMF_audio_stream_e24Bit5_1_24KHz|  5.1 Multichannel, 24 bits per sample,24KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|RMF_audio_stream_e24Bit5_1_32KHz|  5.1 Multichannel, 24 bits per sample,32KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|RMF_audio_stream_e24Bit5_1_44_1KHz|  5.1 Multichannel, 24 bits per sample,44.1KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
|RMF_audio_stream_e24Bit5_1_48KHz|  5.1 Multichannel, 24 bits per sample,48KHz sample rate.  The data is aligned to 32-bits, left-justified. Channels will interleave one|
