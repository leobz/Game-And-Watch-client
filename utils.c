/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

//TODO
/*
 * Recibe un paquete a serializar, y un puntero a un int en el que dejar
 * el tamaño del stream de bytes serializados que devuelve
 */
void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	void* to_send = malloc(*bytes);
	int setoff = 0;

	memcpy(to_send, &(paquete->codigo_operacion) , sizeof(int));
	setoff += sizeof(int);
	memcpy(to_send + setoff, &(paquete->buffer->size) , sizeof(int));
	setoff += sizeof(int);
	memcpy(to_send + setoff, paquete->buffer->stream, paquete->buffer->size);
	return to_send;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

//TODO
void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete *package = (t_paquete*) malloc(sizeof(t_paquete));
	t_buffer  *buffer  = (t_buffer*) malloc(sizeof(t_buffer));
	char      *payload = strdup(mensaje);

	//Initialize buffer
	buffer->size = strlen(payload) + 1;
	void *stream = malloc(buffer->size);

	memcpy(stream, payload, strlen(payload) + 1);
	buffer->stream = stream;
	free(payload);

	//Initialize package
	package->codigo_operacion = MENSAJE;
	package->buffer = buffer;

	void* to_send = serializar_paquete(package, &buffer->size);
	send(socket_cliente, to_send, sizeof(int) * 2 + buffer->size, 0);

	free(to_send);
	free(package->buffer->stream);
	free(package->buffer);
	free(package);
}

//TODO
char* recibir_mensaje(int socket_cliente)
{

}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
