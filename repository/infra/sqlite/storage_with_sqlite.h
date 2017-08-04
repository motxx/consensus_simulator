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

#ifndef REPOSITORY_STORAGE_INFRA_SQLITE_H
#define REPOSITORY_STORAGE_INFRA_SQLITE_H

#include "repository/storage.h"
#include <vector>
#include <nonstd/optional.hpp>

namespace repository {
  class StorageWithSqlite : public repository::Storage {
  public:
    StorageWithSqlite(std::string const& source_dir);
    virtual ~StorageWithSqlite() {}

    // Read
    virtual std::vector<model::Peer> get_all_peers();
    virtual model::Peer get_peer(common::types::pubkey_t const& pubkey);

    // Write
    virtual void save_peer(model::Peer const& peer);

    // Delete
    virtual void delete_peer();
  };
}

#endif  // REPOSITORY_STORAGE_INFRA_SQLITE_H
