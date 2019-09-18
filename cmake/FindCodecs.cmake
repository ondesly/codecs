set(CODECS_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)

set(CODECS_INCLUDE_DIRS ${CODECS_ROOT}/include ${CODECS_ROOT}/libs/msgpack/include ${CODECS_ROOT}/src)

#Base64

set(BASE64_ROOT ${CODECS_ROOT}/libs/base64)

set(BASE64_SRC
	${BASE64_ROOT}/lib/codec_choose.c
	${BASE64_ROOT}/lib/lib.c
    ${BASE64_ROOT}/lib/arch/avx/codec.c
    ${BASE64_ROOT}/lib/arch/avx2/codec.c
    ${BASE64_ROOT}/lib/arch/generic/codec.c
    ${BASE64_ROOT}/lib/arch/neon32/codec.c
    ${BASE64_ROOT}/lib/arch/neon64/codec.c
    ${BASE64_ROOT}/lib/arch/sse41/codec.c
    ${BASE64_ROOT}/lib/arch/sse42/codec.c
    ${BASE64_ROOT}/lib/arch/ssse3/codec.c)

#OpenSSL

set(OPENSSL_ROOT ${CODECS_ROOT}/libs/openssl)

set(OPENSSL_SRC
    ${OPENSSL_ROOT}/crypto/aes/aes_core.c
    ${OPENSSL_ROOT}/crypto/aes/aes_cbc.c
    ${OPENSSL_ROOT}/crypto/modes/cbc128.c)

#Codecs

set(CODECS_SRC
    ${CODECS_ROOT}/src/codecs/codecs.cpp)

#Lib

add_library(codecs STATIC ${BASE64_SRC} ${OPENSSL_SRC} ${CODECS_SRC})