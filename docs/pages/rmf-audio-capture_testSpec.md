# RMF Audio Capture HAL Documentation

## Table of Contents


- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [RMF Audio Capture HAL Functionality](#rmf-audio-capture-hal-functionality)
- [Test scenarios that shall be evaluated](#test-scenarios-that-shall-be-evaluated)



## Acronyms, Terms and Abbreviations

- `API`    - Application Programming Interface
- `HAL`    - Hardware Abstraction layer
- `PCM`    - Pulse Code Modulation
- `RDK`    - Reference Development Kit
- `RMF`    - RDK Media Framework

## RMF Audio Capture HAL Functionality

1. Capture a stream of raw audio data in `PCM` with different Audio formats(Number of Channels, bitsPerSample and sampling Frequencies).
2. Audio Capture must support capture of primary audio, and may optionally support auxiliary audio (alternate language audio tracks etc.) as well. Where auxiliary audio is supported. `HAL` must be able to support concurrent capture sessions
   for both primary and auxiliary audio.

## Test scenarios that shall be evaluated
|SNo|Test Functionality|Description| [L2](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/feature/issues3_Test_Suit_Specification/docs/pages/rmf-audio-capture_L2_Test_Procedure.md)  | [L3](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/feature/issues3_Test_Suit_Specification/docs/pages/rmf-audio-capture_L3_Test_Procedure.md) | 
|---|------------------|-----------|-----|----|
|1  |  Capture a stream of raw audio data in `PCM` |  Capture a stream of raw audio data in `PCM` with type is primary/auxiliary   |   Y  |  Y  | 
|a |   |   Capture a stream of raw audio data in `PCM` with different Audio formats(Number of Channels, bitsPerSample and sampling Frequencies)  |  Y   |  Y  |
|b |   |    Validate Captured audio data |   Y  |  Y  |                    |        |





