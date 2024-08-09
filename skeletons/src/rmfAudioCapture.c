/**
*  If not stated otherwise in this file or this component's LICENSE
*  file the following copyright and licenses apply:
*
*  Copyright 2022 RDK Management
*
*  Licensed under the Apache License, Version 2.0 (the License);
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an AS IS BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/


#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

#include "rmfAudioCapture.h"

RMF_AudioCapture_Settings primary;
RMF_AudioCapture_Settings auxiliary;

rmf_Error RMF_AudioCapture_Open_Type(RMF_AudioCaptureHandle* handle, RMF_AudioCaptureType rmfAcType)
{
  rmf_Error result = RMF_SUCCESS;
  if(NULL != handle)
  {
    if(0 == strncmp(RMF_AC_TYPE_PRIMARY, rmfAcType, strlen(RMF_AC_TYPE_PRIMARY)))
    {
        *handle = (void *)&primary;
    }
    else if(0 == strncmp(RMF_AC_TYPE_AUXILIARY, rmfAcType, strlen(RMF_AC_TYPE_AUXILIARY)))
    {
        *handle = (void *)&auxiliary;
    }
    else
      result = RMF_INVALID_PARM;
  }
  else
    result = RMF_INVALID_PARM;
  return result;
}

rmf_Error RMF_AudioCapture_Open(RMF_AudioCaptureHandle* handle)
{
  rmf_Error result = RMF_SUCCESS;
  if(NULL != handle)
  {
    *handle = (void *)&primary;
  }
  else
    result = RMF_INVALID_PARM;
  return result;
}

rmf_Error RMF_AudioCapture_GetStatus(RMF_AudioCaptureHandle handle, RMF_AudioCapture_Status* status)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)status;
  return (rmf_Error)0;
}

rmf_Error RMF_AudioCapture_GetDefaultSettings(RMF_AudioCapture_Settings* settings)
{
  settings->format = racFormat_e16BitStereo;
  settings->samplingFreq = racFreq_e44100;
  return (rmf_Error)0;
}

rmf_Error RMF_AudioCapture_GetCurrentSettings(RMF_AudioCaptureHandle handle, RMF_AudioCapture_Settings* settings)
{
  /*TODO: Implement Me!*/
  (void)handle;
  (void)settings;
  return (rmf_Error)0;
}

rmf_Error RMF_AudioCapture_Start(RMF_AudioCaptureHandle handle, RMF_AudioCapture_Settings* settings)
{
  rmf_Error result = RMF_SUCCESS;
  if(&primary == (RMF_AudioCapture_Settings *)handle)
  {
    primary = *settings;
    if(primary.cbBufferReady)
    {
      char buffer[10] = {0};
      primary.cbBufferReady(primary.cbBufferReadyParm, (void *)buffer, sizeof(buffer)); //Send buffer ready just once as a dummy call.
    }
    else
      result = RMF_INVALID_PARM;
  }
  else if(&auxiliary == (RMF_AudioCapture_Settings *)handle)
  {
    auxiliary = *settings;
    if(auxiliary.cbBufferReady)
    {
      char buffer[5] = {0};
      auxiliary.cbBufferReady(auxiliary.cbBufferReadyParm, (void *)buffer, sizeof(buffer)); //Send buffer ready just once as a dummy call. Size is different from primary to differentiate.
    }
    else
      result = RMF_INVALID_PARM;
  }
  else
    result = RMF_INVALID_HANDLE;

  if(RMF_SUCCESS != result)
  {
  }
  return result;
}

rmf_Error RMF_AudioCapture_Stop(RMF_AudioCaptureHandle handle)
{
  /*TODO: Implement Me!*/
  (void)handle;
  return (rmf_Error)0;
}

rmf_Error RMF_AudioCapture_Close(RMF_AudioCaptureHandle handle)
{
  rmf_Error result = RMF_SUCCESS;
  if((&primary == (RMF_AudioCapture_Settings *)handle) || (&auxiliary == (RMF_AudioCapture_Settings *)handle))
  {
    RMF_AudioCapture_Settings * ctx = (RMF_AudioCapture_Settings *)handle;
    ctx->cbBufferReady = NULL;
    ctx->cbBufferReadyParm = NULL;
    ctx->cbStatusChange = NULL;
  }
  else
    result = RMF_INVALID_HANDLE;
  return result;
}

