//
// Created by Berci on 2020. 12. 25..
//

#include <fmt/core.h>
#include "aocutils.h"

uint64_t search(uint64_t pk) {
    uint64_t subject_number = 7;
    uint64_t loop_size = 0;
    uint64_t value = 1;
    while (value != pk) {
        value = value * subject_number;
        value = value % 20201227;
        loop_size += 1;
    }
    return loop_size;
}

uint64_t transform(uint64_t subject_number, size_t loop_size) {
    uint64_t value = 1;
    for(size_t i = 0; i < loop_size; i++) {
        value = value * subject_number;
        value = value % 20201227;
    }
    return value;
}

int main() {
    /*
     * example input:
     *  card: 5764801
     *  door: 17807724
     *
     *  card_loop_size: 8
     *  door_loop_size: 11
     *
     *  encryption key: 14897079
     */
    uint64_t card_pk = 3418282;
    uint64_t door_pk = 8719412;

    uint64_t card_loop_size = search(card_pk);

    uint64_t door_loop_size = search(door_pk);

    uint64_t encryption_key_door = transform(door_pk, card_loop_size);
    uint64_t encryption_key_card = transform(card_pk, door_loop_size);
    assert(encryption_key_door == encryption_key_card);

    fmt::print("Part one: {}\n", encryption_key_door);
}