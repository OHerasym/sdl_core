/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "transport_manager/mme/mme_connection_factory.h"
#include "transport_manager/mme/iap_connection.h"
#include "transport_manager/mme/mme_device.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

MmeConnectionFactory::MmeConnectionFactory(TransportAdapterController* controller) : controller_(controller), initialised_(false) {
}

TransportAdapter::Error MmeConnectionFactory::Init() {
  initialised_ = true;
  return TransportAdapter::OK;
}

TransportAdapter::Error MmeConnectionFactory::CreateConnection(
  const DeviceUID& device_uid, const ApplicationHandle& app_handle) {

  DeviceSptr device = controller_->FindDevice(device_uid);
  if (!device) {
    LOG4CXX_ERROR(logger_, "device " << device_uid << " not found");
    return TransportAdapter::BAD_PARAM;
  }
  MmeDevicePtr mme_device = DeviceSptr::static_pointer_cast<MmeDevice>(device);
  std::string mount_point = mme_device->mount_point();
  IAPConnection* iap_connection = new IAPConnection(device_uid, app_handle, controller_, mount_point);
  ConnectionSptr connection(iap_connection);

  controller_->ConnectionCreated(connection, device_uid, app_handle);

  if (iap_connection->Init()) {
    LOG4CXX_INFO(logger_, "iAP connection initialised");
    return TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "Could not initialise iAP connection");
    return TransportAdapter::FAIL;
  }
}

void MmeConnectionFactory::Terminate() {
}

bool MmeConnectionFactory::IsInitialised() const {
  return initialised_;
}

}  // namespace transport_adapter
}  // namespace transport_manager
