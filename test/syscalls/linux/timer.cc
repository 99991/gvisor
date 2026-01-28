// Copyright 2026 The gVisor Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <errno.h>
#include <time.h>

#include "gtest/gtest.h"
#include "test/util/test_util.h"

namespace gvisor {
namespace testing {

namespace {

TEST(TimerTest, NegativeInterval) {
  timer_t timerid;
  struct sigevent sev = {};
  sev.sigev_notify = SIGEV_NONE;
  EXPECT_THAT(timer_create(CLOCK_PROCESS_CPUTIME_ID, nullptr, &timerid),
              SyscallSucceeds());
  struct itimerspec new_value = {};
  new_value.it_interval.tv_sec = 0;
  new_value.it_interval.tv_nsec = -2;  // Negative.
  new_value.it_value.tv_sec = 0;
  new_value.it_value.tv_nsec = 1000000;
  // Make sure this fails with EINVAL.
  EXPECT_THAT(timer_settime(timerid, 0, &new_value, nullptr),
              SyscallFailsWithErrno(EINVAL));
  timer_delete(timerid);
}

}  // namespace

}  // namespace testing
}  // namespace gvisor
