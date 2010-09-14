
function AE = AE_retrain(AE, online = 1)
	if AE.momentum > 0
		last_dW = AE.weights;
		for layer = 1:size(AE.weights)(2)
			last_dW{layer} *= 0;
		end
	end
	i = 0;
	while ((AE_get_error(AE) > AE.epsilon) && (i < AE.maxiter))
		# escolhe aleatoriamente um conjunto de treinamento
		tsindex = floor(unifrnd(1, size(AE.training_sets)(1) + 1));
		# separa o conjunto de treinamento
		training_set = AE.training_sets(tsindex,:);

		result = training_set;
		for layer = 1:size(AE.weights)(2)
			outputs{layer} = result = tanh([1, result] * AE.weights{layer});
		end

		del = tanh_derivative(result) .* (result - training_set);
		for layer = size(AE.weights)(2):-1:2
			dW{layer} = - AE.learning_rate * [1, outputs{layer-1}]' * del;
			del = tanh_derivative(outputs{layer-1}) .* (del * AE.weights{layer-1});
		end
		dW{layer} = - AE.learning_rate * [1, outputs{layer-1}]' * del;

		if AE.momentum > 0
			for layer = 1:size(AE.weights)(2)
				AE.weights{layer} += dW{layer} + AE.momentum .* last_dW{layer};
			end
			last_dW = dW;
		else
			for layer = 1:size(AE.weights)(2)
				AE.weights{layer} += dW{layer};
			end
		end

		i++;
	end
	printf('treinamento de %s: %d iterações, %f de erro final\n', AE.name, i, AE_get_error(AE));
		
end

