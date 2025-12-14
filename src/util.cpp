#include "mindseye/ledger/util.hpp"

namespace mindseye::ledger {

std::string to_hex(const std::vector<uint8_t>& bytes) {
  static const char* hex = "0123456789abcdef";
  std::string out;
  out.reserve(bytes.size() * 2);
  for (auto b : bytes) {
    out.push_back(hex[(b >> 4) & 0xF]);
    out.push_back(hex[b & 0xF]);
  }
  return out;
}

std::vector<uint8_t> to_bytes(const std::string& s) {
  return std::vector<uint8_t>(s.begin(), s.end());
}

} // namespace mindseye::ledger
