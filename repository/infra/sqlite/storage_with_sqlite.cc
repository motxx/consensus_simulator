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
#include <iostream>
#include "common/types.h"

namespace repository {
  StorageWithSQLite::StorageWithSQLite(std::string const& source_dir)
      : db_(source_dir, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) {
    std::cout << "SQLite database file '" << db_.getFilename().c_str()
              << "' opened successfully\n";
  }

  void StorageWithSQLite::initialize() {
    if (db_.exec("DROP TABLE IF EXISTS peer") != SQLite::OK) {
      std::runtime_error("Cannot DROP");
    }
    if (db_.exec("CREATE TABLE peer (pubkey TEXT PRIMARY KEY, ip TEXT, port "
                 "INTEGER, "
                 "trust_order INTEGER)") != SQLite::OK) {
      std::runtime_error("Cannot CREATE TABLE");
    }
  }

  boost::expected<std::vector<model::Peer>> StorageWithSQLite::get_all_peers() {
    SQLite::Statement query(db_, "SELECT * FROM peer");

    std::vector<model::Peer> ret;
    while (auto peer = execute_get_peer(query)) {
      ret.push_back(peer.value());
    }
    if (ret.empty()) {
      return boost::make_unexpected(read_errc::not_found);
    }
    return ret;
  }

  boost::expected<model::Peer> StorageWithSQLite::get_peer_at(size_t const trust_order) {
    SQLite::Statement query(db_, "SELECT * FROM peer WHERE trust_order = ?");
    query.bind(1, static_cast<int>(trust_order));
    return execute_get_peer(query);
  }

  boost::expected<model::Peer> StorageWithSQLite::get_peer(
      common::types::pubkey_t const& pubkey) {
    SQLite::Statement query(db_, "SELECT * FROM peer WHERE pubkey = ?");
    query.bind(1, pubkey.to_hexstring());
    return execute_get_peer(query);
  }

  // Create
  bool StorageWithSQLite::append_peer(model::Peer const& peer) {
    SQLite::Statement query(
        db_,
        "INSERT INTO peer (pubkey, ip, port, trust_order) VALUES (?, ?, ?, ?)");
    query.bind(1, peer.pubkey.to_hexstring());
    query.bind(2, peer.ip);
    query.bind(3, static_cast<int>(peer.port));
    query.bind(4, static_cast<int>(peer.trust_order));
    return query.exec() == 1;  // success is *NOT* SQLite::OK (= 0)
  }

  // Update
  bool StorageWithSQLite::update_peer(common::types::pubkey_t const& pubkey,
                                      model::Peer const& peer) {
    SQLite::Statement query(
        db_, "UPDATE peer SET ip=?, port=?, trust_order=? WHERE pubkey=?");
    query.bind(1, peer.ip);
    query.bind(2, static_cast<int>(peer.port));
    query.bind(3, static_cast<int>(peer.trust_order));
    query.bind(4, peer.pubkey.to_hexstring());
    return query.exec() == 1;  // success is *NOT* SQLite::OK (= 0)
  }

  // Delete
  bool StorageWithSQLite::delete_peer(common::types::pubkey_t const& pubkey) {
    SQLite::Statement query(db_, "DELETE FROM peer WHERE pubkey=?");
    query.bind(1, pubkey.to_hexstring());
    return query.exec() == 1;
  }

  // ----------------------- Private -----------------------
  boost::expected<model::Peer> StorageWithSQLite::execute_get_peer(SQLite::Statement &query) {
    if (not query.executeStep()) {
      return boost::make_unexpected(read_errc::not_found);
    }

    const std::string pk = query.getColumn("pubkey");
    const auto pk_blob = common::types::pubkey_t::hex_to_blob(pk);
    if (not pk_blob.has_value()) {
      return boost::make_unexpected(read_errc::value_is_broken);
    }
    const std::string ip = query.getColumn("ip");
    const int port = query.getColumn("port");
    const int trust_order = query.getColumn("trust_order");

    model::Peer peer;
    peer.pubkey = *pk_blob;
    peer.ip = ip;
    peer.port = static_cast<size_t>(port);
    peer.trust_order = static_cast<size_t>(trust_order);
    return peer;
  }

}  // namespace repository