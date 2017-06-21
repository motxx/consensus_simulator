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

#ifndef CONESENSUS_SUMERAGI_STATE_LEADER_H
#define CONESENSUS_SUMERAGI_STATE_LEADER_H

#include "validator.h"

namespace consensus {
  namespace sumeragi {
    struct Leader : public Validator {
      Leader(std::shared_ptr<infra::Client> client)
        : Validator(client) {}
      virtual ~Leader() {}
      StateType state() const override { return StateType::Leader; }
      virtual void on_vote(VoteMessage const& vote);
      virtual void on_commit(CommitMessage const& commit);
    private:
      void send_to_validators(VoteMessage const& vote);
    };
  }  // namespace sumeragi
}    // namespace consensus

#endif // CONESENSUS_SUMERAGI_STATE_LEADER_H
