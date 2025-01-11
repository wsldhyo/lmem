#ifndef PAGE_CACHE_HPP
#define PAGE_CACHE_HPP

#include "global_var.hpp"
#include "span_list.hpp"
#include <cstddef>
#include <mutex>
#include <unordered_map>
namespace lmem {


// 
// @brief 页缓存类
// PC中也有Span的哈希桶。但映射规则与CC和TC中的不一样。CC和TC中的哈希桶是依据内存块
// 大小进行映射的。而PC中的哈希桶是依据页数映射的，哈希索引为i的SpanList，其管理的
// 每个Span都维护i页内存
//
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