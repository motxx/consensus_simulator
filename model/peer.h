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

#ifndef MODEL_PEER_H
#define MODEL_PEER_H

#include "common/types.h"
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>

namespace model {
  struct Peer {
    std::string address;
    size_t port;
    common::types::pubkey_t pubkey;

    Peer() {}
    Peer(std::string const& address, common::types::pubkey_t pubkey)
      : address(address), pubkey(pubkey) {}

   private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive &ar, const unsigned int) {
      ar & boost::serialization::make_nvp("pubkey", pubkey);
      ar & boost::serialization::make_nvp("address", address);
      ar & boost::serialization::make_nvp("port", port);
    }
  };
}  // namespace model

#endif  // MODEL_PEER_H
