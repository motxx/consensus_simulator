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

#include "proxy_tail_death.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "consensus/sumeragi/infra/server.h"
#include "main/network_simulator.h"
#include "validator/stateful_validator.h"

namespace consensus {
  namespace sumeragi {

    void ProxyTailDeath::on_vote(VoteMessage const& vote) {
    }

    void ProxyTailDeath::on_commit(CommitMessage const& commit) {
    }

  }  // namespace sumeragi
}  // namespace consensus
