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

#include <gtest/gtest.h>
#include <memory>
#include "repository/storage.h"
#include "model/peer.h"
#include <nonstd/optional.hpp>

namespace repository {
  class StorageTest : public ::testing::Test {
  protected:
    void SetUp() override {
      mkdir("/tmp/simulator", 0777);
      storage = std::make_unique<Storage>("/tmp/simulator/storage");
    }

    void TearDown() override {
      remove("/tmp/simulator/storage");
      remove("/tmp/simulator");
    }

    std::unique_ptr<Storage> storage;
  };

  TEST_F(StorageTest, crud_test) {
    model::Peer peer;
    const std::string pk {"pubkey"};
    for (size_t i = 0; i < peer.pubkey.size(); ++i) {
      peer.address[i] = (uint8_t)pk[i];
    }
    //std::copy(pk.begin(), pk.end(), peer.pubkey);
    peer.address = "123.123.0.0";
    peer.port = 12345;
    auto stat = storage->update_peer(peer);
    ASSERT_TRUE(stat);

    auto res = storage->get_peer(peer.pubkey);
    ASSERT_TRUE(res);
    ASSERT_STREQ((*res).address.c_str(), peer.address.c_str());
    ASSERT_EQ((*res).port, peer.port);
  }
}  // namespace repository
