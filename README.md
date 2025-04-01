# Unit Testing Suite For RMF Audio Capture HAL

## Table of Contents

- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Description](#description)
- [Reference Documents](#reference-documents)
- [How to build the test suite](#how-to-build-the-test-suite)
- [Notes](#notes)
- [Manual way of running the L1 and L2 test cases](#manual-way-of-running-the-l1-and-l2-test-cases)
- [Setting Python environment for running the L1 L2 and L3 automation test cases](#setting-python-environment-for-running-the-l1-l2-and-l3-automation-test-cases)

## Acronyms, Terms and Abbreviations

- `L1` - Functional Tests
- `L2` - Module functional Testing
- `L3` - Module testing with External Stimulus is required to validate and control device
- `HAL`- Hardware Abstraction Layer
- `High-Level Test Specification` : These specification will provide a broad overview of the system's functionality from the callers' perspective. It focuses on major use cases, system behavior, and overall caller experience.
- `Low-Level Test Specification` : These specification will deeper into the technical details. They will define specific test cases with inputs, expected outputs, and pass/fail criteria for individual functionalities, modules, or APIs.

## Description

This repository contains the Unit Test Suites(`L1` , `L2` and `L3`) for RMF Audio Capture `HAL`.

## Reference Documents

|SNo|Document Name|Document Description|Document Link|
|---|-------------|--------------------|-------------|
|1|`HAL` Specification Document|This document provides specific information on the APIs for which tests are written in this module|[rmf-audio-capture_halSpec.md](https://github.com/rdkcentral/rdk-halif-rmf_audio_capture/blob/main/docs/pages/rmf-audio-capture_halSpec.md "rmf-audio-capture_halSpec.md")|
|2|High Level Test Specification Document|High Level Test Specification Documentation this module|[rmf-audio-capture_High-Level_TestSpec.md](docs/pages/rmf-audio-capture_High-Level_TestSpec.md)|
|3|`L2` Low Level Test Specification Document|`L2`Low Level Test Specification Documentation this module|[rmf-audio-capture_L2-Low-Level_TestSpecification.md](docs/pages/rmf-audio-capture_L2-Low-Level_TestSpecification.md)|
|4|`L1` and `L2` Test Procedure Document|`L1` and `L2` Test Procedure Document|[rmf-audio-capture_L1_L2_TestProcedure.md](docs/pages/rmf-audio-capture_L1_L2_TestProcedure.md)|
|5|`L3` Low Level Test Specification Document|`L3`Low Level Test Specification Documentation this module|[rmf-audio-capture_L3_Low-Level_TestSpecification.md](docs/pages/rmf-audio-capture_L3_Low-Level_TestSpecification.md)|
|6|`L3` Low Level Test Procedure Document|`L3`Low Level Test Procedure Documentation this module|[rmf-audio-capture_L3_TestProcedure.md](docs/pages/rmf-audio-capture_L3_TestProcedure.md)|

## Notes

- All APIs need to be implemented in this current version. If any API is not supported, please add stub implementation with return type RMF_SUCCESS for the same.
- Building against the actual library may introduce SOC dependencies. Hence, a template SKELETON library is created without SOC dependencies. On the real platform (target), it can be mounted, copied and bound with the actual library.
- When executing the binary, ensure to include a platform-specific profile file as an argument for the designated test cases. The following example illustrates this:

## How to build the test suite

In order to build the test suits, there is a requirement to supply the toolchain, either as a vendor independent toolchain, an SDK, or an RDK Docker Toolchain .

For more information about RDK Docker Toolchain, please check [RDK Docker Toolchain](https://github.com/rdkcentral/ut-core/wiki/FAQ:-RDK-Docker-Toolchain)

- Export the Toolchain Path:

```bash
export TOOLCHAIN_PATH=/opt/toolchains/rdk-glibc-x86_64/sysroots/armv7at2hf-neon-rdk-linux-gnueabi
export CC="arm-rdk-linux-gnueabi-gcc  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=hard --sysroot=$TOOLCHAIN_PATH"
```

- Clone the `HAL` Repository:

```bash
git clone git@github.com:rdkcentral/rdk-halif-rmf_audio_capture.git
```

- Set Project Version:

If you want to build the test repository with a specific tag or branch, set the UT_PROJECT_VERSION variable accordingly. If not set, the default main branch will be used.

```bash
export UT_PROJECT_VERSION=1.0.1  # Build using a specific tag
```

or

```bash
export UT_PROJECT_VERSION=develop  # Build using a specific branch
```

- Build Targets:

 There are two targets for the platform

  1. linux - (default)
  2. arm - TARGET=arm

```bash
build_ut.sh TARGET=arm
```

- `build_ut.sh` script will clone the hal-test repo, ut-core and generate a `hal_test_rmfAudioCapture` binary in `ut/bin/` upon successful completion.

### Manual way of running the `L1` and `L2` test cases

```bash
 ./hal_test_rmfAudioCapture -p rmfAudioCaptureAuxSupported.yaml
 ```

Alternatively, use the run.sh script with the profile file:

```bash
./run.sh -p /absolute/path/to/profile/file
 ```

- Profile files define the configuration for the platform available here [profile yaml file](./profiles/)

### Setting Python environment for running the `L1` `L2` and `L3` automation test cases

- For running the `L1` `L2` and `L3` test suite, a host PC or server with a Python environment is required.
- Install Python Environment and Activation Scripts as detailed in the [HPK Documentation](https://github.com/rdkcentral/rdk-hpk-documentation/blob/main/README.md)
- To run the `L1` `L2` test cases follow the [rmf-audio-capture_L1_L2_TestProcedure.md](docs/pages/rmf-audio-capture_L1_L2_TestProcedure.md)
- To run the `L3` test cases follow the [rmf-audio-capture_L3_TestProcedure.md](docs/pages/rmf-audio-capture_L3_TestProcedure.md)