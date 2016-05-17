cmake_minimum_required (VERSION 2.8)

include_directories(${PROJECT_SOURCE_DIR})

set(MIMETIC_CPP_FILES
    body.cxx
    contentdescription.cxx
    contentdisposition.cxx
    contentid.cxx
    contenttransferencoding.cxx
    contenttype.cxx
    fieldparam.cxx
    header.cxx
    message.cxx
    mimeentity.cxx
    mimeversion.cxx
    strutils.cxx
    utils.cxx
    version.cxx
    rfc822/mailboxlist.cxx
    rfc822/group.cxx
    rfc822/address.cxx
    rfc822/fieldvalue.cxx
    rfc822/addresslist.cxx
    rfc822/message.cxx
    rfc822/mailbox.cxx
    rfc822/messageid.cxx
    rfc822/field.cxx
    rfc822/datetime.cxx
    rfc822/header.cxx
    codec/qp.cxx
    codec/base64.cxx
    os/file_iterator.cxx
    os/utils.cxx
    os/fileop.cxx
    os/stdfile.cxx
)
if (NOT WIN32)
    list(APPEND MIMETIC_CPP_FILES os/mmfile.cxx)
endif()

set(MIMETIC_H_FILES
    body.h
    circular_buffer.h
    config_win32.h
    contentdescription.h
    contentdisposition.h
    contentid.h
    contenttransferencoding.h
    contenttype.h
    fieldparam.h
    header.h
    libconfig.h
    message.h
    mimeentity.h
    mimeentitylist.h
    mimetic.h
    mimeversion.h
    streambufs.h
    strutils.h
    tokenizer.h
    tree.h
    utils.h
    version.h
    rfc822/fieldvalue.h
    rfc822/field.h
    rfc822/datetime.h
    rfc822/mailboxlist.h
    rfc822/body.h
    rfc822/mailbox.h
    rfc822/messageid.h
    rfc822/header.h
    rfc822/address.h
    rfc822/rfc822.h
    rfc822/addresslist.h
    rfc822/group.h
    rfc822/message.h
    codec/codec_chain.h
    codec/codec.h
    codec/base64.h
    codec/codec_base.h
    codec/qp.h
    codec/other_codecs.h
    codec/code.h
    parser/itparserdecl.h
    parser/itparser.h
    os/os.h
    os/file_iterator.h
    os/fileop.h
    os/file.h
    os/stdfile.h
    os/directory.h
    os/utils.h
)
if (NOT WIN32)
    list(APPEND MIMETIC_H_FILES os/mmfile.h)
endif()

add_library(mimetic ${MIMETIC_CPP_FILES})

install(TARGETS mimetic DESTINATION lib)
foreach (file ${MIMETIC_H_FILES})
    get_filename_component(dir ${file} DIRECTORY)
    install(FILES ${file} DESTINATION include/mimetic/${dir})
endforeach()
