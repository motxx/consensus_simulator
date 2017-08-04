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
#include "consensus/sumeragi/state/state_type.h"
#include "consensus/sumeragi/state/validator.h"
#include "consensus/sumeragi/state/validator_death.h"
#include "model/proposal.h"
#include "model/transaction.h"
#include "network_simulator.h"

void print_peer_status(consensus::sumeragi::Sumeragi* s);
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
  ctx[0].sumeragi->on_proposal(proposal);  // Leaderを0に固定
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

void simulate_normal(size_t const PeerCount) {
  std::cout << "\x1b[42m\x1b[37m simulate_normal \x1b[39m\x1b[49m\n";

  using namespace consensus::sumeragi;

  const auto f = PeerCount / 3;

  std::vector<model::Peer> peers;

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    // auto ps = std::make_shared<PeerService>(self);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi = std::make_shared<Sumeragi>(
          i, StateType::Leader, std::make_shared<Leader>(context.conn));
    } else if (i < 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
          i, StateType::Validator, std::make_shared<Validator>(context.conn));
    } else if (i >= 2 * f) {
      context.sumeragi = std::make_shared<Sumeragi>(
          i, StateType::ProxyTail, std::make_shared<ProxyTail>(context.conn));
    }

    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    peers.push_back(self);
  }

  // TODO: peer service
  NETWORK.set_peers(peers);

  std::cout << "\x1b[34m---------------------------------------\n";
  for (const auto& ctx : contexts) {
    print_peer_status(ctx.sumeragi.get());
  }
  std::cout << "---------------------------------------\x1b[39m\n";

  // Send proposal
  send_proposal(contexts);
}

// TODO: クソマクロ。(order, enum, 型)を整理した設計にせよ
#define MAKE_SUMERAGI(i, Type, Behaviour) \
  std::make_shared<Sumeragi>(i, StateType::Type, std::make_shared<Type ## Behaviour>(context.conn))


void simulate_proxy_death(size_t const PeerCount) {
  std::cout << "\x1b[42m\x1b[37m simulate_proxy_death \x1b[39m\x1b[49m\n";

  using namespace consensus::sumeragi;

  const int f = (int)PeerCount / 3;
  const int in_set_a = (int)PeerCount - f;

  std::vector<model::Peer> peers;

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi = MAKE_SUMERAGI(i, Leader,);
    } else if (i < in_set_a - 1) {
      context.sumeragi = MAKE_SUMERAGI(i, Validator,);
    } else if (i == in_set_a - 1) {
      context.sumeragi = MAKE_SUMERAGI(i, ProxyTail, Death);
    } else if (i > in_set_a - 1) {
      context.sumeragi = MAKE_SUMERAGI(i, ProxyTail,);
    }

    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    // TODO: Replace with peer service?
    peers.push_back(self);
  }

  // TODO: peer service
  NETWORK.set_peers(peers);

  std::cout << "\x1b[34m---------------------------------------\n";
  for (const auto& ctx : contexts) {
    print_peer_status(ctx.sumeragi.get());
  }
  std::cout << "---------------------------------------\x1b[39m\n";

  // Send proposal
  send_proposal(contexts);
}

void simulate_validator_death(size_t const PeerCount) {
  std::cout << "\x1b[42m\x1b[37m simulate_validator_death \x1b[39m\x1b[49m\n";

  using namespace consensus::sumeragi;

  const int faulty = (int)PeerCount / 3;
  std::cout << "FAULTY: " << faulty << "\n";
  std::cout << "2 * F + 1: " << 2 * faulty + 1 << "\n";

  std::vector<model::Peer> peers;

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi = MAKE_SUMERAGI(i, Leader,);
    } else if (i == 1) {
      context.sumeragi = MAKE_SUMERAGI(i, Validator, Death);
    } else if (i < 2 * faulty + 1) {
      context.sumeragi = MAKE_SUMERAGI(i, Validator,);
    } else if (i >= 2 * faulty + 1) {
      // SetAのProxyTail + SetB
      context.sumeragi = MAKE_SUMERAGI(i, ProxyTail,);
    }

    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    // TODO: Replace with peer service?
    peers.push_back(self);
  }

  // TODO: Peer service
  NETWORK.set_peers(peers);

  std::cout << "\x1b[34m---------------------------------------\n";
  for (const auto& ctx : contexts) {
    print_peer_status(ctx.sumeragi.get());
  }
  std::cout << "---------------------------------------\x1b[39m\n";

  // Send proposal
  send_proposal(contexts);
}

void simulate_validator_max_faulty(size_t const PeerCount) {
  std::cout
      << "\x1b[42m\x1b[37m simulate_validator_max_faulty \x1b[39m\x1b[49m\n";

  using namespace consensus::sumeragi;

  const size_t f = PeerCount / 3;
  const int in_set_a = (int)PeerCount - (int)f;
  size_t curr_faulty = 0;

  std::vector<model::Peer> peers;

  for (size_t i = 0; i < PeerCount; ++i) {
    auto self = create_peer(i);
    Context context;
    context.conn = std::make_shared<infra::Client>();
    if (i == 0) {
      context.sumeragi = MAKE_SUMERAGI(i, Leader,);
    } else if (i < in_set_a - 1) {
      if (curr_faulty++ < f) {
        context.sumeragi = MAKE_SUMERAGI(i, Validator, Death);
      } else {
        context.sumeragi = MAKE_SUMERAGI(i, Validator,);
      }
    } else if (i >= in_set_a - 1) {
      context.sumeragi = MAKE_SUMERAGI(i, ProxyTail,);
    }

    context.sumeragi->run_server(self.address, self.port);
    contexts.push_back(context);

    // TODO: Replace with peer service?
    peers.push_back(self);
  }

  // TODO: peer service
  NETWORK.set_peers(peers);

  std::cout << "\x1b[34m---------------------------------------\n";
  for (const auto& ctx : contexts) {
    print_peer_status(ctx.sumeragi.get());
  }
  std::cout << "---------------------------------------\x1b[39m\n";

  // Send proposal
  send_proposal(contexts);
}

void simulate_sumeragi(size_t const PeerCount) {
  if (FLAGS_type == "" || FLAGS_type == "normal") {
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

void print_peer_status(consensus::sumeragi::Sumeragi* s) {
  // ピアのStateType, 正常系/異常系
  const auto instance = [s]() -> consensus::sumeragi::Member* {
    if (s->leader()) {
      return *s->leader();
    } else if (s->validator()) {
      return *s->validator();
    } else if (s->proxy_tail()) {
      return *s->proxy_tail();
    } else {
      return *s->member();
    }
  }();

  std::cout << "ID: " << s->order() << " ";
  std::cout << "[" << consensus::sumeragi::state_string(s->state_type()) << "] "
            << (instance->behaviour() ==
                        consensus::sumeragi::Member::HasError::Normal
                    ? "normal"
                    : "error")
            << "\n";
}