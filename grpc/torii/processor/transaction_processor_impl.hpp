/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_TRANSACTION_PROCESSOR_STUB_HPP
#define IROHA_TRANSACTION_PROCESSOR_STUB_HPP

#include <network/peer_communication_service.hpp>
#include <model/transaction_response.hpp>
#include <torii/processor/transaction_processor.hpp>
#include <validation/stateless_validator.hpp>

namespace iroha {
  namespace torii {
    class TransactionProcessorImpl : public TransactionProcessor {
     public:

      /**
       * @param pcs - provide information proposals and commits
       * @param os - ordering service for sharing transactions
       * @param validator - perform stateless validation
       * @param crypto_provider - sign income transactions
       */
      TransactionProcessorImpl(network::PeerCommunicationService &pcs,
                               const validation::StatelessValidator &validator);

      void transactionHandle(std::shared_ptr<model::Transaction> transaction) override;

      rxcpp::observable<std::shared_ptr<model::TransactionResponse>>
      transactionNotifier() override;

     private:
      // connections
      network::PeerCommunicationService &pcs_;

      // processing
      const validation::StatelessValidator &validator_;

      // internal
      rxcpp::subjects::subject<std::shared_ptr<model::TransactionResponse>>
          notifier_;
    };
  }  // namespace torii
}  // namespace iroha

#endif  // IROHA_TRANSACTION_PROCESSOR_STUB_HPP
