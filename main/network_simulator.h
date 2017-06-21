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

#ifndef MAIN_NETWORK_SIMULATOR_H
#define MAIN_NETWORK_SIMULATOR_H

#include <iostream>
#include <memory>
#include <nonstd/optional.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include "consensus/sumeragi/sumeragi.h"

// Alternative to peer service
#include "model/peer.h"

#define NETWORK Network<infra::Server>::get_instance()

template <class Server>
struct Network {
  static Network& get_instance() {
    static Network instance;
    return instance;
  }

  void register_server(std::string const& ip, size_t const port,
                       std::shared_ptr<Server> const& server) {
    const auto address = ip + ":" + std::to_string(port);
    servers[address] = server;
  }

  nonstd::optional<Server*> find(std::string const& ip, size_t const port) {
    auto const address = ip + ":" + std::to_string(port);
    return servers.find(address) == servers.end()
               ? nonstd::nullopt
               : nonstd::make_optional(servers[address].get());
  }

  // Alternative to peer service
  std::vector<model::Peer> get_all_peers() {
    return peers_;
  }

  constexpr size_t peers_size() {
    return peers_.size();
  }

  void set_peers(std::vector<model::Peer> const& peers) {
    peers_ = peers;
  }

 private:
  Network() {}
  std::unordered_map<std::string, std::shared_ptr<Server>> servers;

  // Alternative to peers
  std::vector<model::Peer> peers_;
};

#endif  // MAIN_NETWORK_SIMULATOR_H
