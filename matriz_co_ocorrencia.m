#!/usr/bin/octave -qf

#calculando para uma distância e quatro direções
function resultado = matriz_co_ocorrencia(imagem)
	resultado = zeros(4,256,256);
	[linhas,colunas] = size(imagem);
	for x = 2:linhas-1
		for y = 2:colunas-1
			# angulo 0º, distancia 1
			resultado(1, imagem(x,y) + 1, imagem(x+1,y) + 1) += 1;
			# angulo 45º, distancia 1
			resultado(2, imagem(x,y) + 1, imagem(x+1,y+1) + 1) += 1;
			# angulo 90º, distancia 1
			resultado(3, imagem(x,y) + 1, imagem(x,y+1) + 1) += 1;
			# angulo 135º, distancia 1
			resultado(4, imagem(x,y) + 1, imagem(x-1,y+1) + 1) += 1;
		end
	end
	resultado = double(resultado) / double((linhas-2) * (colunas-2));
end

# programa principal de extração da matriz de co-ocorrência

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

	# gera a matriz de co-ocorrencia da imagem e imprime ela
	for M = matriz_co_ocorrencia(Imagem)
		for L = M
			for v = L
				printf('%f ', v);
			end
		end
	end

	# separador de vetores de características da imagem
	printf('\n');
end
