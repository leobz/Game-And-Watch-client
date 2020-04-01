/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

void* serializar_paquete(t_paquete* paquete, int *bytes) {
	void* a_enviar = malloc(*bytes);
	int setoff = 0;

	memcpy(a_enviar, &(paquete->codigo_operacion), sizeof(int));
	setoff += sizeof(int);
	memcpy(a_enviar + setoff, &(paquete->buffer->size), sizeof(int));
	setoff += sizeof(int);
	memcpy(a_enviar + setoff, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

int crear_conexion(char *ip, char* puerto) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,
			server_info->ai_socktype, server_info->ai_protocol);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen)
			== -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

t_paquete* crear_paquete(char* mensaje) {
	char *payload = strdup(mensaje);

	t_paquete *paquete = (t_paquete*) malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = (t_buffer*) malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(payload) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);

	memcpy(paquete->buffer->stream, payload, paquete->buffer->size);
	free(payload);

	return paquete;
}

void enviar_mensaje(char* mensaje, int socket_cliente) {
	t_paquete *paquete = crear_paquete(mensaje);

	int bytes_a_enviar = paquete->buffer->size + sizeof(int) * 2;
	void* a_enviar = serializar_paquete(paquete, &bytes_a_enviar);

	send(socket_cliente, a_enviar, sizeof(int) * 2 + paquete->buffer->size, 0);

	free(a_enviar);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

char* recibir_mensaje(int socket_cliente) {
	int codigo_operacion, size;
	void* stream;

	recv(socket_cliente, &codigo_operacion, sizeof(int), 0);

	if (codigo_operacion == 1) {
		recv(socket_cliente, &size, sizeof(int), 0);
		stream = malloc(size);
		recv(socket_cliente, stream, size, 0);
	}

	return stream;
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}
