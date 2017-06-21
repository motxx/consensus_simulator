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

#ifndef MODEL_BLOCK_H
#define MODEL_BLOCK_H

#include "transaction.h"
#include <vector>

namespace model {

  struct Block {
    std::vector<Transaction> transactions;
    bool operator == (Block const& rhs) const {
      return transactions == rhs.transactions;
    }
  };

}  // namespace model

#endif  // MODEL_BLOCK_H
