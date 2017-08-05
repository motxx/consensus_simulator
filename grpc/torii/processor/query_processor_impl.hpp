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

#ifndef IROHA_QUERY_PROCESSOR_IMPL_HPP
#define IROHA_QUERY_PROCESSOR_IMPL_HPP

#include "model/query_execution.hpp"
#include "validation/stateless_validator.hpp"
#include "torii/processor/query_processor.hpp"

namespace iroha {
  namespace torii {

    /**
     * QueryProcessor provides start point for queries in the whole system
     */
    class QueryProcessorImpl : public QueryProcessor {
     public:
      explicit QueryProcessorImpl(
          model::QueryProcessingFactory &qpf,
          validation::StatelessValidator &stateless_validator);

      /**
       * Register client query
       * @param query - client intent
       */
      void queryHandle(std::shared_ptr<model::Query> query) override;

      /**
       * Subscribe for query responses
       * @return observable with query responses
       */
      rxcpp::observable<std::shared_ptr<model::QueryResponse>> queryNotifier()
          override;

     private:
      rxcpp::subjects::subject<std::shared_ptr<model::QueryResponse>> subject_;
      model::QueryProcessingFactory &qpf_;
      validation::StatelessValidator &validator_;
    };
  }
}

#endif  // IROHA_QUERY_PROCESSOR_IMPL_HPP
