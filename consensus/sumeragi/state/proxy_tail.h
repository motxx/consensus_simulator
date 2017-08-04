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

#ifndef CONESENSUS_SUMERAGI_STATE_PROXYTAIl_H
#define CONESENSUS_SUMERAGI_STATE_PROXYTAIl_H

#include <chrono>
#include "model/signature.h"
#include "validator.h"

namespace consensus {
  namespace sumeragi {
    struct ProxyTail : public Validator {
      ProxyTail(std::shared_ptr<infra::Client> client) : Validator(client) {}
      virtual ~ProxyTail() {}
      StateType state() const override { return StateType::ProxyTail; }
      virtual void on_vote(VoteMessage const& vote);
      virtual void on_commit(CommitMessage const& commit);

     private:
      CommitMessage make_commit(VoteMessage const& vote);
      void send_commit(CommitMessage const& commit);
      void commit_or_retry(VoteMessage const& vote);
      void wait_for_signatures(VoteMessage const& vote);
      bool count_valid_signatures(
          std::vector<model::Signature> const& signatures);

      std::vector<model::Signature> signature_pool_;
      std::chrono::milliseconds elasped_waiting_votes{0};
    };
  }  // namespace sumeragi
}  // namespace consensus

#endif  // CONESENSUS_SUMERAGI_STATE_PROXYTAIl_H
