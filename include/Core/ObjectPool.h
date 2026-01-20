//
// Created by Oier Álvarez on 20/01/2026.
//

#ifndef SPACESTATIONDEFENSE_OBJECTPOOL_H
#define SPACESTATIONDEFENSE_OBJECTPOOL_H

#include <vector>
#include <memory>

template <typename T>
class ObjectPool
{
public:
    struct Item
    {
        bool active = false;
        std::unique_ptr<T> object;
    };

    explicit ObjectPool(std::size_t capacity)
    {
        m_items.reserve(capacity);
        for (std::size_t i = 0; i < capacity; ++i)
        {
            Item item;
            item.active = false;
            item.object = std::make_unique<T>(); // T constructed once
            m_items.push_back(std::move(item));
        }
    }

    template <typename ResetFn>
    T* acquire(ResetFn&& resetFn)
    {
        for (auto& item : m_items)
        {
            if (!item.active)
            {
                item.active = true;
                resetFn(*item.object);
                return item.object.get();
            }
        }
        return nullptr;
    }

    void release(T* ptr)
    {
        for (auto& item : m_items)
        {
            if (item.object.get() == ptr)
            {
                item.active = false;
                return;
            }
        }
    }

    template <typename Fn>
    void forEachActive(Fn&& fn)
    {
        for (auto& item : m_items)
        {
            if (item.active)
                fn(*item.object);
        }
    }

private:
    std::vector<Item> m_items;
};


#endif //SPACESTATIONDEFENSE_OBJECTPOOL_H