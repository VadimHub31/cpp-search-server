#pragma once

#include <iostream>
#include <vector>

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator range_begin, Iterator range_end, size_t page_size) : begin_(range_begin), end_(range_end), size_(page_size) {};

    Iterator begin() const {
        return begin_;
    }
    Iterator end() const {
        return end_;
    }
    size_t size() const {
        return size_;
    }
private:
    Iterator begin_;
    Iterator end_;
    size_t size_;
};

template <typename Iterator>
std::ostream& operator<<(std::ostream& output, const IteratorRange<Iterator> range) {
    for (auto i = range.begin(); i != range.end(); ++i) {
        output << *i;
    }
    return output;
}

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator range_begin, Iterator range_end, size_t page_size) {
        int container_size = distance(range_begin, range_end);
        
        while (container_size >= static_cast<int>(page_size)) {
            Iterator page_begin = range_begin;
            advance(range_begin, static_cast<int>(page_size));
            Iterator page_end = range_begin;
            pages_.push_back({page_begin, page_end, page_size});
            container_size -= static_cast<int>(page_size);
        }
        if (container_size < static_cast<int>(page_size) && container_size != 0) {
            Iterator page_begin = range_begin;
            advance(range_begin, container_size);
            Iterator page_end = range_begin;
            pages_.push_back({page_begin, page_end, page_size});
        }
    }

        auto begin() const {
            return pages_.begin();
        }
        auto end() const {
            return pages_.end();
        }
        size_t size() const {
            return pages_.size();
        }

private:
    std::vector<IteratorRange<Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}