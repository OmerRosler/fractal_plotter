#pragma once
#include <array>
#include <cassert>
#include <cstdint>
#include <deque>

namespace frc
{
class packed_5bit_queue {
public:
    static constexpr uint64_t BITS_PER_COUNTER = 5;
    static constexpr uint64_t COUNTERS_PER_PAGE = 512;
    static constexpr uint64_t WORDS_PER_PAGE =
        (COUNTERS_PER_PAGE * BITS_PER_COUNTER) / 64;

private:
    struct Page {
        std::array<uint64_t, WORDS_PER_PAGE> words{};

        Page() {
            words.fill(0);
        }
    };

    std::deque<Page> pages;

    // Absolute logical indices
    uint64_t head_index = 0;
    uint64_t tail_index = 0;

    // Logical index of pages.front()
    uint64_t first_page_index = 0;

private:
    static constexpr uint64_t MASK = 0x1FULL;

    static uint64_t page_of(uint64_t idx) {
        return idx / COUNTERS_PER_PAGE;
    }

    static uint64_t local_of(uint64_t idx) {
        return idx % COUNTERS_PER_PAGE;
    }

    Page& get_page_for_write(uint64_t absolute_page) {
        while (absolute_page >= first_page_index + pages.size()) {
            pages.emplace_back();
        }

        return pages[absolute_page - first_page_index];
    }

    const Page& get_page_for_read(uint64_t absolute_page) const {
        return pages[absolute_page - first_page_index];
    }

    static void write_5bits(
        Page& page,
        uint64_t local_index,
        uint8_t value
    ) {
        value &= MASK;

        const uint64_t bitpos = local_index * BITS_PER_COUNTER;

        const uint64_t word = bitpos >> 6;
        const uint64_t offset = bitpos & 63;

        if (offset <= 59) {
            // Fits in one word
            page.words[word] |=
                (uint64_t(value) << offset);
        }
        else {
            // Crosses boundary
            const uint64_t low_bits = 64 - offset;
            const uint64_t high_bits = 5 - low_bits;

            const uint64_t low_mask =
                (1ULL << low_bits) - 1ULL;

            page.words[word] |=
                uint64_t(value & low_mask) << offset;

            page.words[word + 1] |=
                uint64_t(value >> low_bits);
        }
    }

    static uint8_t read_5bits(
        const Page& page,
        uint64_t local_index
    ) {
        const uint64_t bitpos =
            local_index * BITS_PER_COUNTER;

        const uint64_t word = bitpos >> 6;
        const uint64_t offset = bitpos & 63;

        if (offset <= 59) {
            return uint8_t(
                (page.words[word] >> offset) & MASK
            );
        }

        // Crosses boundary
        const uint64_t low_bits = 64 - offset;
        const uint64_t high_bits = 5 - low_bits;

        const uint64_t low =
            (page.words[word] >> offset) &
            ((1ULL << low_bits) - 1ULL);

        const uint64_t high =
            page.words[word + 1] &
            ((1ULL << high_bits) - 1ULL);

        return uint8_t(low | (high << low_bits));
    }

    void reclaim_pages() {
        const uint64_t needed_first_page =
            page_of(head_index);

        while (first_page_index < needed_first_page) {
            pages.pop_front();
            ++first_page_index;
        }
    }

public:
    packed_5bit_queue() = default;

    inline bool empty() const {
        return head_index == tail_index;
    }

    inline uint64_t size() const {
        return tail_index - head_index;
    }

    void push(uint8_t value) {
        assert(value < 32);

        const uint64_t idx = tail_index;

        const uint64_t absolute_page =
            page_of(idx);

        const uint64_t local_index =
            local_of(idx);

        Page& page =
            get_page_for_write(absolute_page);

        write_5bits(page, local_index, value);

        ++tail_index;
    }

    uint8_t pop() {
        const uint64_t idx = head_index;

        const uint64_t absolute_page =
            page_of(idx);

        const uint64_t local_index =
            local_of(idx);

        const Page& page =
            get_page_for_read(absolute_page);

        const uint8_t result =
            read_5bits(page, local_index);

        ++head_index;

        reclaim_pages();

        return result;
    }
};

}