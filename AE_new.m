
function AE = AE_new(name, insize, hidsize, outsize)
	clear AE;
	AE.name = name;

	# compresses the data from R^insize to R^hidsize to R^outsize
	# and uncompresses the data from R^outsize to R^hidsize to R^insize
	AE.sizes = [insize, hidsize, outsize, hidsize, insize];
	AE.epsilon = .01;
	AE.maxiter = 1000;
	AE.learning_rate = 0.2;
	AE.momentum = 0.8;

	# weights initialized randomly in [-1,1]
	AE.weights = {rand(AE.sizes(1) + 1, AE.sizes(2)) * 2 - 1, rand(AE.sizes(2) + 1, AE.sizes(3)) * 2 - 1, rand(AE.sizes(3) + 1, AE.sizes(4)) * 2 - 1, rand(AE.sizes(4) + 1, AE.sizes(5)) * 2 - 1};
	AE.training_sets = zeros(0, insize);
	AE_save(AE);
end

