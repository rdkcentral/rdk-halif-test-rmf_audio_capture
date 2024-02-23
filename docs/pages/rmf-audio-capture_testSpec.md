# RMF Audio Capture HAL Documentation

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [RMF Audio Capture HAL Functionality](#rmf-audio-capture-hal-functionality)
- [Test scenarios that shall be evaluated](#test-procedure-for-hal-functionality)

## Acronyms, Terms and Abbreviations

- `HAL`    - Hardware Abstraction layer
- `PCM`    - Pulse Code Modulation
- `RDK`    - Reference Development Kit
- `RMF`    - RDK Media Framework

## RMF Audio Capture HAL Functionality

1. Audio Capture must support capture of primary audio, and may optionally support auxiliary audio (alternate    language audio tracks etc.) as well. Where auxiliary audio is supported. `HAL` must be able to support concurrent capture sessions
   for both primary and auxiliary audio.
2. Capture a stream of raw audio data in `PCM` with Different Audio formats/parameters.
3. Validate Captured audio data.

## Test Procedure for HAL Functionality
[Level2 Test Procedure](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/feature/issues3_Test_Suit_Specification/docs/pages/rmf-audio-capture_L2_Test_Procedure.md)  
[Level3 Test Procedure](https://github.com/rdkcentral/rdk-halif-test-rmf_audio_capture/blob/feature/issues3_Test_Suit_Specification/docs/pages/rmf-audio-capture_L3_Test_Procedure.md) 





