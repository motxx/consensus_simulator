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

#include <iostream>
#include "network_simulator.h"

#define NETWORK Network::get_instance()

void Network::register_server(std::string const& ip, size_t const port,
                     std::shared_ptr<consensus::sumeragi::infra::Server> const& server) {
  const auto address = ip + ":" + std::to_string(port);
  servers_[address] = server;
}

nonstd::optional<consensus::sumeragi::infra::Server*> Network::find(std::string const& ip, size_t const port) {
  auto const address = ip + ":" + std::to_string(port);
  return servers_.find(address) == servers_.end()
         ? nonstd::nullopt
         : nonstd::make_optional(servers_[address].get());
}

std::vector<model::Peer> Network::get_all_peers() {
  return peers_;
}

void Network::set_peers(std::vector<model::Peer> const& peers) {
  peers_ = peers;
}
