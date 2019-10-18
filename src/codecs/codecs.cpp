//
// Created by ondesly on 2019-09-18.
// Copyright (c) 2019 ondesly. All rights reserved.
//

#include <cstddef>
#include <sstream>

#include <base64/libbase64.h>
#include <msgpack/msgpack.hpp>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <zlib.h>

#include "codecs.h"

// Zip

std::string codecs::zip::encode(const std::string &data) {
    std::string compressed;

    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
        return "";
    }

    zs.next_in = (Bytef *) data.data();
    zs.avail_in = uInt(data.size());

    int ret;
    char buffer[32768];
    do {
        zs.next_out = reinterpret_cast<Bytef *>(buffer);
        zs.avail_out = sizeof(buffer);

        ret = deflate(&zs, Z_FINISH);

        compressed.append(buffer, std::min(zs.total_out, zs.total_out - compressed.size()));
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        return "";
    }

    return compressed;
}

std::string codecs::zip::decode(const std::string &data) {
    if (data.empty()) {
        return "";
    }

    std::string decompressed;

    z_stream zs;
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK) {
        return "";
    }

    zs.next_in = (Bytef *) data.data();
    zs.avail_in = uInt(data.size());

    int ret;
    char buffer[32768];


    do {
        zs.next_out = reinterpret_cast<Bytef *>(buffer);
        zs.avail_out = sizeof(buffer);

        ret = inflate(&zs, 0);

        decompressed.append(buffer, std::min(zs.total_out, zs.total_out - decompressed.size()));
    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {
        return "";
    }

    return decompressed;
}

// Aes

unsigned char *make_key(const char *key) {
    auto key_len = strlen(key);

    char *out = new char[key_len];
    size_t out_len;

    base64_decode(key, key_len, out, &out_len, 0);

    return reinterpret_cast<unsigned char *>(out);
}

std::string codecs::aes::encode(const char *raw_key, const char *raw_iv, std::string data) {
    const auto encrypted_length = ((data.length() + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    data.append(encrypted_length - data.length(), char(0));

    unsigned char encrypted[encrypted_length];
    const auto key = make_key(raw_key);
    const auto iv = make_key(raw_iv);

    AES_KEY encrypt_key;
    AES_set_encrypt_key(key, 128, &encrypt_key);
    AES_cbc_encrypt((unsigned char *) data.c_str(), encrypted, data.length(), &encrypt_key, iv, AES_ENCRYPT);

    free(key);
    free(iv);

    return std::string((char *) encrypted, encrypted_length);
}

std::string codecs::aes::decode(const char *raw_key, const char *raw_iv, const std::string &data) {
    if (data.empty()) {
        return "";
    }

    unsigned char decrypted[data.size()];
    const auto key = make_key(raw_key);
    const auto iv = make_key(raw_iv);

    AES_KEY decrypt_key;
    AES_set_decrypt_key(key, 128, &decrypt_key);
    AES_cbc_encrypt((unsigned char *) data.c_str(), decrypted, data.length(), &decrypt_key, iv, AES_DECRYPT);

    free(key);
    free(iv);

    return std::string((char *) decrypted, data.size());
}

// Base64

std::string codecs::base64::encode(const std::string &data) {
    char out[(data.length() + 2) / 3 * 4];
    size_t out_len;

    base64_encode(data.c_str(), data.length(), out, &out_len, 0);

    return std::string(out, out_len);
}

std::string codecs::base64::decode(const std::string &data) {
    if (data.empty()) {
        return "";
    }

    char out[data.length() / 4 * 3 + 1];
    size_t out_len;

    base64_decode(data.c_str(), data.length(), out, &out_len, 0);

    return std::string(out, out_len);
}

// Msg

std::string codecs::msg::encode(const std::unordered_map<int, std::string> &data) {
    std::stringstream buffer;
    msgpack::pack(buffer, data);

    return buffer.str();
}

std::unordered_map<int, std::string> codecs::msg::decode(const std::string &data) {
    if (data.empty()) {
        return {};
    }

    auto oh = msgpack::unpack(data.data(), data.size());

    auto obj = oh.get();

    std::unordered_map<int, std::string> buffer;
    obj.convert(buffer);

    return buffer;
}

std::string codecs::msg::encode(const std::vector<int> &data) {
    std::stringstream buffer;
    msgpack::pack(buffer, data);

    return buffer.str();
}

std::vector<int> codecs::msg::decode_to_vector(const std::string &data) {
    if (data.empty()) {
        return {};
    }

    auto oh = msgpack::unpack(data.data(), data.size());

    auto obj = oh.get();

    std::vector<int> buffer;
    obj.convert(buffer);

    return buffer;
}
