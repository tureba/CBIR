#!/bin/bash

function exec_extr() {
	extrator=$1
	shift
	./${extrator}.m "$@" >imagens.${extrator}.txt
}

echo "Iniciando a extração de características de ${#*} imagens:"
echo 

echo "Extraindo as matrizes de co-ocorrência..."
time exec_extr matriz_co_ocorrencia "$@"
echo

echo "Extraindo os histogramas..."
time exec_extr histograma "$@"
echo

echo "Fim da extração das imagens"
