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

#include "leader.h"
#include <iostream>
#include <thread>
#include "model/signature.h"
#include "validator/stateful_validator.h"

// Alternative to peer service
#include "consensus/sumeragi/infra/server.h"
#include "main/network_simulator.h"

namespace consensus {
  namespace sumeragi {

    void Leader::send_to_validators(VoteMessage const& vote) {
      std::cout << "Leader::send_to_validators()\n";
      auto peers = NETWORK.get_all_peers();
      auto f = peers.size() / 3;
      for (size_t i = 1; i < 2 * f; ++i) {
        client_->send_vote(peers[i], vote);
      }
    }

    void Leader::on_vote(VoteMessage const& vote) {
      auto valid = validator::stateful_validate(vote.txs);
      if (not valid) { return; }
      auto new_vote = add_signature(vote);
      send_to_validators(new_vote);
      Validator::send_to_proxy_tail(vote); // indexの状態管理のため、他のvalidatorと区別する
      Validator::error_recover(vote);
    }

    void Leader::on_commit(CommitMessage const& commit) {
      committed.store(true);
      std::cout << "Leader::on_commit() [承認]\n";
      // TODO: Permutate peers
    }
  }  // namespace sumeragi
}    // namespace consensus
