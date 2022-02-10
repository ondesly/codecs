#
#  CMakeLists.txt
#  codecs
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 10.02.2022.
#  Copyright © 2022 Dmitrii Torkhov. All rights reserved.
#

set(SRC
		base64/lib/codec_choose.c
		base64/lib/lib.c
		base64/lib/arch/avx/codec.c
		base64/lib/arch/avx2/codec.c
		base64/lib/arch/generic/codec.c
		base64/lib/arch/neon32/codec.c
		base64/lib/arch/neon64/codec.c
		base64/lib/arch/sse41/codec.c
		base64/lib/arch/sse42/codec.c
		base64/lib/arch/ssse3/codec.c

		crypto/aes/aes_core.c
		crypto/aes/aes_cbc.c
		crypto/modes/cbc128.c

		codecs/codecs.cpp)

add_library(codecs STATIC ${SRC})

target_include_directories(codecs
		PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>
		PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/msgpack>)

set_target_properties(codecs PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)