#pragma once

#include "allocer_base.h"
#include <vector>

namespace jstruct {

class AlignAllocer : public AllocerBase {
public:
    AlignAllocer(size_t block_size = 1024 * 4) : _block_size(block_size >> 3),
            _cur(new uint64_t[_block_size]), _end(_cur + _block_size), _block_idx(0), _pool({ _cur }) {}

    void clear() override {
        _block_idx = 0;
        _cur = _pool[0];
        _end = _cur + _block_size;

        for (size_t i = 0, sz = _indiv.size(); i < sz; ++i) {
            delete [] _indiv[i];
        }
        _indiv.clear();
    }

    size_t space_used() override {
        return ((_block_size * (_block_idx + 1)) + (_cur - _end)) * 8;
    }
    size_t total_space() override {
        return _pool.size() * _block_size * 8;
    }

    void * malloc(size_t sz) override {
        sz = ((((sz != 0) ? sz : 1) - 1) >> 3) + 1;

        uint64_t * pos = _cur;
        if ((_cur += sz) <= _end) {
            return (void *) pos;
        } else if (sz <= _block_size) {
            uint64_t * blk = get_block();
            _cur = blk + sz;
            _end = blk + _block_size;
            return blk;
        } else {
            uint64_t * buf = new uint64_t[sz];
            _indiv.push_back(buf);
            return (void *) buf;
        }
    }

private:
    uint64_t * get_block() {
        if (++_block_idx < _pool.size()) {
            return _pool[_block_idx];
        } else {
            uint64_t * blk = new uint64_t[_block_size];
            _pool.push_back(blk);
            return blk;
        }
    }

private:
    size_t _block_size;

    uint64_t * _cur, *_end;

    size_t _block_idx;
    std::vector<uint64_t *> _pool;

    std::vector<uint64_t *> _indiv;
};

} // ns jstruct
