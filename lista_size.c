#include <stdio.h>      // Para snprintf
#include <stdlib.h>     // Para exit
#include <fcntl.h>      // Para open
#include <unistd.h>     // Para write, read, close
#include <dirent.h>     // Para opendir, readdir, closedir
#include <sys/stat.h>   // Para stat
#include <string.h>     // Para strlen
#include <errno.h>      // Para manejo de errores

#define OUTPUT_FILE "/tmp/lista_sz"

void handle_error(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(EXIT_FAILURE);
}

int main() {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    int output_fd;

    // Abrir el directorio actual
    dir = opendir(".");
    if (dir == NULL) {
        handle_error("Error al abrir el directorio actual");
    }

    // Abrir el archivo de salida
    output_fd = open(OUTPUT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        closedir(dir);
        handle_error("Error al abrir el archivo de salida");
    }

    // Leer las entradas del directorio
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Obtener información del fichero
        if (stat(entry->d_name, &file_stat) == -1) {
            write(STDERR_FILENO, "Error al obtener información del fichero\n", strlen("Error al obtener información del fichero\n"));
            continue;
        }

        // Crear el string con el nombre del archivo y su tamaño
        char buffer[512];
        int len = snprintf(buffer, sizeof(buffer), "%s %ld\n", entry->d_name, file_stat.st_size);

        // Escribir en el archivo de salida
        if (write(output_fd, buffer, len) == -1) {
            write(STDERR_FILENO, "Error al escribir en el archivo de salida\n", strlen("Error al escribir en el archivo de salida\n"));
            break;
        }
    }

    // Cerrar el directorio y el archivo
    if (closedir(dir) == -1) {
        handle_error("Error al cerrar el directorio");
    }

    if (close(output_fd) == -1) {
        handle_error("Error al cerrar el archivo de salida");
    }

    return EXIT_SUCCESS;
}
