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
