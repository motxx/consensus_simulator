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

#ifndef VALIDATOR_STATEFUL_VALIDATOR_H
#define VALIDATOR_STATEFUL_VALIDATOR_H

#include "model/transaction.h"
#include <vector>

namespace validator {
  inline bool stateful_validate(std::vector<model::Transaction> const& txs) {
    return true;
  }
}  // namespace validator

#endif  // VALIDATOR_STATEFUL_VALIDATOR_H
