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

#include "consensus/sumeragi/state/leader.h"
#include "consensus/sumeragi/infra/client.h"
#include "model/peer.h"
#include "consensus/sumeragi/messages.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace consensus::sumeragi;

class ClientMock : public infra::Client {
public:
  MOCK_METHOD2(send_vote, bool(model::Peer const&, VoteMessage const&));
  MOCK_METHOD2(send_commit, bool(model::Peer const&, CommitMessage const&));
};

class LeaderTest : public ::testing::Test {
protected:
  void SetUp() override {
    NETWORK.clear();
  }

  void TearDown() override {
  }
};

TEST_F(LeaderTest, on_vote) {
  std::vector<model::Peer> peers(1);
  peers[0].ip = "net1";
  peers[0].port = 12345;
  NETWORK.set_peers(peers);

  auto client = std::make_shared<ClientMock>();
  Leader leader(client);
  VoteMessage vote;
  leader.on_vote(vote);
}