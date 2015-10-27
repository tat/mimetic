cmake_minimum_required (VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/examples)

add_executable(b64 b64.cxx)
target_link_libraries(b64 mimetic)

add_executable(qp qp.cxx)
target_link_libraries(qp mimetic)

# add_executable(exbin exbin.cxx)
# target_link_libraries(exbin mimetic)

add_executable(structure structure.cxx)
target_link_libraries(structure mimetic)

add_executable(catpart catpart.cxx)
target_link_libraries(catpart mimetic)

if (NOT WIN32)
    add_executable(buildidx buildidx.cxx)
    target_link_libraries(buildidx mimetic)
endif()

add_executable(search search.cxx)
target_link_libraries(search mimetic)

# add_executable(mbox mbox.cxx)
# target_link_libraries(mbox mimetic)

if (NOT WIN32)
    add_executable(mm mm.cxx engine.cxx cmdline.cxx)
    target_link_libraries(mm mimetic)
endif()

find_package(OpenSSL)
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    add_executable(smimestruct smimestruct.cxx)
    target_link_libraries(smimestruct mimetic ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})

    add_executable(smime2xml smime2xml.cxx)
    target_link_libraries(smime2xml mimetic ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
endif()

