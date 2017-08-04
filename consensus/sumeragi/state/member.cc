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

#include "member.h"
#include <stdexcept>
#include <iostream>
#include <thread>

namespace consensus {
  namespace sumeragi {
    // TODO: Memberは不要

    VoteMessage Member::add_signature(VoteMessage const& vote) {
      VoteMessage ret;
      ret.txs = vote.txs;
      // TODO: Get self state from peer service.
      ret.sig.pubkey[0] = 'p', ret.sig.pubkey[1] = 'k';
      ret.sig.signature[0] = 's', ret.sig.signature[1] = 'k';
      return ret;
    }

    void Member::on_vote(VoteMessage const& vote) {
      std::runtime_error("Member function 'on_vote()' doesn't exist.");
    }

    void Member::on_commit(CommitMessage const& commit) {
      std::runtime_error("Member function 'on_commit()' doesn't exist.");
    }

    void Member::print_accept() {
      if (auto sumeragi = *sumeragi_) {
        std::cout << "\x1b[32m" << state_string(state())
                  << " Peer order: " << (*sumeragi).order() << " [承認]\x1b[39m\n";
      } else {
        std::runtime_error("[Member::print_accept] Invalid read sumeragi");
      }
    }

    void Member::print_reject() {
      if (auto sumeragi = *sumeragi_) {
        std::cout << "\x1b[41m" << state_string(state())
                  << " Peer order: " << (*sumeragi).order() << " [否認]\x1b[49m\n";
      } else {
        std::runtime_error("[Member::print_reject] Invalid read sumeragi");
      }
    }

  }  // namespace sumeragi
}    // namespace consensus
