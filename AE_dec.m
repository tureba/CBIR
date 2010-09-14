
function result = AE_dec(AE, inputvalues)
	result = inputvalues;
	for layer = size(AE.weights)(2)/2 + 1:size(AE.weights)(2)
		result = tanh([1, result] * AE.weights{layer});
	end
end

