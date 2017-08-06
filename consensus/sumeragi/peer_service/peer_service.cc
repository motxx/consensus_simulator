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

#include "peer_service.h"
#include "repository/infra/sqlite/storage_with_sqlite.h"
#include <iostream>

namespace consensus {
  namespace sumeragi {

    /**
     * PeerService
     * Peerの順序に関する情報をSumeragiに対して隠蔽する
     */

    PeerService::PeerService(model::Peer const& self,
                             std::shared_ptr<repository::Storage> storage)
      : self_(self), storage_(storage) {
      storage_->get_all_peers().map([this](std::vector<model::Peer> const& peers){
        this->peers_ = peers;
      });
    }

    bool PeerService::is_leader(model::Peer const& peer) {
      // TODO: Ordering Service が作られた場合、Leader groupを考慮する
      if (peers_.empty()) {
        return false;
      }
      return peers_[0] == peer;
    }

    std::vector<model::Peer> PeerService::initial_validators() const {
      std::vector<model::Peer> ret;
      if (num_required_sigs() == 0) {
        return {};
      }
      for (size_t i = 1; i < num_required_sigs() - 1; ++i) {
        ret.push_back(peers_[i]);
      }
      return ret;
    }

    nonstd::optional<model::Peer> PeerService::proxy_tail(size_t offset) const {
      const auto index = num_required_sigs() + offset;
      if (peers_.size() <= index) {
        return nonstd::nullopt;
      }
      return peers_[index];
    }

    void PeerService::permutate_peers() {
      // TODO: 性能が低いピアが合意形成に参加できる場合、permutateでは対処しきれない
      // Non-validating peerを導入すべきか、reputationを頑張るべき？
      auto stat = std::next_permutation(peers_.begin(), peers_.end());
      if (not stat) {
        std::sort(peers_.begin(), peers_.end());
      }
      for (size_t i = 0; i < peers_.size(); ++i) {
        peers_[i].trust_order = i;
        // FIXME: これはダメ。Storageの更新はTransactionの発行に基づく必要がある。
        storage_->update_peer(peers_[i].pubkey, peers_[i]);
      }
    }

    size_t PeerService::max_faulty() const {
      return peers_.size() / 3;
    }

    size_t PeerService::num_required_sigs() const {
      return peers_.size() - max_faulty();
    }

  }  // namespace sumeragi
}  // namespace consensus
