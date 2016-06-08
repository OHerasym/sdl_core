/*

 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */
#include <string.h>
#include "application_manager/commands/mobile/show_request.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "utils/file_system.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

ShowRequest::ShowRequest(const MessageSharedPtr& message,
                         ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

ShowRequest::~ShowRequest() {}

void ShowRequest::Run() {
  SDL_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_ERROR( "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }
  // SDLAQ-CRS-494, VC3.1
  if ((*message_)[strings::msg_params].empty()) {
    SDL_ERROR( strings::msg_params << " is empty.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (!CheckStringsOfShowRequest()) {
    SDL_ERROR( "Incorrect characters in string");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result = mobile_apis::Result::SUCCESS;
  if (((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) &&
      ((*message_)[strings::msg_params][strings::soft_buttons].length() > 0)) {
    processing_result = MessageHelper::ProcessSoftButtons(
        (*message_)[strings::msg_params],
        app,
        application_manager_.GetPolicyHandler(),
        application_manager_);
  }

  if (mobile_apis::Result::SUCCESS != processing_result) {
    SDL_ERROR( "Processing of soft buttons failed.");
    SendResponse(false, processing_result);
    return;
  }

  mobile_apis::Result::eType verification_result = mobile_apis::Result::SUCCESS;
  if (((*message_)[strings::msg_params].keyExists(strings::graphic)) &&
      ((*message_)[strings::msg_params][strings::graphic][strings::value]
           .asString()).length()) {
    verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::graphic],
        app,
        application_manager_);
    if (mobile_apis::Result::SUCCESS != verification_result) {
      SDL_ERROR( "Image verification failed.");
      SendResponse(false, verification_result);
      return;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::secondary_graphic)) {
    verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::secondary_graphic],
        app,
        application_manager_);
    if (mobile_apis::Result::SUCCESS != verification_result) {
      SDL_ERROR( "Image verification failed.");
      SendResponse(false, verification_result);
      return;
    }
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();

  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  int32_t index = 0;
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_1)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField1);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_1];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_2)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField2);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_2];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_3)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField3);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_3];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_4)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField4);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_4];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_clock)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mediaClock);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::media_clock];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_track)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mediaTrack);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::media_track];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::status_bar)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::statusBar);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::status_bar];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alignment)) {
    msg_params[strings::alignment] =
        (*message_)[strings::msg_params][strings::alignment];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::graphic)) {
    msg_params[strings::graphic] =
        (*message_)[strings::msg_params][strings::graphic];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::secondary_graphic)) {
    msg_params[strings::secondary_graphic] =
        (*message_)[strings::msg_params][strings::secondary_graphic];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[strings::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
    if ((*message_)[strings::msg_params][strings::soft_buttons].length() == 0) {
      app->UnsubscribeFromSoftButtons(function_id());
    } else {
      MessageHelper::SubscribeApplicationToSoftButton(
          (*message_)[strings::msg_params], app, function_id());
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::custom_presets)) {
    msg_params[strings::custom_presets] =
        (*message_)[strings::msg_params][strings::custom_presets];
  }

  SendHMIRequest(hmi_apis::FunctionID::UI_Show, &msg_params, true);

  MessageSharedPtr persistentData = new smart_objects::SmartObject(msg_params);
  app->set_show_command(*persistentData);
}

void ShowRequest::on_event(const event_engine::Event& event) {
  SDL_AUTO_TRACE();
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_Show: {
      SDL_DEBUG( "Received UI_Show event.");
      std::string response_info;
      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      const bool result = Compare<mobile_api::Result::eType, EQ, ONE>(
          result_code,
          mobile_api::Result::SUCCESS,
          mobile_api::Result::WARNINGS);

      if (mobile_apis::Result::WARNINGS == result_code &&
          message[strings::params].keyExists(hmi_response::message)) {
        response_info =
            message[strings::params][hmi_response::message].asString();
      }

      SendResponse(result,
                   result_code,
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_ERROR( "Received unknown event " << event.id());
      break;
    }
  }
}

bool ShowRequest::CheckStringsOfShowRequest() {
  SDL_AUTO_TRACE();
  const char* str;

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_4)) {
    str = (*message_)[strings::msg_params][strings::main_field_4].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid main_field_4 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_3)) {
    str = (*message_)[strings::msg_params][strings::main_field_3].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid main_field_3 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_2)) {
    str = (*message_)[strings::msg_params][strings::main_field_2].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid main_field_2 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_1)) {
    str = (*message_)[strings::msg_params][strings::main_field_1].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid main_field_1 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::status_bar)) {
    str = (*message_)[strings::msg_params][strings::status_bar].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid status_bar syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::media_clock)) {
    str = (*message_)[strings::msg_params][strings::media_clock].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid media_clock syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::media_track)) {
    str = (*message_)[strings::msg_params][strings::media_track].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid media_track syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::custom_presets)) {
    smart_objects::SmartObject& custom_presets_array =
        (*message_)[strings::msg_params][strings::custom_presets];
    for (size_t i = 0; i < custom_presets_array.length(); ++i) {
      str = custom_presets_array[i].asCharArray();
      if (!CheckSyntax(str)) {
        SDL_ERROR( "Invalid custom_presets syntax check failed");
        return false;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::graphic)) {
    str = (*message_)[strings::msg_params][strings::graphic][strings::value]
              .asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_ERROR( "Invalid graphic value syntax check failed");
      return false;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::secondary_graphic)) {
    str = (*message_)[strings::msg_params][strings::secondary_graphic]
                     [strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_ERROR(
                   "Invalid secondary_graphic value syntax check failed");
      return false;
    }
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager
