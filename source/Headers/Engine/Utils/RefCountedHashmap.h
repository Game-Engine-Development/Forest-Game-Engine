#pragma once

#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <optional>
#include <cassert>
#include <string>

#include "Headers/Engine/Constants.h"

//must be defined in header since it uses class template arguments
template <typename Key, typename Value>
class RefCountedHashmap {
    //WARNING: If you have two RefCountedHashmap's with the same template types, this will mean it is referring to the same hashmap
    // (might cause weird behavior if you insert the same thing into two of these RefCountedHashmap's)
    static inline std::unordered_map<Key, std::pair<Value, RefCount>> hashmapImpl;

    std::optional<Key> hashmapKey = std::nullopt;
    Value *value = nullptr;

    friend void swap(RefCountedHashmap &hashmap1, RefCountedHashmap &hashmap2) {
        std::swap(hashmap1.hashmapKey, hashmap2.hashmapKey);
        std::swap(hashmap1.value, hashmap2.value);
    }

public:
    RefCountedHashmap() = delete;

    template <typename ...Args>
    explicit RefCountedHashmap(const Key &key, Args &&...args) : hashmapKey(key) {
        const auto item = hashmapImpl.find(hashmapKey.value());

        if(item != std::end(hashmapImpl)) item->second.second += 1;
        else hashmapImpl.emplace(Key(hashmapKey.value()), std::make_pair(Value(std::forward<Args>(std::move(args))...), 1u));

        const auto newItem = hashmapImpl.find(key);
        value = &(newItem->second.first);
    }

    RefCountedHashmap(const RefCountedHashmap &hashmap) : hashmapKey(hashmap.hashmapKey), value(hashmap.value) {
        if(hashmapKey.has_value()) hashmapImpl.at(hashmapKey.value()).second += 1;
    }
    RefCountedHashmap(RefCountedHashmap &&oldHashmap) noexcept
    : hashmapKey(std::move(oldHashmap.hashmapKey)), value(oldHashmap.value)
    {
        oldHashmap.hashmapKey = std::nullopt;
        oldHashmap.value = nullptr;
    }

    RefCountedHashmap& operator=(const RefCountedHashmap &hashmap) {
        RefCountedHashmap copy(hashmap);
        swap(*this, copy);
        return *this;
    }
    RefCountedHashmap& operator=(RefCountedHashmap &&oldHashmap) noexcept {
        RefCountedHashmap move(std::move(oldHashmap));
        swap(*this, move);
        return *this;
    }

    Value& getElement(const std::string &filename) {
        return *value;
    }

    ~RefCountedHashmap() {
        if(hashmapKey.has_value()) {
            const auto item = hashmapImpl.find(hashmapKey.value());
            assert(item != std::end(hashmapImpl));
            item->second.second -= 1;

            //if the refcount drops to 0, delete the element
            if (!item->second.second) hashmapImpl.erase(hashmapKey.value());
        }
    }
};
