
function AE = AE_load(name)
	clear AE;
	load(strcat('~/.redesneurais/', name, '.AE'), 'sizes', 'epsilon', 'maxiter', 'weights', 'learning_rate', 'momentum', 'training_sets');
	AE.name = name;
	AE.sizes = sizes;
	AE.epsilon = epsilon;
	AE.maxiter = maxiter;
	AE.learning_rate = learning_rate;
	AE.momentum = momentum;
	AE.weights = weights;
	AE.training_sets = training_sets;
end

