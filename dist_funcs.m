#!/usr/bin/octave -qf

# Documentação do Octave:
# http://www.gnu.org/software/octave/doc/interpreter/
# Tutorial de Octave em:
# http://en.wikibooks.org/wiki/Octave_Programming_Tutorial/Getting_started
# Exemplos de Matlab/Octave em:
# http://www.csse.uwa.edu.au/~pk/Research/MatlabFns/
# Distâncias comparadas em:
# www.comp.rgu.ac.uk/staff/ds/papers/25-Liu-etal-AIRS2008.pdf


function d = distancia_minkowski(u, v, p = 2)
	# de acordo com a documentação, norm(a, p) é igual a: (sum (abs (a) .^ p)) ^ (1/p)
	d = norm(u-v,p);
end


function d = distancia_itakura_saito(u, v)
	d = sum((u ./ v) - log(u ./ v) - 1);
end


function d = divergencia_kullback_leibler(u, v)
	d = sum(u .* log(u ./ v));
end


function d = dissimilaridade_cosseno(u,v)
	d = dot(u,v)/(norm(u,"fro") + norm(v,"fro"));
end


