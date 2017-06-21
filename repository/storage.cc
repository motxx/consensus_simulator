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

#include "storage.h"
//#include <leveldb/db.h>
#include <assert.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <fstream>
#include <nonstd/optional.hpp>
#include "model/peer.h"

namespace repository {

  Storage::Storage(std::string const& source_dir) : source_dir_(source_dir) {
    /*
    leveldb::Options options;
    options.create_if_missing = true;
    auto db = db_.get();
    auto status = leveldb::DB::Open(options, source_dir, &db);
    if (!status.ok()) {
      std::runtime_error("cannot open database: " + source_dir);
    }
    */
  }

  // Read
  nonstd::optional<model::Peer> Storage::get_peer(
      common::types::pubkey_t const& pubkey) {
    /*
    std::ifstream ifs(source_dir_);
    if (!ifs) {
      return nonstd::nullopt;
    }

    boost::archive::binary_iarchive iar(ifs);
    model::Peer peer;
    peer.pubkey = pubkey;
    iar >> boost::serialization::make_nvp(pubkey.to_hexstring().c_str(),
                                          peer.address);
    iar >> boost::serialization::make_nvp(pubkey.to_hexstring().c_str(),
                                          peer.port);
    return nonstd::make_optional(peer);
     */


    /*
    std::string address;
    auto stat = db_->Get(leveldb::ReadOptions(),
                        pubkey.to_hexstring() + ":address", &address);
    if (!stat.ok()) {
      return nonstd::nullopt;
    }
    model::Peer ret;
    ret.pubkey = pubkey;
    ret.address = address;
    return nonstd::make_optional(ret);
     */
  }

  nonstd::optional<model::Peer> Storage::get_peer_from_index(size_t index) {
    /*
    std::ifstream ifs(source_dir_);
    if (!ifs) {
      return nonstd::nullopt;
    }

    boost::archive::binary_iarchive iar(ifs);
    model::Peer peer;
    //auto key = ("peer_index:" + std::to_string(index)).c_str();
    iar >> boost::serialization::make_nvp("pubkey", peer.pubkey);
    iar >> boost::serialization::make_nvp("address", peer.address);
    iar >> boost::serialization::make_nvp("port", peer.port);
    return nonstd::make_optional(peer);
     */
  }

  // Update
  bool Storage::update_peer(model::Peer const& peer) {
    /*
    std::ofstream ofs(source_dir_);
    if (!ofs) {
      return false;
    }

    boost::archive::binary_oarchive oar(ofs);
    //auto key = ("peer_index:" + std::to_string(peer.index)).c_str();
    oar << boost::serialization::make_nvp("pubkey", peer.pubkey);
    oar << boost::serialization::make_nvp("address", peer.address);
    oar << boost::serialization::make_nvp("port", peer.port);
    return true;
     */

    /*
    auto stat = db_->Put(leveldb::WriteOptions(),
                        peer.pubkey.to_hexstring() + ":address", peer.address);
    return stat.ok();
     */
  }

  // Delete
  bool Storage::delete_peer(model::Peer const& peer) {
    //assert(false && "not supported");
    /*
    auto stat = db_->Delete(leveldb::WriteOptions(),
    peer.pubkey.to_hexstring()); return stat.ok();
     */
  }

}  // namespace repository
