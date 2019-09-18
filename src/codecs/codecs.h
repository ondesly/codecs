//
// Created by ondesly on 2019-09-18.
// Copyright (c) 2019 ondesly. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>

namespace codecs {

    class zip {
    public:

        static std::string encode(const std::string &data);

        static std::string decode(const std::string &data);

    };

    class aes {
    public:

        static std::string encode(const char *raw_key, const char *raw_iv, std::string data);

        static std::string decode(const char *raw_key, const char *raw_iv, const std::string &data);

    };

    class base64 {
    public:

        static std::string encode(const std::string &data);

        static std::string decode(const std::string &data);

    };

    class msg {
    public:

        static std::string encode(const std::unordered_map<int, std::string> &data);

        static std::unordered_map<int, std::string> decode(const std::string &data);

    };

}