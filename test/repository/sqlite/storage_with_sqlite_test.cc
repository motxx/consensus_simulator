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
#include "repository/infra/sqlite/storage_with_sqlite.h"

using namespace repository;

class StorageTest : public ::testing::Test {
protected:
  void SetUp() override {
    mkdir("/tmp/simulator", 0777);
    storage = std::make_unique<StorageWithSQLite>("/tmp/simulator/storage.db3");
    storage->initialize();
  }

  void TearDown() override {
    std::remove("/tmp/simulator/storage.db3");
    std::remove("/tmp/simulator");
  }

  std::unique_ptr<StorageWithSQLite> storage;
};

TEST_F(StorageTest, crud_peer) {
  model::Peer peer;
  peer.pubkey.fill(0);
  peer.pubkey[0] = 1, peer.pubkey[1] = 2;
  peer.ip = "123.123.0.0";
  peer.port = 12345;
  peer.trust_order = 5;

  // Create
  {
    ASSERT_TRUE(storage->append_peer(peer));
  }

  // Read
  {
    auto got = storage->get_peer(peer.pubkey);
    ASSERT_TRUE(got);

    auto actual = got.value();
    ASSERT_STREQ(actual.ip.c_str(), peer.ip.c_str());
    ASSERT_EQ(actual.port, peer.port);
    ASSERT_EQ(actual.pubkey, peer.pubkey);
    ASSERT_EQ(actual.trust_order, peer.trust_order);
  }

  // Update
  {
    peer.ip = "222.222.222.222";
    peer.port = 55555;
    peer.trust_order = 2;
    storage->update_peer(peer.pubkey, peer);

    auto got = storage->get_peer(peer.pubkey);
    ASSERT_TRUE(got);

    auto actual = got.value();
    ASSERT_STREQ(actual.ip.c_str(), peer.ip.c_str());
    ASSERT_EQ(actual.port, peer.port);
    ASSERT_EQ(actual.pubkey, peer.pubkey);
    ASSERT_EQ(actual.trust_order, peer.trust_order);
  }

  // Delete
  {
    ASSERT_TRUE(storage->delete_peer(peer.pubkey));
    ASSERT_FALSE(storage->get_peer(peer.pubkey));
  }
}

TEST_F(StorageTest, get_all_peers) {

  std::vector<model::Peer> peers;
  for (size_t i = 0; i < 5; ++i) {
    model::Peer peer;
    peer.pubkey.fill(0);
    peer.pubkey[0] = (uint8_t)i;
    peer.ip = "123.123.0." + std::to_string(i);
    peer.port = 12345;
    peer.trust_order = i;
    ASSERT_TRUE(storage->append_peer(peer));
    peers.push_back(peer);
  }

  auto actual = storage->get_all_peers();
  ASSERT_TRUE(actual);
  ASSERT_EQ(actual.value(), peers);
}