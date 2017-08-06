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

#include "consensus/sumeragi/peer_service/peer_service.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "repository/storage.h"

/**
 * PeerService
 * - 自分のピアの情報を提供する
 * - 他のピアの情報を提供する
 *
 * 順序
 * 1. 自分のピアの情報を自分のPeerServiceに持つ
 * 2. ネットワーク上のピアの情報をダウンロードする
 */

using namespace consensus::sumeragi;

class PeerServiceTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {}
};

class StorageMock : public repository::Storage {
 public:
  MOCK_METHOD0(init_db, void());
  MOCK_METHOD0(get_all_peers, boost::expected<std::vector<model::Peer>>());
  MOCK_METHOD1(get_peer_at, boost::expected<model::Peer>(size_t const));
  MOCK_METHOD1(get_peer,
               boost::expected<model::Peer>(common::types::pubkey_t const&));
  MOCK_METHOD1(append_peer, bool(model::Peer const&));
  MOCK_METHOD2(update_peer, bool(common::types::pubkey_t const& pubkey,
                                 model::Peer const& peer));
  MOCK_METHOD1(delete_peer, bool(common::types::pubkey_t const& pubkey));
};

TEST_F(PeerServiceTest, self) {
  model::Peer self;
  self.ip = "net1";
  self.port = 12345;
  auto storage_mock = std::make_shared<StorageMock>();
  PeerService peer_service(self, storage_mock);
}
