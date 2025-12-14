#include "mindseye/ledger/ledger.hpp"
#include "mindseye/ledger/errors.hpp"
#include "mindseye/ledger/block.hpp"

#include <utility>

namespace mindseye::ledger {

Ledger::Ledger(std::shared_ptr<IStorage> storage)
  : storage_(std::move(storage)) {
  if (!storage_) throw LedgerError("Ledger: storage is null");
}

Block Ledger::append_block(std::vector<std::vector<uint8_t>> entries, int64_t ts_ms) {
  std::string prev = "GENESIS";
  if (auto latest = storage_->get_latest(); latest.has_value()) {
    prev = latest->block_hash_hex;
  }

  Block b;
  b.block_id = "blk-" + std::to_string(ts_ms);
  b.prev_hash = prev;
  b.entries = std::move(entries);
  b.ts_ms = ts_ms;

  b = finalize_block(std::move(b));
  storage_->append(b);
  return b;
}

bool Ledger::verify_chain(std::string* error) const {
  auto blocks = storage_->read_all();
  if (blocks.empty()) return true;

  for (size_t i = 0; i < blocks.size(); ++i) {
    const auto& b = blocks[i];

    // recompute
    Block recomputed = finalize_block(Block{
      b.block_id, b.prev_hash, b.entries, b.ts_ms, "", ""
    });

    if (recomputed.merkle_root_hex != b.merkle_root_hex) {
      if (error) *error = "Merkle root mismatch at index " + std::to_string(i);
      return false;
    }

    if (recomputed.block_hash_hex != b.block_hash_hex) {
      if (error) *error = "Block hash mismatch at index " + std::to_string(i);
      return false;
    }

    if (i == 0) continue;
    const auto& prev = blocks[i - 1];
    if (b.prev_hash != prev.block_hash_hex) {
      if (error) *error = "Prev hash mismatch at index " + std::to_string(i);
      return false;
    }
  }

  return true;
}

std::vector<Block> Ledger::read_all() const {
  return storage_->read_all();
}

} // namespace mindseye::ledger
