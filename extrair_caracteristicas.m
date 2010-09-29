#!/usr/bin/octave -qf

# programa principal de extração das características

db = fopen('imagens.txt','w');

arg_list = argv ();
# para cada imagem fornecida pela linha de comando
for i = 1:nargin
	#imprime o nome do arquivo e a classe da imagem
	arq = arg_list{i};
	inicio = rindex(arq, '/') + 1;
	fim = rindex(arq, '_') - 1;
	classe = arq(inicio:fim);
	fprintf(db, '%d %s %s\n', i, arq, classe);

	#carrega a imagem como matriz de números inteiros
	Imagem = uint8(imread(arq));

	fd = fopen([arq, '.histograma'],'w');
	# gera o histograma da imagem e imprime ele
	for x = histograma(Imagem)
		fprintf(fd, '%f ', x);
	end
	fclose(fd);

	fd = fopen([arq, '.matrix_co_ocorrencia'],'w');
	# gera a matriz de co-ocorrencia da imagem e imprime ela
	for M = matriz_co_ocorrencia(Imagem)
		for L = M
			for v = L
				fprintf(fd, '%f ', v);
			end
		end
	end
	fclose(fd);

	# separador de vetores de características da imagem
end

fclose(db);
