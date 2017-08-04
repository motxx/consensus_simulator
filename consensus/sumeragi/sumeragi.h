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

#ifndef CONSENSUS_SUMERAGI_H
#define CONSENSUS_SUMERAGI_H

#include <consensus/sumeragi/infra/server.h>
#include <memory>
#include <iostream>
#include <nonstd/optional.hpp>
#include "consensus/sumeragi/state/state_type.h"
#include "infra/server.h"
#include "main/network_simulator.h"
#include "sumeragi_gate.h"

namespace consensus {
  namespace sumeragi {
    namespace infra {
      class Client;
      class Server;
    }  // namespace infra

    class ConnectionTo;
    class Leader;
    class Validator;
    class ProxyTail;
    class Member;

    class PeerService;

    class Sumeragi : public SumeragiGate {
     public:
      Sumeragi(size_t const order, StateType state_type, std::shared_ptr<Member> state);

      // 各ステートは、個別の実装が責務となる。
      // state machineなので、virtual methodsを入れ替えられる。
      nonstd::optional<Leader*> leader();
      nonstd::optional<Validator*> validator();
      nonstd::optional<ProxyTail*> proxy_tail();
      nonstd::optional<Member*> member();
      void change_state(StateType state_type, std::shared_ptr<Member> state);

      size_t order() const { return order_; }
      StateType state_type() const { return state_type_; }

      size_t max_faulty() const { return NETWORK.peers_size() / 3; }
      size_t required_num_sigs() const { return NETWORK.peers_size() - max_faulty(); }

      // SumeragiGate
      void on_proposal(model::Proposal const& proposal) override;

      // network simulator / grpc server
      void run_server(std::string const& ip, size_t const port);

      // TODO: 別のピアとの接続は、ステートマシン自身のもつ仕事ではない。せめてmix-inにしておく。
      std::shared_ptr<infra::Client> client() { return client_; }

      bool has_next_proxy() {
        return next_proxy_tail_ + 1 < NETWORK.peers_size();
      }

      // TODO: Remove return type optional
      nonstd::optional<size_t> next_proxy() {
        if (next_proxy_tail_ == 0) {
          const int init_index = static_cast<int>(required_num_sigs()) - 1;
          next_proxy_tail_ = init_index < order_ ? order_ : static_cast<size_t>(init_index);
          return nonstd::make_optional(next_proxy_tail_);
        }
        return ++next_proxy_tail_ < NETWORK.peers_size()
                   ? nonstd::make_optional(next_proxy_tail_)
                   : nonstd::nullopt;
      }

     private:
      StateType state_type_;
      size_t order_ = 0;
      size_t next_proxy_tail_ = 0;  // 0(= 2f), 2f+1, ...
      std::shared_ptr<Member> state_;
      std::shared_ptr<infra::Client> client_;
      std::shared_ptr<infra::Server> server_;
    };
  }  // namespace sumeragi
}  // namespace consensus

#endif  // CONSENSUS_SUMERAGI_H
