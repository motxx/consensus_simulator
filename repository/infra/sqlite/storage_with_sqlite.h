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

#include <SQLiteCpp/SQLiteCpp.h>
#include "repository/storage.h"

namespace repository {
  class StorageWithSQLite : public repository::Storage {
   public:
    StorageWithSQLite(std::string const& source_dir);
    virtual ~StorageWithSQLite() override {}

    virtual void initialize() override;

    // Read
    virtual boost::expected<std::vector<model::Peer>> get_all_peers() override;
    virtual boost::expected<model::Peer> get_peer_at(size_t const trust_order) override;
    virtual boost::expected<model::Peer> get_peer(
        common::types::pubkey_t const& pubkey) override;

    // Create
    virtual bool append_peer(model::Peer const& peer) override;

    // Update
    virtual bool update_peer(common::types::pubkey_t const& pubkey,
                             model::Peer const& peer) override;

    // Delete
    virtual bool delete_peer(common::types::pubkey_t const& pubkey) override;

   private:
    boost::expected<model::Peer> execute_get_peer(SQLite::Statement &query);

    SQLite::Database db_;
  };
}  // namespace repository

#endif  // REPOSITORY_STORAGE_INFRA_SQLITE_H
