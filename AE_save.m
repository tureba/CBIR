
function AE_save(AE)
	save (strcat('~/.redesneurais/', AE.name, '.AE'), '-struct', 'AE');
end

