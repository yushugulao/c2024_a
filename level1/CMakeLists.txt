project(level1)

add_executable(p01_running_letter p01_running_letter/main.c
        p01_running_letter/letter.cpp
)

add_executable(p02_is_prime p02_is_prime/main.c)

add_executable(p03_all_primes p03_all_primes/main.cpp)

add_executable(p04_goldbach p04_goldbach/main.c)

add_executable(p05_encrypt_decrypt p05_encrypt_decrypt/main.cpp
        p05_encrypt_decrypt/test.cpp)

add_executable(p06_hanoi p06_hanoi/main.cpp)

add_executable(p07_maze
        p07_maze/main.cpp
        p07_maze/checkMaze.cpp
        p07_maze/main.hpp)

add_executable(p08_push_boxes p08_push_boxes/main.cpp
)

add_executable(p09_linked_list p09_linked_list/main.c)

add_executable(p10_warehouse p10_warehouse/main.c)

add_executable(new_target stack/stack.cpp
        stack/stack.cpp)

add_executable(new_target1
        ReversePolishNotation/ReversePolishNotation.cpp)

add_executable(new_target2 RPN_with_only_one/RPN_with_only_one.cpp)
