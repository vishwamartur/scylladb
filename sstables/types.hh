#pragma once

#include "core/enum.hh"

namespace sstables {

// Some in-disk structures have an associated integer (of varying sizes) that
// represents how large they are. They can be a byte-length, in the case of a
// string, number of elements, in the case of an array, etc.
//
// For those elements, we encapsulate the underlying type in an outter
// structure that embeds how large is the in-disk size. It is a lot more
// convenient to embed it in the size than explicitly writing it in the parser.
// This way, we don't need to encode this information in multiple places at
// once - it is already part of the type.
template <typename Size>
struct disk_string {
    sstring value;
};

template <typename Size, typename Members>
struct disk_array {
    static_assert(std::is_integral<Size>::value, "Length type must be convertible to integer");
    std::vector<Members> elements;
};

template <typename Size, typename Key, typename Value>
struct disk_hash {
    std::unordered_map<Key, Value, std::hash<Key>> map;
};

struct option {
    disk_string<uint16_t> key;
    disk_string<uint16_t> value;
};

struct compression {
    disk_string<uint16_t> name;
    disk_array<uint32_t, option> options;
    uint32_t chunk_len;
    uint64_t data_len;
    disk_array<uint32_t, uint64_t> offsets;
};
}
