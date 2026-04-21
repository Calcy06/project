cmake_minimum_required(VERSION 3.5)
project(student_uci C)
include_directories(/root/test/uci)


add_executable(student_write student_write.c)
target_link_libraries(student_write PRIVATE uci)

add_executable(main main.c student_timer.c student_list.c)
target_link_libraries(main PRIVATE ubox uci)