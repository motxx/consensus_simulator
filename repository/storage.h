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

#ifndef REPOSITORY_STORAGE_H
#define REPOSITORY_STORAGE_H

#include <boost/expected/expected.hpp>
#include <memory>
#include <nonstd/optional.hpp>
#include <vector>
#include "common/types.h"
#include "model/peer.h"

namespace repository {

  enum class read_errc {
    value_is_broken,
    not_found,
  };

  enum class write_errc { failed_to_write };

  class Storage {
   public:
    virtual ~Storage() {}

    // Initialize
    virtual void initialize() { std::runtime_error("undefined"); }

    // Read
    virtual boost::expected<std::vector<model::Peer>> get_all_peers() {
      std::runtime_error("undefined");
    }
    virtual boost::expected<model::Peer> get_peer_at(size_t const trust_order) {
      std::runtime_error("undefined");
    }
    virtual boost::expected<model::Peer> get_peer(
        common::types::pubkey_t const& pubkey) {
      std::runtime_error("undefined");
    }

    // Create
    virtual bool append_peer(model::Peer const& peer) {
      std::runtime_error("undefined");
    }

    // Update
    virtual bool update_peer(common::types::pubkey_t const& pubkey,
                             model::Peer const& peer) {
      std::runtime_error("undefined");
    }

    // Delete
    virtual bool delete_peer(common::types::pubkey_t const& pubkey) {
      std::runtime_error("undefined");
    }
  };
}  // namespace repository

#endif  // REPOSITORY_STORAGE_H
