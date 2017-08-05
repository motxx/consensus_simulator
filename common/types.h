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

#ifndef CONSENSUS_SUMERAGI_MODEL_SIGNATURE_H
#define CONSENSUS_SUMERAGI_MODEL_SIGNATURE_H

#include <array>
#include <iomanip>
#include <cstdlib>
#include <nonstd/optional.hpp>

namespace common {
  namespace types {
    template <size_t Bits>
    struct blob : public std::array<uint8_t, Bits> {
      uint8_t at(size_t i) const {
        return this->std::array<uint8_t, Bits>::at(i);
      }
      std::string to_hexstring() const {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (size_t i = 0; i < size(); ++i) {
          ss << std::setw(2) << static_cast<unsigned>(at(i));
        }
        return ss.str();
      }
      constexpr size_t size() const {
        return this->std::array<uint8_t, Bits>::size();
      }
      static nonstd::optional<blob<Bits>> hex_to_blob(std::string const& hex) {
        blob<Bits> bytes;
        if (hex.size() != bytes.size() * 2) {
          return nonstd::nullopt;
        }
        const auto hex_size = hex.size();
        for (size_t i = 0; i < hex_size; i += 2) {
          const auto byteString = hex.substr(i, 2);
          const auto byte = static_cast<uint8_t>(std::strtol(byteString.c_str(), NULL, 16));
          bytes[i / 2] = byte;
        }
        return bytes;
      }
    };

    struct sig_t : public blob<64> {};
    struct pubkey_t : public blob<32> {
      pubkey_t& operator= (blob<32> const& rhs) {
        std::copy(rhs.begin(), rhs.end(), this->begin());
        return *this;
      }
    };
    struct privkey_t : public blob<64> {};

  }  // namespace types
}  // namespace common

#endif  // CONSENSUS_SUMERAGI_MODEL_SIGNATURE_H
