# RMF Audio Capture High Level Test Specification Documentation

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Introduction](#introduction)
- [Test Scenarios](#test-scenarios)
  - [Check primary audio capture](#check-primary-audio-capture)
  - [Check auxiliary audio capture](#check-auxiliary-audio-capture)
  - [Check concurrent audio capture](#check-concurrent-audio-capture)


### Acronyms, Terms and Abbreviations

- `HAL` - Hardware Abstraction Layer, may include some common components
- `UT` - Unit Test(s)
- `SoC` - System on a Chip
- `HAL` - Hardware Abstraction Layer
- `API` - Application Programming Interface
- `L2` - Level2 Testing
- `L3` - Level3 Testing
- `NA` - Not Applicable
- `Y` - Yes


## Introduction

This document provides an overview of the testing requirements for the RMF audio capture module. It outlines the scope of testing, objectives, testing levels and approaches, specific test requirements, and expected deliverables.

- `HAL` specification is available here: [rmf-audio-capture_halSpec.md](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/main/docs/pages/rmf-audio-capture_halSpec.md)
- `HAL` interface is available here: [rmfAudioCapture header](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/main/include/rmfAudioCapture.h)


## Test Scenarios

|#|Test Functionality|Description|
|-|------------------|-----------|
|1|[Check primary audio capture](#check-primary-audio-capture)|Run a capture of primary audio for a while and verify delivery of data|
|2|[Check auxiliary audio capture](#check-auxiliary-audio-capture)|Run a capture of auxiliary audio (on supported devices only) for a while and verify delivery of data|
|3|[Check concurrent audio capture](#check-concurrent-audio-capture)|Run parallel captures of primary and auxiliary audio (on supported devices only) and verify delivery of data|

### Check primary audio capture

|Description|HAL APIs|L2|L3|Control plane requirements|
|-----------|--------|--|--|--------------------------|
|Run primary audio capture for 10 seconds and verify receipt of commensurate amount of audio samples. Verify that there are no more data ready callbacks issued after the RMF_AudioCapture_Stop returns |RMF_AudioCapture_Open, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`Y`|`NA`|`NA`|
|Run primary audio capture for 10 seconds with known source material and verify that captured audio is faithful to the source within margin of error|RMF_AudioCapture_Open, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`NA`|`Y`|`Y`|
|Run primary audio capture for 2 minutes and verify that a commensurate amount of audio data is delivered. Also verify that jitter low enough to avoid underruns with an application buffer that's half the FIFO size.|RMF_AudioCapture_Open, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_GetCurrentSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`NA`|`Y`|`N`|

#### Test Startup Requirement - Check primary audio capture

 - Ensure audio is playing in the background before starting L3 tests.

#### Emulator Requirements - Check primary audio capture

- Emulator to implement RMF_AudioCapture HAL that is able to deliver a known 10-second audio clip (from wav or raw PCM file) when triggered by control plane.

#### Control Plane Requirements - Check primary audio capture

- Control plane must be able to trigger emulator HAL to deliver a known 10-second audio clip.



### Check auxiliary audio capture

Applicable only on devices that support auxiliary capture.


|Description|HAL APIs|L2|L3|Control plane requirements|
|-----------|--------|--|--|--------------------------|
|Run auxiliary audio capture for 10 seconds and verify receipt of commensurate amount of audio samples. Verify that there are no more data ready callbacks issued after the RMF_AudioCapture_Stop returns |RMF_AudioCapture_Open_Type, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`Y`|`NA`|`NA`|
|Run auxiliary audio capture for 10 seconds with known source material and verify that captured audio is faithful to the source within margin of error|RMF_AudioCapture_Open_Type, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`NA`|`Y`|`Y`|
|Run auxiliary audio capture for 2 minutes and verify that a commensurate amount of audio data is delivered. Also verify that jitter low enough to avoid underruns with an application buffer that's half the FIFO size.|RMF_AudioCapture_Open, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_GetCurrentSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`NA`|`Y`|`N`|

#### Test Startup Requirement - Check auxiliary audio capture

 - Ensure audio is playing in the background before starting L3 test. Test content must have auxiliary audio track.

#### Emulator Requirements - Check auxiliary audio capture

- Emulator to implement RMF_AudioCapture HAL that is able to deliver a known 10-second audio clip (from wav or raw PCM file) when triggered by control plane.

#### Control Plane Requirements - Check auxiliary audio capture

- Control plane must be able to trigger emulator HAL to deliver a known 10-second audio clip.


### Check concurrent audio capture

Applicable only on devices that support auxiliary capture.


|Description|HAL APIs|L2|L3|Control plane requirements|
|-----------|--------|--|--|--------------------------|
|Run auxiliary+primary audio capture for 10 seconds and verify receipt of commensurate amount of audio samples.  Verify that there are no more data ready callbacks issued after the RMF_AudioCapture_Stop returns |RMF_AudioCapture_Open_Type, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`Y`|`NA`|`NA`|
|Open primary and auxiliary capture interfaces, then issue a series of start and stop calls in a mixed sequence that verifies that primary and audio capture sessions are truly independent of each other and free of side-effects when the other is started or stopped. |RMF_AudioCapture_Open_Type, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`NA`|`Y`|`N`|
|Run auxiliary+primary audio capture for 10 seconds with known source material and verify that captured audio clips are faithful to the source within margin of error|RMF_AudioCapture_Open_Type, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`NA`|`Y`|`Y`|
|Run auxiliary+primary audio capture for 2 minutes and verify that a commensurate amount of audio data is delivered. Also verify that jitter low enough to avoid underruns with an application buffer that's half the FIFO size.|RMF_AudioCapture_Open_Type, RMF_AudioCapture_GetDefaultSettings, RMF_AudioCapture_GetCurrentSettings, RMF_AudioCapture_Start, RMF_AudioCapture_Stop, RMF_AudioCapture_Close|`NA`|`Y`|`N`|

#### Test Startup Requirement - Check concurrent audio capture

 - Ensure audio is playing in the background before starting L3 test. Test content must have primary as well as auxiliary audio tracks.

#### Emulator Requirements - Check concurrent audio capture

- Emulator to implement RMF_AudioCapture HAL that is able to deliver two distinct 10-second audio clips (from wav or raw PCM file) to each capture interface when triggered by control plane. It must be able to drive primary and auxiliary captures concurrently.

#### Control Plane Requirements - Check concurrent audio capture

- Control plane must be able to trigger emulator HAL to deliver a known 10-second audio clip.