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

#ifndef CONSENSUS_SUMERAGI_GATE_H
#define CONSENSUS_SUMERAGI_GATE_H

#include "model/proposal.h"
#include "../consensus_gate.h"

namespace consensus { namespace sumeragi {
  struct SumeragiGate : public ConsensusGate {
    virtual ~SumeragiGate(){};
    virtual void on_proposal(model::Proposal const &proposal) = 0;
  };
}}

#endif  // CONSENSUS_SUMERAGI_GATE_H
