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

#ifndef CONSENSUS_SUMERAGI_INFRA_SERVER_H
#define CONSENSUS_SUMERAGI_INFRA_SERVER_H

#include "consensus/sumeragi/sumeragi.h"
#include "consensus/sumeragi/messages.h"
#include <memory>

namespace consensus {
  namespace sumeragi {
    class Sumeragi;
    namespace infra {
      class Server {
      public:
        Server(){}
        Server(consensus::sumeragi::Sumeragi* sumeragi)
          : sumeragi_(sumeragi) {}
        virtual ~Server() {}
        virtual void on_vote(VoteMessage const& vote);
        virtual void on_commit(CommitMessage const& commit);
      private:
        consensus::sumeragi::Sumeragi* sumeragi_;
      };
    }  // namespace infra
  }  // namespace sumeragi
}  // namespace consensus

#endif  // CONSENSUS_SUMERAGI_INFRA_SERVER_H
