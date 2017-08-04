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

#include <gflags/gflags.h>
#include <iostream>
#include <thread>
#include <vector>
#include "consensus/sumeragi/infra/client.h"
#include "consensus/sumeragi/infra/server.h"
#include "consensus/sumeragi/state/state_type.h"
#include "consensus/sumeragi/sumeragi.h"
//#include "consensus/sumeragi/peer_service/peer_service.h"
#include "consensus/sumeragi/state/leader.h"
#include "consensus/sumeragi/state/member.h"
#include "consensus/sumeragi/state/proxy_tail.h"
#include "consensus/sumeragi/state/proxy_tail_death.h"
#include "consensus/sumeragi/state/validator.h"
#include "consensus/sumeragi/state/validator_death.h"
#include "model/proposal.h"
#include "model/transaction.h"
#include "network_simulator.h"

void simulate_sumeragi(size_t peer_count);

bool validate_peer_count(const char*, uint32_t peer_count) {
  if (peer_count <= 0) {
    std::cerr << "peer_count should be greater than 0.\n";
    return false;
  }
  return true;
}

DEFINE_uint32(peer_count, 4, "Number of peers to simulate");
DEFINE_validator(peer_count, &validate_peer_count);
DEFINE_string(type, "", "Simulate type");
// DEFINE_validator(type, ...)

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  simulate_sumeragi(FLAGS_peer_count);
}

struct Context {
  std::shared_ptr<consensus::sumeragi::infra::Client> conn;
  std::shared_ptr<consensus::sumeragi::Sumeragi> sumeragi;
};

model::Proposal create_proposal() {
  model::Proposal proposal;
  model::Transaction tx1;
  tx1.command = "AddPeer";
  proposal.transactions.push_back(tx1);
  model::Transaction tx2;
  tx2.command = "RemovePeer";
  proposal.transactions.push_back(tx2);
  return proposal;
}

void send_proposal(std::vector<Context> const& ctx) {
  auto proposal = create_proposal();
  ctx[0].sumeragi->on_proposal(proposal); // Leaderを0に固定
}

model::Peer create_peer(size_t i) {
  model::Peer ret;
  ret.address = "net" + std::to_string(i);
  ret.port = 12345;
  auto pk = std::to_string(i);
  for (size_t k = 0; i < pk.size(); ++i) {
    ret.pubkey[k] = (uint8_t)pk[k];
  }
  return ret;
}

std::vector<Context> contexts;
std::vector<model::Peer> peers;  // Alternative to peer service

void simulate_normal(size_t const PeerCount) {
  using namespace consensus::sumeragi;

  const auto f = PeerCount / 3;

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    // auto ps = std::make_shared<PeerService>(self);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi =
        std::make_shared<Sumeragi>(i, std::make_shared<Leader>(context.conn));
    } else if (i < 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<Validator>(context.conn));
    } else if (i >= 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<ProxyTail>(context.conn));
    }/* else {
      context.sumeragi =
          std::make_shared<Sumeragi>(i, std::make_shared<Member>(context.conn));
    }*/
    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    // TODO: Replace with peer service?
    peers.push_back(self);
  }
  // TODO: peer service
  Network<infra::Server>::get_instance().set_peers(peers);

  // Send proposal
  send_proposal(contexts);
}

void simulate_proxy_death(size_t const PeerCount) {
  using namespace consensus::sumeragi;

  const auto f = PeerCount / 3;

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    // auto ps = std::make_shared<PeerService>(self);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi =
        std::make_shared<Sumeragi>(i, std::make_shared<Leader>(context.conn));
    } else if (i < 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<Validator>(context.conn));
    } else if (i == 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<ProxyTailDeath>(context.conn));
    } else if (i > 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<ProxyTail>(context.conn));
    }

    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    // TODO: Replace with peer service?
    peers.push_back(self);
  }
  // TODO: peer service
  Network<infra::Server>::get_instance().set_peers(peers);

  // Send proposal
  send_proposal(contexts);
}

void simulate_validator_death(size_t const PeerCount) {
  using namespace consensus::sumeragi;

  const auto f = PeerCount / 3;

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    // auto ps = std::make_shared<PeerService>(self);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi =
        std::make_shared<Sumeragi>(i, std::make_shared<Leader>(context.conn));
    } else if (i == 1) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<ValidatorDeath>(context.conn));
    } else if (i < 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<Validator>(context.conn));
    } else if (i >= 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<ProxyTail>(context.conn));
    }

    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    // TODO: Replace with peer service?
    peers.push_back(self);
  }
  // TODO: peer service
  Network<infra::Server>::get_instance().set_peers(peers);

  // Send proposal
  send_proposal(contexts);
}

void simulate_validator_max_faulty(size_t const PeerCount) {
  using namespace consensus::sumeragi;

  const auto f = PeerCount / 3;
  size_t curr_faulty = 0;

  // TODO: 異常系のピアの表示をわかりやすくする

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    // auto ps = std::make_shared<PeerService>(self);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi =
        std::make_shared<Sumeragi>(i, std::make_shared<Leader>(context.conn));
    } else if (i < 2 * f) {
      if (++curr_faulty < f) { // 2f+1の署名が必要。ピアの数=30, f=30/3=10, 2f+1=21だと、9台までしかピアを落とせない。
        context.sumeragi = std::make_shared<Sumeragi>(
          i, std::make_shared<ValidatorDeath>(context.conn));
      } else {
        context.sumeragi = std::make_shared<Sumeragi>(
          i, std::make_shared<Validator>(context.conn));
      }
    } else if (i >= 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
        i, std::make_shared<ProxyTail>(context.conn));
    }

    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    // TODO: Replace with peer service?
    peers.push_back(self);
  }
  // TODO: peer service
  Network<infra::Server>::get_instance().set_peers(peers);

  // Send proposal
  send_proposal(contexts);
}

void simulate_sumeragi(size_t const PeerCount) {
  if (FLAGS_type == "") {
    simulate_normal(PeerCount);
  }
  if (FLAGS_type == "validator_death") {
    simulate_validator_death(PeerCount);
  }
  if (FLAGS_type == "proxy_death") {
    simulate_proxy_death(PeerCount);
  }
  if (FLAGS_type == "validator_max_faulty") {
    simulate_validator_max_faulty(PeerCount);
  }

  sleep(unsigned(PeerCount) / 3 + 10);
  std::cout << "Shutting down...\n";
}
