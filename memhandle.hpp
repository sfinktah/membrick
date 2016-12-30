class memHandle
{
protected:
    void* _handle;

public:
    memHandle()
        : _handle(nullptr)
    { }

    memHandle(void* p)
        : _handle(p)
    { }

    memHandle(std::uintptr_t p)
        : _handle(reinterpret_cast<void*>(p))
    { }

    memHandle(const memHandle& copy)
        : _handle(copy._handle)
    { }

    static memHandle scan(const char* pattern)
    { return {}; }

    static memHandle scan(const char* pattern, const char* mask)
    { }

    template <typename T>
    T* get()
    {
        return reinterpret_cast<T*>(this->_handle);
    }

    template <typename T>
    T& as()
    {
        return *this->get<T>();
    }

    memHandle save(memHandle& out)
    {
        return out = *this;
    }

    memHandle offset(std::intptr_t offset)
    {
        return this->get<char>() + offset;
    }

    memHandle deference()
    {
        return *this->get<void*>();
    }

    memHandle iprelative(std::uintptr_t ipoffset)
    {
        return this->offset(ipoffset).offset(this->as<int>());
    }

    memHandle rva(memHandle va)
    {
        return va.offset(this->as<int>());
    }
    
    //memHandle set(const unsigned char c, std::size_t size)
    //{
    //    return memset(this->get<void>(), c, size);
    //}

    //memHandle nop(std::size_t size)
    //{
    //    return this->set(0x90, size);
    //}

    //memHandle zero(std::size_t size)
    //{
    //    return this->set(0x00, size);
    //}

    //memHandle write(const void* bytes, std::size_t size)
    //{
    //    return memcpy(this->get<void>(), bytes, size);
    //}

    //memHandle read(void* buffer, std::size_t size)
    //{
    //    return memcpy(buffer, this->get<const void>(), size);
    //}
};