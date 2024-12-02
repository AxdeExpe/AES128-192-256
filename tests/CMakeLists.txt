set(tests
    numKeyWords
    numRounds
    sboxes
    rc
    keyExpansion
    getRoundKey
    addRoundKey
    subBytes
    shiftRows
    mul2
    mul3
    mixColumns
    encrypt
    invMixColumns
    invSubBytes
    invShiftRows
    decrypt
    encrypt_decrypt
    ecb_encrypt
    ecb_decrypt
    cbc_encrypt
    cbc_decrypt
    aes192
    aes256
)

foreach(test ${tests})
    add_executable(${test} ${test}.c)
    target_include_directories(${test} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(${test} aes)
    add_test(NAME ${test} COMMAND ${test})
endforeach()