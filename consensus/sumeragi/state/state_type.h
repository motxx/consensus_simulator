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

#ifndef CONESENSUS_SUMERAGI_STATE_TYPE_H
#define CONESENSUS_SUMERAGI_STATE_TYPE_H

namespace consensus {
  namespace sumeragi {
    enum class StateType {
      Leader,
      Validator,
      ProxyTail,
      Member,
    };

    inline const char* state_string(StateType state) {
      if (state == StateType::Leader) { return "Leader"; }
      if (state == StateType::Validator) { return "Validator"; }
      if (state == StateType::ProxyTail) { return "ProxyTail"; }
      return "Member";
    }
  }  // namespace sumeragi
}    // namespace consensus

#endif // CONESENSUS_SUMERAGI_STATE_TYPE_H
