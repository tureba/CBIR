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
	tmp = [];
	for x = resultado
		for y = x
			tmp = [tmp,y];
		end
	end
	resultado = double(tmp) / double((linhas-2) * (colunas-2));
end
