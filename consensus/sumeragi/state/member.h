/*
 * Copyright 2017 Motohiko Abe
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CONESENSUS_SUMERAGI_STATE_MEMBER_H
#define CONESENSUS_SUMERAGI_STATE_MEMBER_H

#include "consensus/sumeragi/sumeragi.h"
#include "consensus/sumeragi/infra/client.h"
#include "consensus/sumeragi/messages.h"
#include "consensus/sumeragi/state/state_type.h"
#include <memory>
#include <chrono>
#include <string>
#include <nonstd/optional.hpp>
#include <atomic>

namespace consensus {
  namespace sumeragi {
    struct Member {
      Member(std::shared_ptr<infra::Client> client)
        : client_(client) {}
      virtual ~Member() {}
      virtual StateType state() const { return StateType::Member; }
      virtual void on_vote(VoteMessage const& vote);
      virtual void on_commit(CommitMessage const& commit);

      void print_accept();
      void print_reject();

      std::string state_string();
      void set_sumeragi(Sumeragi* sumeragi) { sumeragi_ = sumeragi; }
      VoteMessage add_signature(VoteMessage const& vote);

    protected:
      const std::chrono::milliseconds WaitSpan {100};
      const std::chrono::milliseconds TimeOut  {1000};
      std::shared_ptr<infra::Client> client_;
      nonstd::optional<Sumeragi*> sumeragi_;
      std::atomic_bool committed {false};

    private:
      std::chrono::milliseconds wait_for_proxy {0};
    };
  }  // namespace sumeragi
}    // namespace consensus

#endif // CONESENSUS_SUMERAGI_STATE_MEMBER_H
