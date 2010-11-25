#!/usr/bin/octave -qf

while 1
	resp = input('Digite o caminho da imagem a usar: ', 's');

	if (size(resp) == [0,0])
		quit();
	end

	Imagem = imread(resp);

	mostre = Imagem;
	imshow(mostre);

	VC = [];
	dados = [];
	usa_hist = yes_or_no('Buscar usando o histograma? ');
	if (usa_hist == 1)
		printf('Calculando o histograma de %s...', resp)
		VC = [VC,histograma(Imagem)];
		dados = dlmread('histograma.txt', ' ')(:,2:257);
		printf('OK\n');
	end

	usa_matco = yes_or_no('Buscar usando a matriz de co-ocorrencia? ');
	if (usa_matco == 1)
		printf('Calculando a matriz de co-ocorrencia de %s...', resp)
		VC = [VC,matriz_co_ocorrencia(Imagem)];
		dados = [dados,dlmread('matriz_co_ocorrencia.txt', ' ')(:,2:257)];
		printf('OK\n');
	end

	if (size(VC) == [0,0])
		printf('Ao menos um vetor de características precisa ser usado.\n');
		continue;
	end

	dist = menu('Escolha a funcao de distancia: ', 'Distancia de Minkowski', 'Distancia de Itakura-Saito', 'Divergencia de Kullback-Leibler', ' Dissimilaridade dos Cossenos');

	if ((tipo = menu('Tipo de busca: ', 'N-mais proximos', 'Vizinhos num raio R')) == 1)
		num_viz = input('Número de vizinhos mais próximos: ', 's');
	else
		raio = input('Raio da distância: ', 's');
	end

	tam_bd = max(size(histogramas)(1),size(co_ocorrencias)(1));
	printf('Buscando...\n');
	resultado = [];
	for i = 1:tam_bd
		resultado = [resultado;[i,dist(VC,dados(i,:))]];
	end

	if (tipo == 1)
		for i = 1:num_viz
			mostre = [mostre;zeros(5,512);resultado{i}];
		end
	else
		for i = 1:tam_bd
			if (resultado(i,2) < raio)
				mostre = [mostre;zeros(5,512);resultado{i}];
			end
		end
	end

	imshow(mostre);

	input('','s');
end
