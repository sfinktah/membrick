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

    memBrick(std::nullptr_t p)
        : _handle(p)
    { }

    memBrick(const memBrick& copy)
        : _handle(copy._handle)
    { }


    static memBrick scan(const char* pattern)
    { }

    static memBrick scan(const char* pattern, const char* mask)
    { }

    template <typename T>
    T as() const
    {
        static_assert(std::is_pointer<T>::value, "Type is not a pointer");

        return reinterpret_cast<T>(this->_handle);
    }

    template <>
    std::uintptr_t as<std::uintptr_t>() const
    {
        return reinterpret_cast<std::uintptr_t>(this->_handle);
    }

    template <>
    std::intptr_t as<std::intptr_t>() const
    {
        return reinterpret_cast<std::intptr_t>(this->_handle);
    }

    template <typename T>
    T read() const
    {
        return *this->as<T*>();
    }

    memBrick save(memBrick& out) const
    {
        return (out = *this);
    }

    memBrick offset(std::intptr_t offset) const
    {
        return this->as<std::uintptr_t>() + offset;
    }

    memBrick rip(std::uintptr_t ipoffset) const
    {
        return this->offset(ipoffset).offset(this->read<int>());
    }

    memBrick translate(memBrick from, memBrick to) const
    {
        return to.offset(this->as<std::intptr_t>() - from.as<std::intptr_t>());
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
