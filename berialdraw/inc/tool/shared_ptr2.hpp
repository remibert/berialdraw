#pragma once
#ifndef SWIG

#include <cstdint>
#include <cassert>
// #include <atomic> // Uncomment if you want thread-safe reference counting

namespace berialdraw
{
    /** Shared pointer implementation */
    template<typename T>
    class SharedPtr
    {
    public:
        typedef T element_type;

        /** Default constructor */
        SharedPtr() noexcept : m_ptr(nullptr), m_count(nullptr) {}

        /** Constructor from raw pointer */
        explicit SharedPtr(T* ptr) : m_ptr(ptr)
        {
            try {
                m_count = new uint32_t(1);
                // If using atomic:
                // m_count = new std::atomic<uint32_t>(1);
            } catch (...) {
                delete ptr;
                m_ptr = nullptr;
                m_count = nullptr;
            }
        }

        /** Copy constructor */
        SharedPtr(const SharedPtr& other) noexcept
            : m_ptr(other.m_ptr), m_count(other.m_count)
        {
            if (m_count) {
                ++(*m_count);
            }
        }

        /** Move constructor */
        SharedPtr(SharedPtr&& other) noexcept
            : m_ptr(other.m_ptr), m_count(other.m_count)
        {
            other.m_ptr = nullptr;
            other.m_count = nullptr;
        }

        /** Destructor */
        virtual ~SharedPtr()
        {
            destroy();
        }

        /** Copy assignment */
        SharedPtr& operator=(const SharedPtr& other)
        {
            if (this != &other && m_count != other.m_count) {
                destroy();
                m_ptr = other.m_ptr;
                m_count = other.m_count;
                if (m_count) {
                    ++(*m_count);
                }
            }
            return *this;
        }

        /** Move assignment */
        SharedPtr& operator=(SharedPtr&& other) noexcept
        {
            if (this != &other) {
                destroy();
                m_ptr = other.m_ptr;
                m_count = other.m_count;
                other.m_ptr = nullptr;
                other.m_count = nullptr;
            }
            return *this;
        }

        /** Dereference operator */
        T& operator*() const
        {
            assert(m_ptr && "Dereferencing null pointer");
            return *m_ptr;
        }

        /** Member access operator */
        T* operator->() const
        {
            assert(m_ptr && "Accessing member of null pointer");
            return m_ptr;
        }

        /** Get raw pointer */
        T* get() const noexcept
        {
            return m_ptr;
        }

        /** Get reference count */
        uint32_t count() const noexcept
        {
            return m_count ? *m_count : 0;
            // If using atomic:
            // return m_count ? m_count->load() : 0;
        }

    protected:
        /** Destroy the managed object and reference count */
        void destroy()
        {
            if (m_count) {
                if (--(*m_count) == 0) {
                    delete m_ptr;
                    delete m_count;
                }
                m_ptr = nullptr;
                m_count = nullptr;
            }
        }

        T* m_ptr;               /**< Raw pointer to the managed object */
        uint32_t* m_count;      /**< Reference count */
        // std::atomic<uint32_t>* m_count; // Use this instead for thread safety
    };
}

#endif // SWIG