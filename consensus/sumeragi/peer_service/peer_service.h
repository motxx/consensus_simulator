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

#ifndef CONSENSUS_SUMERAGI_PEER_SERVICE_H
#define CONSENSUS_SUMERAGI_PEER_SERVICE_H

#include <algorithm>
#include <nonstd/optional.hpp>
#include <vector>
#include "model/peer.h"
#include "repository/storage.h"

namespace consensus {
  namespace sumeragi {
    class PeerService {
     public:
      PeerService(model::Peer const& self,
                  std::shared_ptr<repository::Storage> storage)
          : self_(self), storage_(storage) {}

      model::Peer self() const { return self_; }

      model::Peer proxy_tail() const {
        auto size = storage_->get_size_of_peers();
        auto f = size / 3;
        return storage_->get_peer_from_index(size - f - 1).value();
      }

      nonstd::optional<model::Peer> next_peer() const {
        return storage_->get_peer_from_index(self_.index + 1);
      }

      std::vector<model::Peer> validators() {
        std::vector<model::Peer> peers;
        storage_->get_peer_from_index();
        return peers;
      }

      void permutate_validators() const {
        /*
        auto perm = storage_->get_perm();
        std::next_permutation(perm.begin(), perm.end());
        storage_->update_perm(perm);
        */
      }

     private:
      model::Peer self_;
      std::vector<Peer> peers_;
      std::shared_ptr<repository::Storage> storage_;
    };
  }  // namespace sumeragi
}  // namespace consensus

#endif  // CONSENSUS_SUMERAGI_PEER_SERVICE_H
