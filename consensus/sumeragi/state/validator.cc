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

#include "validator.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include "consensus/sumeragi/infra/server.h"
#include "main/network_simulator.h"
#include "validator/stateful_validator.h"

namespace consensus {
  namespace sumeragi {
    void Validator::send_to_proxy_tail(VoteMessage const& vote) {
      auto peers = NETWORK.get_all_peers();  // TODO: Fix get_peer_from_trust_order()
      if (not sumeragi_.has_value()) {
        std::runtime_error(
            "Validator::send_to_proxy_tail() failed to read sumeragi");
      }
      auto sumeragi = *sumeragi_;
      if (auto px = sumeragi->next_proxy()) {
        std::cout << "send_to_proxy_tail() : " << sumeragi->trust_order() << " -> "
                  << *px << "\n";
        client_->send_vote(peers[*px], vote);
      } else {
        std::runtime_error("invalid px");
      }
    }

    void Validator::on_vote(VoteMessage const& vote) {
      auto valid = validator::stateful_validate(
          vote.txs);  // TODO: Rename validator -> validation
      if (not valid) {
        return;
      }

      auto new_vote = add_signature(vote);
      send_to_proxy_tail(new_vote);
      error_recover(vote);  // TODO: これにタイマーを入れたほうが自然
    }

    void Validator::on_commit(CommitMessage const& commit) {
      committed.store(true);
      print_accept();
      // TODO: Permutate peers.
    }

    void Validator::error_recover(VoteMessage const& vote) {
      using namespace std::chrono;
      std::thread([this, vote] {
        std::this_thread::sleep_for(TimeOut);
        if (committed) {
          return;
        }  // TODO: これはRecoverが始まってからが自然

        if (sumeragi_.has_value()) {
          std::runtime_error(
              "Validator::error_recover() failed to read sumeragi");
        }

        auto sumeragi = *sumeragi_;
        if (sumeragi->has_next_proxy()) {
          std::cout << "Validator::error_recover::send_to_proxy_tail()\n";
          send_to_proxy_tail(vote);
          error_recover(vote);
        } else {
          std::cout
              << "\x1b[41mValidator::error_recover::on_reject()\x1b[49m\n";
          print_reject();
        }
      })
          .detach();
    }
  }  // namespace sumeragi
}  // namespace consensus
