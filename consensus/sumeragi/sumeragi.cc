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

#include "sumeragi.h"
#include <iostream>
#include <memory>
#include <nonstd/optional.hpp>
#include <stdexcept>
#include "infra/server.h"
#include "main/network_simulator.h"
#include "state/leader.h"
#include "state/member.h"
#include "state/proxy_tail.h"
#include "state/state_type.h"
#include "state/validator.h"

namespace consensus {
  namespace sumeragi {

    Sumeragi::Sumeragi(size_t const order, StateType state_type, std::shared_ptr<Member> state)
        : order_(order),
          state_(state),
          server_(std::make_shared<infra::Server>(this)) {
      change_state(state_type, state);
      state_->set_sumeragi(this);
    }

    nonstd::optional<Leader*> Sumeragi::leader() {
      if (state_type_ == StateType::Leader) {
        return nonstd::make_optional(static_cast<Leader*>(state_.get()));
      }
      return nonstd::nullopt;
    }

    nonstd::optional<Validator*> Sumeragi::validator() {
      if (state_type_ == StateType::Validator) {
        return nonstd::make_optional(static_cast<Validator*>(state_.get()));
      }
      return nonstd::nullopt;
    }

    nonstd::optional<ProxyTail*> Sumeragi::proxy_tail() {
      if (state_type_ == StateType::ProxyTail) {
        return nonstd::make_optional(static_cast<ProxyTail*>(state_.get()));
      }
      return nonstd::nullopt;
    }

    nonstd::optional<Member*> Sumeragi::member() {
      if (state_type_ == StateType::Member) {
        return nonstd::make_optional(static_cast<Member*>(state_.get()));
      }
      return nonstd::nullopt;
    }

    void Sumeragi::change_state(StateType state_type,
                                std::shared_ptr<Member> state) {
      state_type_ = state_type;
      state_ = state;
    }

    // SumeragiGate
    void Sumeragi::on_proposal(model::Proposal const& proposal) {
      if (state_type_ != StateType::Leader) {
        return;
      }

      std::cout << "Sumeragi::on_proposal()\n";

      VoteMessage vote;
      vote.txs = proposal.transactions;
      state_->on_vote(vote);
    }

    // network simulator / grpc server
    void Sumeragi::run_server(std::string const& ip, size_t const port) {
      NETWORK.register_server(ip, port, server_);
    }

  }  // namespace sumeragi
}  // namespace consensus
