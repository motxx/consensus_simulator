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

#include "storage_with_sqlite.h"
#include <SQLiteCpp/SQLiteCpp.h>

namespace repository {
  StorageWithSqlite::StorageWithSqlite(std::string const &source_dir)
    : Storage(source_dir)
  {
  }

  // Read
  std::vector<model::Peer> get_all_peers() {
    return std::vector<model::Peer>();
  }

  model::Peer get_peer(common::types::pubkey_t const& pubkey) {
    return model::Peer();
  }

  // Write
  void save_peer(model::Peer const& peer) {

  }

  // Delete
  void delete_peer() {

  }
}