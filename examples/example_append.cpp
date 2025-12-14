#include "mindseye/ledger/ledger.hpp"
#include "mindseye/ledger/storage.hpp"

#include <chrono>
#include <iostream>

static int64_t now_ms() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main() {
  auto storage = std::make_shared<mindseye::ledger::MemoryStorage>();
  mindseye::ledger::Ledger ledger(storage);

  std::vector<std::vector<uint8_t>> entries;
  entries.push_back(std::vector<uint8_t>{'h','e','l','l','o'});
  entries.push_back(std::vector<uint8_t>{'w','o','r','l','d'});

  auto b = ledger.append_block(std::move(entries), now_ms());
  std::cout << "block_id=" << b.block_id << "\n";
  std::cout << "prev=" << b.prev_hash << "\n";
  std::cout << "merkle=" << b.merkle_root_hex << "\n";
  std::cout << "hash=" << b.block_hash_hex << "\n";
  return 0;
}
