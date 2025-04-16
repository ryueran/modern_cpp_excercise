#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <vector>
#include <bit>

enum MsgType: uint8_t
{
    MSG_TYPE_1,
    MSG_TYPE_2,
    MSG_TYPE_3,
};

#pragma pack(push, 1)
struct MsgBase
{
    uint32_t payload;
    MsgType type;
};
#pragma pack(pop)

template<typename T>
T endian_handle(T&& value)
{
    if(std::endian::native == std::endian::little)
    {
        uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(&value);
        std::reverse(byte_ptr, byte_ptr + sizeof(T));
    }

    return std::forward<T>(value);
}

template<typename T>
void serialize(T& msg, std::vector<uint8_t>& buffer)
{
    return;
}

template<typename T, auto Member, auto... Members>
void serialize(T& msg, std::vector<uint8_t>& buffer)
{
    auto *byte_ptr = reinterpret_cast<const uint8_t*>(&(endian_handle(msg.*Member)));
    buffer.insert(buffer.end(), byte_ptr, byte_ptr + sizeof(msg.*Member));
    serialize<T, Members...>(msg, buffer);
}

template<typename T, auto... Members>
void serialize_wrapper(T& msg, std::vector<uint8_t>& buffer)
{
    // Serialize the message
    serialize<T, Members...>(msg, buffer);
}

int main()
{
    MsgBase msg = {
        .payload = 8,
        .type = MsgType::MSG_TYPE_1,
    };

    std::vector<uint8_t> buffer;
    serialize_wrapper<MsgBase, &MsgBase::payload, &MsgBase::type>(msg, buffer);
    for(int i = 0; i < buffer.size(); ++i)
    {
        std::cout << unsigned(buffer[i]) << std::endl;
    }
    return 0;
}