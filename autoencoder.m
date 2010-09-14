#!/usr/bin/octave -qf

# Rede Neural Auto Codificadora (Auto-Encoder Neural Network)

# Descrição superficial:
# http://en.wikipedia.org/wiki/Autoencoder

# Artigo demonstrando como ela pode ser usada para dimensão de dimensionalidade:
# http://www.sciencemag.org/cgi/content/abstract/313/5786/504

# TODO:
# - Expandir para muitas camadas intermediárias (so está usando 1 agora);
# - Implementar o back propagation;
# - Generalizar para mais funções de passo (está usando a Sigmóide Logística);
# - Repensar os valores de erro e número máximo de iterações do treinamento (talvez algo estabelecido dinamicamente como um recuo exponencial ou logarítmico);
# - Pré-treinamento com uma Máquina de Boltzman Restrita;
# - Colocar tudo em Inglês _ou_ em Português.

