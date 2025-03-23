# 查找 OpenSSL
find_package(OpenSSL REQUIRED)

add_executable(ssl_server main.cc)

target_link_libraries(ssl_server OpenSSL::SSL OpenSSL::Crypto)