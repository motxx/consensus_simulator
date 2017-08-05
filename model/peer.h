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

namespace model {
  struct Peer {
    std::string ip;
    size_t port;
    common::types::pubkey_t pubkey;
    size_t trust_order;

    bool operator==(Peer const& rhs) const {
      return ip == rhs.ip && port == rhs.port && pubkey == rhs.pubkey &&
             trust_order == rhs.trust_order;
    }
  };
}  // namespace model

#endif  // MODEL_PEER_H
