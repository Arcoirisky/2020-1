#include "FunServidor.h"

int* server_init(char* IP, int port){
    // Se define la estructura para almacenar info del socket del servidor al momento de su creación
    struct sockaddr_in server_addr;

    // Se solicita un socket al SO, que se usará para escuchar conexiones entrantes
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);

    // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
    int opt = 1;
    int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    // Se guardan el puerto e IP en la estructura antes definida
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_aton(IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);

    // Se le asigna al socket del servidor un puerto y una IP donde escuchar
    int ret2 = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Se coloca el socket en modo listening
    // maximo de 2 conexiones
    if (listen(server_socket, 2) == 0){
        printf("Esperando jugadores...\n");
    }
    else{
        printf("Error en el listen :c");
    }

    // Se definen las estructuras para almacenar info sobre los sockets de los clientes
    struct sockaddr_storage serverStorage;
    socklen_t addr_size = sizeof serverStorage;
    

    int Socket1 = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);
   
    /* 
    Aqui deberia enviarse el mensaje de que se conecto el jugador 1...
    */
    char* msg_in = recivirMensaje(Socket1);
	char* msg_out = responderAccion(msg_in, 0);

	enviarMensaje(Socket1, msg_out);
    enviarMensaje(Socket1, paquete_init(GiveId, "1"));

	free(msg_in);
    

    printf("\n--> Jugador 1 conectado <--\n");

    // maximo de 2 conexiones again
    if (listen(server_socket, 2) == 0){
        printf("Esperando jugadores...\n");
    }
    else{
        printf("Error en el listen :c");
    }

    addr_size = sizeof serverStorage;
    int Socket2 = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);
    
    /* 
    Aqui deberia enviarse el mensaje de que se conecto el jugador 2...
    */
    msg_in = recivirMensaje(Socket2);
	msg_out = responderAccion(msg_in, 0);

	enviarMensaje(Socket2, msg_out);
    enviarMensaje(Socket2, paquete_init(GiveId, "2"));
	free(msg_in);

    printf("--> Jugador 2 conectado <--\n");

    static int sockets[2];
	sockets[0] = Socket1;
	sockets[1] = Socket2;

	return sockets;
}

void terminate_connection(int socket){
    close(socket);
}

Partida* inicializarPrograma(char* ip, int port){
    printf("Inicializando servidor...\n");
    int* sockets = server_init(ip, port);
    printf("... ¡Servidor inicializado! ...\n");

    char msg[] = " ";
    enviarMensaje(sockets[0], paquete_init(AskNickname, msg));
    enviarMensaje(sockets[1], paquete_init(AskNickname, msg));
    char* msg_in = recivirMensaje(sockets[0]);
    char* nickname0 = responderAccion(msg_in, 0);
    Jugador* Player1 = player_init(sockets[0], nickname0);
    enviarMensaje(sockets[0], paquete_init(WelcomeName, nickname0));
    
    free(msg_in);

    Jugador* Player2;
    char* nickname1;
    while(true){
        char* msg_in = recivirMensaje(sockets[1]);
        nickname1 = responderAccion(msg_in, 0);
        if(strcmp(nickname0, nickname1) != 0){
        free(msg_in);
        Player2 = player_init(sockets[1], nickname1);
        enviarMensaje(sockets[1], paquete_init(WelcomeName, nickname1));
    
        free(nickname0);
        free(nickname1);
        break;
        }
        else{
        free(msg_in);
        free(nickname1);
        enviarMensaje(sockets[1], paquete_init(AskNickname, msg));
        }
    }
    // CARGA DEL TABLERO EN EL SERVIDOR CON RUTA ENTREGADA POR JUGADOR 1 //
    char msgBoard[] = " ";
    enviarMensaje(sockets[0], paquete_init(AskBoard, msgBoard));
    char* boardMsg_in = recivirMensaje(sockets[0]);
    char* boardName = responderAccion(boardMsg_in, 0); // "tableros/ejemplo1.txt"
    Tablero* tablero_juego = tablero_init(boardName);
    // Enviar ubicacion del tablero a jugadores
    enviarMensaje(sockets[0], paquete_init(RetBoard, boardName));
    enviarMensaje(sockets[1], paquete_init(RetBoard, boardName));
    //Nombres
    enviarMensaje(Player1->socket, paquete_init(GiveEnemyName, Player2->nombre));
    enviarMensaje(Player2->socket, paquete_init(GiveEnemyName, Player1->nombre));
    enviarMensaje(Player1->socket, paquete_init(GivePlayerName, Player1->nombre));
    enviarMensaje(Player2->socket, paquete_init(GivePlayerName, Player2->nombre));

    free(boardMsg_in);
    free(boardName);
    ////////////////

    Partida* partida = partida_init(tablero_juego, Player1, Player2);
    return partida;  
}

void mostrarTablerosJugadores(Partida* partida){
    enviarMensaje(partida ->Player1->socket, paquete_init(ShowBoard, " "));
    enviarMensaje(partida ->Player2->socket, paquete_init(ShowBoard, " "));
}

Jugador* sacarPlayer(Partida* partida, int jugador, bool enemy){
    if (jugador == 1){
        if(enemy){
            return partida->Player2;
        }
        return partida->Player1;
    }
    else{
        if(enemy){
            return partida->Player1;
        }
        return partida->Player2;
    }
}

void startturn(Partida* partida, int jugador){
    Jugador* player = sacarPlayer(partida, jugador, false);
    Jugador* enemy = sacarPlayer(partida, jugador, true);
    // Se muestra el tablero actualizado
    

    // Menu con todas las opciones
    /* 
    1) Utilizar un poder
    2) Rendirse
    3) Avanzar
    */
    if(player->Yadran){
        enviarMensaje(enemy->socket, paquete_init(Wait, " "));
        enviarMensaje(player->socket, paquete_init(CryYadranCry, " "));
        player ->Yadran = false;
        return;
    }
    enviarMensaje(enemy->socket, paquete_init(Wait, " "));
    enviarMensaje(player->socket, paquete_init(AskOption, " "));
    char* msg_in = recivirMensaje(player->socket);
    char* opcion = responderAccion(msg_in, 0);
    free(msg_in);
    printf("opcion: %i\n", atoi(opcion));
    switch(atoi(opcion))
    {
        case 1:
            // Eligio utilizar un poder
            free(opcion);
            escogerPoder(partida, jugador);
            mostrarTablerosJugadores(partida);
            return;
            break;
        case 2:
            // Eligio Rendirse
            free(opcion);
            if(jugador == 1){
                anunciarGanador(partida, 2);
            }
            else{
                anunciarGanador(partida, 1);
            }
            exit(0);
            break;
        case 3:
            // Eligio avanzar con lanzamiento de dados normales
            free(opcion);
            avanzar(partida, jugador);
            mostrarTablerosJugadores(partida);
            return;
            break;
        default:
            free(opcion);
            printf("Opcion invalida, no hay perdon porque no lo controlo\n");
            return;
    }
    // Esto solo si la opcion es 3
    // La casilla en donde termina el jugador despues de moverse se activa

    //Si el juego no termina entonces comienza el nuevo turno
}

void actualizarPosJug(Partida* partida, int jugador, int pos){
    Jugador* player = sacarPlayer(partida, jugador, false);
    Jugador* enemy = sacarPlayer(partida, jugador, true);

    char sint[100];
    sprintf(sint, "%i", pos);

    if (jugador==1){
        partida ->Player1->posicion = pos;
    }
    else
    {
        partida ->Player2->posicion = pos;
    }

    enviarMensaje(player->socket, paquete_init(MovePlayer, sint));
    enviarMensaje(enemy->socket, paquete_init(MoveEnemy, sint));
}

int actualizarMerJug(Partida* partida, int jugador, int cant){
    Jugador* player = sacarPlayer(partida, jugador, false);
    Jugador* enemy = sacarPlayer(partida, jugador, true);

    if (jugador==1){
        if(partida ->Player1->meritos + cant < 0){
            cant = -partida ->Player1->meritos;
            partida ->Player1->meritos = 0;
        }
        else{
            partida ->Player1->meritos += cant;
        }
    }
    else{
        printf("tenemos %i y c cant %i\n",partida ->Player2->meritos,partida ->Player2->meritos + cant);
        if(partida ->Player2->meritos + cant < 0){
            cant = -partida ->Player2->meritos;
            partida ->Player2->meritos = 0;
        }
        else{
            partida ->Player2->meritos += cant;
        }
    }
    
    if (cant != 0){
        char sint[100];
        sprintf(sint, "%i", cant);

        enviarMensaje(player->socket, paquete_init(MoreMerPlayer, sint));
        enviarMensaje(enemy->socket, paquete_init(MoreMerEnemy, sint));
    }
    return cant;
}

void avanzar(Partida* partida, int jugador){
    Jugador* player = sacarPlayer(partida, jugador, false);
    enviarMensaje(player->socket, paquete_init(AskMovType, " "));
    char* msg_in = recivirMensaje(player->socket);
    char* opcion = responderAccion(msg_in, 0);
    free(msg_in);
    printf("opcion: %i\n", atoi(opcion));
    int movimiento;
    if(atoi(opcion) == 1){
        movimiento = rollSinleDice();
    }
    else{
        enviarMensaje(player->socket, paquete_init(AskMovNum, " "));
        char* msg_in2 = recivirMensaje(player->socket);
        char* cant = responderAccion(msg_in2, 0);
        movimiento = atoi(cant);
        free(msg_in2);
        free(cant);
    }
    free(opcion);

    div_t posicion_final = div(player->posicion + movimiento, partida->board->largo);
    
    player->posicion = posicion_final.rem;
    
    if(posicion_final.quot > 0){
        printf("+20 meritos para el jugador %i\n", jugador);
        actualizarMerJug(partida,jugador,20);
    }

    actualizarPosJug(partida, jugador, posicion_final.rem);
    
    Casilla* box = partida->board->boxs[posicion_final.rem];

    ejecutarCasilla(box->tipo, partida, jugador);
    if(partida->Player1->posicion == partida->Player2->posicion && box->tipo == D){
        ejecutarCasilla(box->tipo, partida, jugador);
    } // este if es cuando despues de una movida ambos jugadores estan en la misma casilla de duelo (hay 2 duelos en total)
    show_board(partida);
    
    somebodyWin(partida);
}
