﻿/*
 * Copyright (c) 2015-2016, Ford Motor Company
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

#include "utils/lock.h"
#include "utils/logger.h"

namespace sync_primitives {

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

Lock::Lock()
#ifndef NDEBUG
    : lock_taken_(0)
    , is_mutex_recursive_(false)
#endif  // NDEBUG
{
  Init(false);
}

Lock::Lock(bool is_recursive)
#ifndef NDEBUG
    : lock_taken_(0)
    , is_mutex_recursive_(is_recursive)
#endif  // NDEBUG
{
  Init(is_recursive);
}

Lock::~Lock() {
#ifndef NDEBUG
  if (0 < lock_taken_) {
    LOGGER_ERROR(logger_, "Destroying non-released mutex " << &mutex_);
  }
#endif
  delete mutex_;
  mutex_ = NULL;
}

void Lock::Acquire() {
  mutex_->lock();
  AssertFreeAndMarkTaken();
}

void Lock::Release() {
  AssertTakenAndMarkFree();
  mutex_->unlock();
}

bool Lock::Try() {
  if ((lock_taken_ > 0) && !is_mutex_recursive_) {
    return false;
  }
  if (!mutex_->tryLock()) {
    return false;
  }
#ifndef NDEBUG
  lock_taken_++;
#endif
  return true;
}

#ifndef NDEBUG
void Lock::AssertFreeAndMarkTaken() {
  if ((0 < lock_taken_) && !is_mutex_recursive_) {
    NOTREACHED();
  }
  lock_taken_++;
}
void Lock::AssertTakenAndMarkFree() {
  if (!lock_taken_) {
    NOTREACHED();
  }
  lock_taken_--;
}
#endif

void Lock::Init(bool is_recursive) {
  const QMutex::RecursionMode mutex_type =
      is_recursive ? QMutex::Recursive : QMutex::NonRecursive;

  mutex_ = new QMutex(mutex_type);
}

}  // namespace sync_primitives