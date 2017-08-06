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

#include <memory>
#include <algorithm>
#include <nonstd/optional.hpp>
#include <vector>
#include "model/peer.h"
#include "repository/storage.h"

namespace consensus {
  namespace sumeragi {

    /**
     * PeerService
     * Peerの順序に関する情報をSumeragiに対して隠蔽する
     */
    class PeerService {
     public:
      PeerService(model::Peer const& self,
                  std::shared_ptr<repository::Storage> storage);

      model::Peer self() const { return self_; }
      virtual bool is_leader(model::Peer const& peer);
      virtual std::vector<model::Peer> initial_validators() const;
      virtual nonstd::optional<model::Peer> proxy_tail(size_t offset) const;
      virtual void permutate_peers();

      virtual size_t max_faulty() const;
      virtual size_t num_required_sigs() const;

     private:
      model::Peer self_;
      std::vector<model::Peer> peers_;
      std::shared_ptr<repository::Storage> storage_;
      // TODO: Storageの更新はTransactionによってのみ行われるため、Storageの読み込み以外の機能を制限する
    };
  }  // namespace sumeragi
}  // namespace consensus

#endif  // CONSENSUS_SUMERAGI_PEER_SERVICE_H
