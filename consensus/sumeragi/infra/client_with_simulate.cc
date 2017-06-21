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

#include <iostream>
#include <thread>
#include "client.h"
#include "server.h"
#include "main/network_simulator.h"
#include "model/peer.h"

namespace consensus {
  namespace sumeragi {
    namespace infra {
      bool Client::send_vote(model::Peer const& peer, VoteMessage const& vote) {
        auto to_peer = Network<Server>::get_instance().find(peer.address, peer.port);
        if (!to_peer.has_value()) { return false; }
        std::thread([to_peer, vote]{ (*to_peer)->on_vote(vote); }).detach();
        return true;
      }
      bool Client::send_commit(model::Peer const& peer, CommitMessage const& commit) {
        auto to_peer = Network<Server>::get_instance().find(peer.address, peer.port);
        if (!to_peer.has_value()) { return false; }
        std::thread([to_peer, commit]{ (*to_peer)->on_commit(commit); }).detach();
        return true;
      }
    }  // namespace infra
  }  // namespace sumeragi
}  // namespace consensus
