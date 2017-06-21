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

#include "proxy_tail.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>
#include "consensus/sumeragi/infra/server.h"
#include "main/network_simulator.h"
#include "validator/stateful_validator.h"

namespace consensus {
  namespace sumeragi {

    void ProxyTail::on_vote(VoteMessage const& vote) {
      if (not sumeragi_.has_value()) {
        std::runtime_error("ProxyTails::on_vote() failed to read sumeragi");
      }
      auto sumeragi = *sumeragi_;
      std::cout << "[ProxyTails] order: " << sumeragi->order() << "\n";

      auto valid = validator::stateful_validate(
          vote.txs);  // TODO: Rename validator -> validation
      if (not valid) {
        return;
      }

      if (signature_pool_.empty()) {
        auto my_vote = add_signature(vote);
        signature_pool_.push_back(my_vote.sig);
        // TODO: Poolingのトリガーは必ずLeaderのVoteであるべきか否か
        wait_for_signatures(my_vote);
      }
      signature_pool_.push_back(vote.sig);
      std::cout << "[ProxyTails] \x1b[31m#(sigs) = " << signature_pool_.size()
                << "\x1b[39m\n";
    }

    void ProxyTail::on_commit(CommitMessage const& commit) {
      std::cout << "[ProxyTails] [承認]\n";
      committed.store(true);
      // TODO: Permutate peers
    }

    // Private member funcs
    void ProxyTail::wait_for_signatures(VoteMessage const& vote) {
      std::thread([this, vote] {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(WaitSpan);
        if (not committed) {
          commit_or_retry(vote);
        }
      })
          .detach();
    }

    CommitMessage ProxyTail::make_commit(VoteMessage const& vote) {
      // TODO: Should be asynchronous. Use thread for each peers.
      CommitMessage commit;
      commit.block.transactions = vote.txs;
      // TODO: Attach validation signatures to commit message.
      return commit;
    }

    void ProxyTail::send_commit(CommitMessage const& commit) {
      auto peers = Network<infra::Server>::get_instance().get_all_peers();
      for (size_t i = 0; i < peers.size(); ++i) {
        // TODO: 自分のピアかの判定をし、そうであればスキップする
        client_->send_commit(peers[i], commit);
      }
      // TODO: on_commit() を直接呼び出す。send_commit()
      // が終了してからこの操作を行う。
    }

    bool count_valid_signatures(
        std::vector<model::Signature> const& signatures) {
      // TODO: Mock
      auto f =
          Network<infra::Server>::get_instance().get_all_peers().size() / 3;
      return signatures.size() >=
             2 * f + 1;  // Note: skips validating signatures.
    }

    void ProxyTail::commit_or_retry(VoteMessage const& vote) {
      if (sumeragi_.has_value()) {
        std::runtime_error(
            "ProxyTails::commit_or_retry failed to read sumeragi");
      }

      auto sumeragi = *sumeragi_;
      if (count_valid_signatures(signature_pool_)) {
        auto commit = make_commit(vote);
        send_commit(commit);
      } else {
        std::cout << "peer: " << sumeragi->order() << " commit_or_retry()\n";
        if (elasped_waiting_votes < TimeOut) {
          elasped_waiting_votes += WaitSpan;
          wait_for_signatures(vote);
        } else {
          if (sumeragi->has_next_proxy()) {
            sumeragi->next_proxy();  // ProxyTailのindexを消費
            Validator::send_to_proxy_tail(vote);
            Validator::error_recover(vote);
          } else {
            on_reject();
          }
        }
      }
    }

  }  // namespace sumeragi
}  // namespace consensus
