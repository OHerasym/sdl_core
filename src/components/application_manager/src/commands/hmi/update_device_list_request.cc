/*
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/hmi/update_device_list_request.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

UpdateDeviceListRequest::UpdateDeviceListRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : RequestToHMI(message, application_manager)
    , EventObserver(application_manager_.event_dispatcher()) {}

UpdateDeviceListRequest::~UpdateDeviceListRequest() {}

void UpdateDeviceListRequest::Run() {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  // Fix problem with SDL and HMI HTML. This problem is not actual for HMI PASA.
  // Flag conditional compilation for specific customer is used in order to
  // exclude
  // hit code to RTC
  if (true == application_manager_.get_settings().launch_hmi()) {
    if (!application_manager_.IsHMICooperating()) {
      LOGGER_INFO(logger_, "Wait for HMI Cooperation");
      subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnReady);
      termination_condition_.Wait(auto_lock);
      LOGGER_DEBUG(logger_, "HMI Cooperation OK");
    }
  }

  SendRequest();
}

void UpdateDeviceListRequest::on_event(const event_engine::Event& event) {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  switch (event.id()) {
    case hmi_apis::FunctionID::BasicCommunication_OnReady : {
      LOGGER_INFO(logger_, "received OnReady");
      unsubscribe_from_event(hmi_apis::FunctionID::BasicCommunication_OnReady);
      termination_condition_.Broadcast();
      break;
    };
    default : {
      LOGGER_ERROR(logger_, "Unknown event ");
      break;
    };
  }
}

bool UpdateDeviceListRequest::CleanUp() {
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  termination_condition_.Broadcast();
  return true;
}

}  // namespace commands

}  // namespace application_manager

