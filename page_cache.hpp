#ifndef PAGE_CACHE_HPP
#define PAGE_CACHE_HPP

#include "global_var.hpp"
#include "span_list.hpp"
#include <cstddef>
#include <mutex>
#include <unordered_map>
namespace lmem {

class PageCache{
public:
    static PageCache* get_instance();
    Span* new_raw_span(std::size_t k);
    void return_span_to_pc(Span* span);
    void lock()const;
    void unlock()const;
    Span* map_obj_to_span(void* obj)const;
private:
    PageCache() = default;
    PageCache(PageCache const&) = delete;
    PageCache& operator=(PageCache const&) = delete;
    
private:
    static PageCache* instance_;
    SpanList span_lists_[MAX_PAGE_NUM];
    mutable std::mutex page_mutex_;
    std::unordered_map<PageID_t, Span*>   pageID_span_map_;
};

}
#endif