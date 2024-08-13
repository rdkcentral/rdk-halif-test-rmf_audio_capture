# Unit Testing Suite For RMF Audio Capture HAL

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Description](#description)
- [Reference Documents](#reference-documents)
- [Notes](#notes)

## Acronyms, Terms and Abbreviations

- `L1` - Functional Tests
- `L2` - Module functional Testing
- `HAL`- Hardware Abstraction Layer
- `High-Level Test Specification` : These specification will provide a broad overview of the system's functionality from the callers' perspective. It focuses on major use cases, system behavior, and overall caller experience.
- `Low-Level Test Specification` : These specification will delve deeper into the technical details. They will define specific test cases with inputs, expected outputs, and pass/fail criteria for individual functionalities, modules, or APIs.

## Description

This repository contains the Unit Test Suites(L1 & L2) for RMF Audio Capture `HAL`.

## Reference Documents

|SNo|Document Name|Document Description|Document Link|
|---|-------------|--------------------|-------------|
|1|`HAL` Specification Document|This document provides specific information on the APIs for which tests are written in this module|[rmf-audio-capture_halSpec.md](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/main/docs/pages/rmf-audio-capture_halSpec.md "rmf-audio-capture_halSpec.md")|
|2|High Level Test Specification Document|High Level Test Specification Documentation this module|[rmf-audio-capture_High-Level_TestSpec.md](docs/pages/rmf-audio-capture_High-Level_TestSpec.md)|
|3|`L2` Low Level Test Specification Document|`L2`Low Level Test Specification Documentation this module|[rmf-audio-capture_L2-Low-Level_TestSpecification.md](docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)|

## Notes

- All APIs need to be implemented in this current version. If any API is not supported, please add stub implementation with return type RMF_SUCCESS for the same.
- Building against the actual library may introduce SOC dependencies. Hence, a template SKELETON library is created without SOC dependencies. On the real platform (target), it can be mounted, copied and bound with the actual library.
- When executing the binary, ensure to include a platform-specific profile file as an argument for the designated test cases. The following example illustrates this:

```bash
 ./hal_test -p rmfAudioCaptureAuxSupported.yaml
 ```

Alternatively, use the run.sh script with the profile file:

```bash
./run.sh -p /absolute/path/to/profile/file
 ```

- Profile files define the configuration for the platform available here [profile yaml file](./profiles/)
