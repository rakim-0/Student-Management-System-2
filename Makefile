CC = gcc
OPTIONS = -Wall -Wextra -Werror

all: generate_students kernel request_generator_admin request_generator_manager request_generator_member server

clear: 
	rm disk generate_students kernel server request_generator_admin requests.txt log.txt requests

generate_students: generate_students.c
	$(CC) -Wall -g generate_students.c -o generate_students

kernel:
	$(CC) -Wall -g kernel.c -o kernel

request_generator_admin: request_generator_admin.c
	$(CC) -Wall -g request_generator_admin.c -o request_generator_admin

request_generator_manager: request_generator_manager.c
	$(CC) -Wall -g request_generator_manager.c -o request_generator_manager

request_generator_member: request_generator_member.c
	$(CC) -Wall -g request_generator_member.c -o request_generator_member

server: server.c
	$(CC) -Wall -g server.c -o server
# build:
# 	@$(CC) -Wall -g $(MODULE).c -o bin/$(MODULE)
# 	@./bin/$(MODULE)

