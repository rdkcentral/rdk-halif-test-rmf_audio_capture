# L3 Test Procedure Document
## Table of Contents

  - [Overview](#overview)
    - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
    - [Definitions](#definitions)
    - [References](#references)
  - [Level 3 Test Procedure](#level-3-test-procedure)
    - [UT-Core functions to be added](#ut-core-functions-to-be-added)
    - [Raft L3 Testing Requirements](#raft-l3-testing-requirements)
    - [Input Stream Requirements](#input-stream-requirements)

## Overview

This document describes the level 3 testing suite for the RMF_AudioCapture module.

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

## Level 3 Test Procedure

## UT-Core functions to be added

- L3 - RMF_AudioCapture_Open - Depricated not being tested
- L3 - RMF_AudioCapture_Open_Type(Primary)
- L3 - RMF_AudioCapture_Open_Type(Auxiliary)
- L3 - RMF_AudioCapture_GetStatus
- L3 - RMF_AudioCapture_GetDefaultSettings
- L3 - RMF_AudioCapture_GetCurrentSettings
- L3 - RMF_AudioCapture_Start
- L3 - RMF_AudioCapture_Stop
- L3 - RMF_AudioCapture_Close

## Raft L3 Testing Requirements

- use these ut-core functions to achieve requirements

### Internal CheckAudioDecodingCorrectFormat( Type Stream Type )

- configuration YAML file contains all info on fixed streams and the format of each stream.
- Set up the primary audio decoder, with a fixed stream.
- Start the primary audio decoder
- RMF_AudioCapture_Open_Type( PRIMARY / Auxiliary )
- RMF_AudioCapture_GetStatus() - should not be running
- RMF_AudioCapture_Start() - should now start

- loop for x time
- L3 - RMF_AudioCapture_GetStatus

- Dump all these fields. and check for overflow/underflow.
  - uint32_t overflows;     //!< Overflow count
  - uint32_t underflows;    //!< Underflow count
  - int8_t muted;           //!< Indicates whether capture is muted
  - int8_t paused;          //!< Indicates whether capture is paused
  - float volume;           //!< Current capture volume

- Check results against known data input from the fixed stream
- end loop

- RMF_AudioCapture_Stop()
- RMF_AudioCapture_GetStatus() - should not be running
- RMF_AudioCapture_Close() - should close

### Internal CaptureCheckData(type)

- capture the data to roofs.
- run CCheckAudioDecodingCorrectFormat(Primary)
- Copy captured data to the workspace
- Using host tools extract validate PCM data is correctly formatted
- Decode each packet of the captured PCM data. e.g packages 100-200 have been captured
- Compare against the packages of the same source data, Stream is 0-100000 packets, compare 100-200 is the same.

### L3 - Check Audio Capture Primary

- CaptureCheckData(Primary, Stream Type)

### L3 - Check Audio Capture Auxiliary

- CaptureCheckData(Auxiliary, Stream Type)

### L3 - Check Audio Capture Both Primary / Auxiliary

- CaptureCheckData(Primary, Stream Type)
- CaptureCheckData(Auxiliary, Stream Type)

### L3 - Underflow test

- TBC: Start / Stop the audio feed
- Needs to be big enough to avoid overflow (expected service time * byte rate)

### L3 - Overflow test

- TBC: Run the audio data at a higher output rate.

### L3 - Fifo Level test

- Change the fifo level and check that the callbacks operate correctly

### L3 - Run test and plot data - Capture data for a specific time.

- TBC: Generate test plot ready for consumption, fifo level, overflow, underflow, etc.
- Plot the callback rates, volume etc.
- Do the callback rates adjust based on delay compensation?
- PyChart generate a plot of the data and output a png
- Check against pass/fail thresholds which are set in the test configuration `yaml`.

### L3 - delayCompensation_ms

- TBC: Add delay into the output streams
- Check the callbacks are delayed.

### L3 - Volume

- TBC: Perform volume 

### Get Status function dump

- RMF_AudioCapture_GetStatus() - Log information on fifo & threshold to plot in graph
- Fifo data output, to plot
- threshold data, to plot
- FIFO size
- format
- samplingFreq
- delayCompensation_ms

## Input Stream Requirements

- 6 * 6 streams requirement, to ensure testing of all the required formats = 36 streams.

racFormat_e16BitStereo      Stereo, 16 bits per sample interleaved into a 32-bit word  <br>
racFormat_e24BitStereo      Stereo, 24 bits per sample.  The data is aligned to 32-bits left-justified.  Left and right channels will interleave one sample per 32-bit word  <br>
racFormat_e16BitMonoLeft   Mono, 16 bits per sample interleaved into a 32-bit word. Left channel samples only  <br>
racFormat_e16BitMonoRight  Mono, 16 bits per sample interleaved into a 32-bit word. Right channel samples only   <br>
racFormat_e16BitMono       Mono, 16 bits per sample interleaved into a 32-bit word. Left and Right channels mixed  <br>
racFormat_e24Bit5_1        5.1 Multichannel, 24 bits per sample.  The data is aligned to 32-bits, left-justified. Channels will interleave one  <br>

racFreq_e16000    16KHz <br>
racFreq_e22050    22.05KHz  <br>
racFreq_e24000    24KHz  <br>
racFreq_e32000    32KHz  <br>
racFreq_e44100    44.1KHz  <br>
racFreq_e48000    48KHz  <br>

