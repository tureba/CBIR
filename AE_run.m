
function result = AE_run(AE, inputvalues)
	result = inputvalues;
	for layer = 1:size(AE.weights)(2)
		result = tanh([1, result] * AE.weights{layer});
	end
end

