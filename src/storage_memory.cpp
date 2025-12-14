#include "mindseye/ledger/storage.hpp"

namespace mindseye::ledger {

void MemoryStorage::append(const Block& b) {
  blocks_.push_back(b);
}

std::optional<Block> MemoryStorage::get_latest() const {
  if (blocks_.empty()) return std::nullopt;
  return blocks_.back();
}

std::vector<Block> MemoryStorage::read_all() const {
  return blocks_;
}

} // namespace mindseye::ledger
