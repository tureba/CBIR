
# autoretrain==0 => don't retrain o each training set. instead, you must
#	call AE_retrain after feeding all the training sets.
function AE = AE_add_training(AE, new_training_set, autoretrain = 1)
	AE.training_sets = [AE.training_sets;new_training_set];
	if (autoretrain)
		AE = AE_retrain(AE);
	end
	AE_save(AE);
end

