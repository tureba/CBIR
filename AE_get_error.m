
# retorna a norma do vetor de erro normalizada pelo número de dimensões
function e = AE_get_error(AE)
	e = .0;
	for training_set = AE.training_sets'
		# calcula a "norma de Frobenius": sqrt (sumsq (abs (a)))
		e += norm(training_set' - AE_run(AE, training_set'), "fro");
	end
	# erro médio pelo número de conjuntos de treinamento
	e /= size(AE.training_sets)(1);
end

