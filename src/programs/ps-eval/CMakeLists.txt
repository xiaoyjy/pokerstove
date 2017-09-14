project(eval)

add_executable(ps-eval main.cpp)
add_executable(my-eval my.cpp)
#add_definitions ("-ansi -Wall -std=c++0x")
add_definitions ("-std=c++11 -pthread")

target_link_libraries(ps-eval
        peval
        penum
        ${Boost_LIBRARIES}
        pthread
)

target_link_libraries(my-eval
        peval
        penum
        ${Boost_LIBRARIES}
        pthread
)
