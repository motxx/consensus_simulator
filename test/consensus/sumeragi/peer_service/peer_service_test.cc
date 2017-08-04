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
#include <gtest/gtest.h>
#include <gmock/gmock.h>

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
  void SetUp() override {

  }

  void TearDown() override {

  }
};

class StorageMock {
public:
};

TEST_F(PeerServiceTest, self) {
  model::Peer self;
  self.address = "net1";
  self.port = 12345;
  PeerService peer_service(self, storage_mock);

}
