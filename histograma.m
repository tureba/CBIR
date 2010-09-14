#!/usr/bin/octave -qf

function resultado = histograma(imagem)
	resultado = zeros(1,256);
	[linhas,colunas] = size(imagem);
	for x = 1:linhas
		for y = 1:colunas
			resultado(imagem(x,y) + 1) += 1;
		end
	end
	resultado = double(resultado) / double(max(resultado));
	return;
end

# programa principal de extração do histograma

arg_list = argv ();
# para cada imagem fornecida pela linha de comando
for i = 1:nargin
	#imprime o nome do arquivo e a classe da imagem
	arq = arg_list{i};
	inicio = rindex(arq, '/') + 1;
	fim = rindex(arq, '_') - 1;
	classe = arq(inicio:fim);
	printf('%s %s ', arq, classe);

	#carrega a imagem como matriz de números inteiros
	Imagem = uint8(imread(arq));

	# gera o histograma da imagem e imprime ele
	for x = histograma(Imagem);
		printf('%f ', x);
	end

	# separador de vetores de características da imagem
	printf('\n');
end
