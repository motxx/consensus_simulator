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

#ifndef CONESENSUS_SUMERAGI_STATE_PROXYTAIL_DEATH_H
#define CONESENSUS_SUMERAGI_STATE_PROXYTAIL_DEATH_H

#include "proxy_tail.h"

namespace consensus {
  namespace sumeragi {
    struct ProxyTailDeath final : public ProxyTail {
      ProxyTailDeath(std::shared_ptr<infra::Client> client)
        : ProxyTail(client) {}
      virtual ~ProxyTailDeath() {}
      HasError behaviour() const override { return HasError::Error; }
      void on_vote(VoteMessage const& vote) override;
      void on_commit(CommitMessage const& commit) override;
    };
  }  // namespace sumeragi
}    // namespace consensus

#endif // CONESENSUS_SUMERAGI_STATE_PROXYTAIL_DEATH_H
