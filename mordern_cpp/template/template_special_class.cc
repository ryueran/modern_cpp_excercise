#include <iostream>
#include <vector>
#include <stdint.h>

struct Strings
{
    uint8_t len;
    std::vector<uint8_t> content;
};

enum MessageType: uint8_t
{
    MESSAGE_TYPE_1,
    MESSAGE_TYPE_2,
    MESSAGE_TYPE_3,
    MESSAGE_TYPE_4,
    MESSAGE_TYPE_5,
};


struct ConnectMessage
{
    MessageType msgTyp;
    uint32_t payload;
};

struct StoreMessage
{
    MessageType msgTyp;
    uint32_t ply_size;
    Strings key;
    Strings value;
};
// Deserialization function
template<typename T>
void deserialize_impl(T& obj, const std::vector<uint8_t>& buffer, size_t& offset)
{

}
template<typename T>
void deserialize_member(T& member, const std::vector<uint8_t>& buffer, size_t& offset)
{
    auto *byte_ptr = reinterpret_cast<uint8_t*>(&member);
    std::copy(buffer.begin() + offset, buffer.begin() + offset + sizeof(T), byte_ptr);
    offset += sizeof(T);
}

template<>
void deserialize_member<Strings>(Strings& member, const std::vector<uint8_t>& buffer, size_t& offset)
{
    deserialize_member(member.len, buffer, offset);
    member.content.insert(member.content.end(), buffer.begin() + offset, buffer.begin() + offset + member.len);
    offset += member.len;
}

template<typename T, auto Member, auto... Members>
void deserialize_impl(T& obj, const std::vector<uint8_t>& buffer, size_t& offset)
{
    deserialize_member(obj.*Member, buffer, offset);
    deserialize_impl<T, Members...>(obj, buffer, offset);
}

template<typename T, auto... Members>
void deserialize(T& obj, const std::vector<uint8_t>& buffer, size_t& offset)
{
    deserialize_impl<T, Members...>(obj, buffer, offset);
}

// Serialization function
template<typename T>
void serialize_impl(const T& member, std::vector<uint8_t>& buffer)
{

}

template<typename T>
void serialize_member(const T& member, std::vector<uint8_t>& buffer)
{
    auto *byte_ptr = reinterpret_cast<const uint8_t*>(&member);
    buffer.insert(buffer.end(), byte_ptr, byte_ptr + sizeof(T));
}

template<>
void serialize_member<Strings>(const Strings& member, std::vector<uint8_t>& buffer)
{
    buffer.push_back(member.len);
    buffer.insert(buffer.end(), member.content.begin(), member.content.end());
}

template<typename T, auto Member, auto... Members>
void serialize_impl(const T& obj, std::vector<uint8_t>& buffer)
{
    serialize_member(obj.*Member, buffer);
    serialize_impl<T, Members...>(obj, buffer);
}

template<typename T, auto... Members>
void serialize(const T& obj, std::vector<uint8_t>& buffer)
{
    serialize_impl<T, Members...>(obj, buffer);
}

int main()
{
    struct ConnectMessage conMsg = {
        .msgTyp = MESSAGE_TYPE_1,
        .payload = 8,
    };
    std::vector<uint8_t> buffer;
    serialize<ConnectMessage, &ConnectMessage::msgTyp, &ConnectMessage::payload>(conMsg, buffer);
    std::cout << "Serialized data: ";
    for (const auto& byte : buffer)
    {
        std::cout << unsigned(byte) << " ";
    }
    std ::cout << std::endl;

    struct StoreMessage storeMsg = {
        .msgTyp = MESSAGE_TYPE_2,
        .ply_size = 8,
        .key = {5, {1, 2, 3, 4, 5}},
        .value = {3, {6, 7, 8}},
    };
    std::vector<uint8_t> storeBuffer;
    serialize<StoreMessage, &StoreMessage::msgTyp, &StoreMessage::ply_size, &StoreMessage::key, &StoreMessage::value>(storeMsg, storeBuffer);
    std::cout << "Serialized store message: ";
    for (const auto& byte : storeBuffer)
    {
        std::cout << unsigned(byte) << " ";
    }
    std::cout << std::endl;

    struct StoreMessage deserializedStoreMsg = {};
    size_t offset = 0;
    deserialize<StoreMessage, &StoreMessage::msgTyp, &StoreMessage::ply_size, &StoreMessage::key, &StoreMessage::value>(deserializedStoreMsg, storeBuffer, offset);
    std::cout << "Deserialized store message: " << unsigned(deserializedStoreMsg.msgTyp) << ", " << deserializedStoreMsg.ply_size << ", ";
    std::cout << "key: ";
    for (const auto& byte : deserializedStoreMsg.key.content)
    {
        std::cout << unsigned(byte) << " ";
    }
    std::cout << ", value: ";
    for (const auto& byte : deserializedStoreMsg.value.content)
    {
        std::cout << unsigned(byte) << " ";
    }
    std::cout << std::endl;

    return 0;
}
