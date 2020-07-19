#pragma once
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netdb.h>

#include "../Mensajes/Mensajes.h"
#include "../Mensajes/Juego.h"
int init_cliente(char* IP, int port);