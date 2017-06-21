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

#include <memory>
#include <nonstd/optional.hpp>
#include <vector>
#include "server.h"
#include "consensus/sumeragi/state/leader.h"
#include "consensus/sumeragi/state/validator.h"
#include "consensus/sumeragi/state/proxy_tail.h"
#include "consensus/sumeragi/state/member.h"

namespace consensus {
  namespace sumeragi {
    namespace infra {
      void Server::on_vote(VoteMessage const& vote) {
        auto s = sumeragi_;//.lock();
        if (auto leader = s->leader()) {
          (*leader)->on_vote(vote);
        } else if (auto validator = s->validator()) {
          (*validator)->on_vote(vote);
        } else if (auto proxy_tail = s->proxy_tail()) {
          (*proxy_tail)->on_vote(vote);
        } else if (auto member = s->member()) {
          (*member)->on_vote(vote);
        } else {
          std::runtime_error("no match member");
        }
      }
      void Server::on_commit(CommitMessage const& commit) {
        auto s = sumeragi_;//.lock();
        if (auto leader = s->leader()) {
          (*leader)->on_commit(commit);
        } else if (auto validator = s->validator()) {
          (*validator)->on_commit(commit);
        } else if (auto proxy_tail = s->proxy_tail()) {
          (*proxy_tail)->on_commit(commit);
        } else if (auto member = s->member()) {
          (*member)->on_commit(commit);
        } else {
          std::runtime_error("no match member");
        }
      }
    }  // namespace infra
  }  // namespace sumeragi
}  // namespace consensus
