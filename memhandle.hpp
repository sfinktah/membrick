#pragma once

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

    static memBrick scan(memBrick base, std::size_t size, const char* pattern)
    {
        struct nibble
        {
            std::uint8_t value  = 0;
            std::uint8_t offset = 0;
        } nibbles[128];

        std::size_t count = 0;

        for (; pattern; pattern = std::strpbrk(pattern, " "))
        {
            pattern += std::strspn(pattern, " "); // Discard whitespace

            if (pattern[0] != '?')
            {
                nibbles[count].value      = std::uint8_t(std::strtol(pattern, NULL, 16));
                nibbles[count + 1].offset = nibbles[count].offset;

                count++;
            }

            nibbles[count].offset++;
        }

        for (std::size_t i = 0; i < size - nibbles[count].offset; i++)
        {
            memBrick currentOffset = base.offset(i);

            bool found = true;

            for (std::size_t j = 0; j < count; ++j)
            {
                if (nibbles[j].value != currentOffset.offset(nibbles[j].offset).read<std::uint8_t>())
                {
                    found = false;

                    break;
                }
            }

            if (found)
            {
                return currentOffset;
            }
        }

        return nullptr;
    }

    template <typename T>
    std::enable_if_t<std::is_pointer<T>::value, T> as() const
    {
        return reinterpret_cast<T>(this->_handle);
    }

    template <typename T>
    std::enable_if_t<std::is_lvalue_reference<T>::value, T> as() const
    {
        return *reinterpret_cast<std::remove_reference_t<T>*>(this->_handle);
    }

    template <typename T>
    std::enable_if_t<std::is_same<T, std::uintptr_t>::value, T> as() const
    {
        return reinterpret_cast<std::uintptr_t>(this->_handle);
    }

    template <typename T>
    std::enable_if_t<std::is_same<T, std::intptr_t>::value, T> as() const
    {
        return reinterpret_cast<std::intptr_t>(this->_handle);
    }

    operator bool()
    {
        return (this->as<void*>() != nullptr);
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
};
