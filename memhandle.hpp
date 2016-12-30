/*
 *           _             
 * __  _ __ |_) __ o  _  | 
 * |||(/_||||_) |  | (_  |<
 * 
 */
class memBrick
{
protected:
    void* _handle;

public:
    memBrick()
        : _handle(nullptr)
    { }

    memBrick(void* p)
        : _handle(p)
    { }

    memBrick(std::uintptr_t p)
        : _handle(reinterpret_cast<void*>(p))
    { }

    memBrick(const memBrick& copy)
        : _handle(copy._handle)
    { }

    static memBrick scan(const char* pattern)
    { return {}; }

    static memBrick scan(const char* pattern, const char* mask)
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

    memBrick save(memBrick& out)
    {
        return out = *this;
    }

    memBrick offset(std::intptr_t offset)
    {
        return this->get<char>() + offset;
    }

    memBrick deference()
    {
        return *this->get<void*>();
    }

    memBrick iprelative(std::uintptr_t ipoffset)
    {
        return this->offset(ipoffset).offset(this->as<int>());
    }

    memBrick rva(memBrick va)
    {
        return va.offset(this->as<int>());
    }
    
    //memBrick set(const unsigned char c, std::size_t size)
    //{
    //    return memset(this->get<void>(), c, size);
    //}

    //memBrick nop(std::size_t size)
    //{
    //    return this->set(0x90, size);
    //}

    //memBrick zero(std::size_t size)
    //{
    //    return this->set(0x00, size);
    //}

    //memBrick write(const void* bytes, std::size_t size)
    //{
    //    return memcpy(this->get<void>(), bytes, size);
    //}

    //memBrick read(void* buffer, std::size_t size)
    //{
    //    return memcpy(buffer, this->get<const void>(), size);
    //}
};
