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

#include <memory>
#include <nonstd/optional.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include "model/peer.h"

namespace consensus {
  namespace sumeragi {
    namespace infra {
      class Server;
    }
  }  // namespace sumeragi
}  // namespace consensus

#define NETWORK Network::get_instance()

class Network {
 public:
  virtual void register_server(
      std::string const& ip, size_t const port,
      std::shared_ptr<consensus::sumeragi::infra::Server> const& server);
  virtual nonstd::optional<consensus::sumeragi::infra::Server*> find(
      std::string const& ip, size_t const port);
  virtual std::vector<model::Peer> get_all_peers();
  virtual void set_peers(std::vector<model::Peer> const& peers);

  size_t peers_size() const { return peers_.size(); }

  void clear() {
    servers_.clear();
    peers_.clear();
  }

  static Network& get_instance() {
    static Network instance;
    return instance;
  }

 private:
  Network() {}
  std::unordered_map<std::string,
                     std::shared_ptr<consensus::sumeragi::infra::Server>>
      servers_;
  std::vector<model::Peer> peers_;
};

#endif  // MAIN_NETWORK_SIMULATOR_H
