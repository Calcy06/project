cmake_minimum_required(VERSION 3.5)
project(uci_usock_uloop C)

include_directories(usock stdin read)

add_executable(com
	main.c
	usock/usock.c
	stdin/stdin.c
	read/read.c
)

target_link_libraries(com PRIVATE ubox uci)