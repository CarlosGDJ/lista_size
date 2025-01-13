#!/bin/bash

# Archivo de salida del programa lista_size
OUTPUT_FILE="/tmp/lista_sz"

# Ejecutar el programa lista_size
./lista_size

# Verificar si el programa se ejecutó correctamente
if [ $? -ne 0 ]; then
    echo "Error al ejecutar el programa lista_size."
    exit 1
fi

# Verificar si el archivo de salida existe
if [ ! -f "$OUTPUT_FILE" ]; then
    echo "El archivo $OUTPUT_FILE no fue generado."
    exit 1
fi

# Calcular el espacio total ocupado por los archivos
total_space=$(awk '{sum += $2} END {print sum}' "$OUTPUT_FILE")

# Encontrar el archivo más grande
largest_file=$(awk 'BEGIN {max = 0} {if ($2 > max) {max = $2; file = $1}} END {print file, max}' "$OUTPUT_FILE")

# Encontrar el archivo más pequeño
smallest_file=$(awk 'BEGIN {min = 999999999} {if ($2 < min) {min = $2; file = $1}} END {if (min != 999999999) print file, min}' "$OUTPUT_FILE")

# Mostrar resultados
echo "Espacio total ocupado por el directorio actual: $total_space bytes"
echo "Fichero más grande: $largest_file"

# Comprobar si se encontró un archivo más pequeño
if [ -z "$smallest_file" ]; then
    echo "Fichero más pequeño: No hay archivos válidos en el directorio."
else
    echo "Fichero más pequeño: $smallest_file"
fi
