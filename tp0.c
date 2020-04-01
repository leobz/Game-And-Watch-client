/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"
#include "utils.h"

int main(void) {
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	log_info(logger, "Soy un log");

	config = leer_config();
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	log_info(logger, "Configuracion > PUERTO:%s - IP:%s", puerto, ip);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// Crear conexion.
	conexion = crear_conexion(ip, puerto);

	// Enviar mensaje.
	enviar_mensaje("dattebayo!", conexion);

	// Recibir mensaje.
	char* mensaje_recibido = recibir_mensaje(conexion);

	// Loguear mensaje recibido
	log_info(logger, "Mensaje recibido: %s", mensaje_recibido);

	free(mensaje_recibido);
	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void) {
	return log_create("tp0.log", "tp0", 1, LOG_LEVEL_INFO);
}

t_config* leer_config(void) {
	return config_create("tp0.config");
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
