#include "Distancias.h"

#include <algorithm>
#include <cmath>

float distMinkowski(QVector<float> a, QVector<float> b)
{
	float resultado = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++)
		resultado += std::pow(std::abs(a[i] - b[i]), 2);

	return std::sqrt(resultado);
}

float distItakuraSaito(QVector<float> a, QVector<float> b)
{
	float resultado = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++)
		if ((a[i] != .0f) && (b[i] != .0f))
			resultado += (a[i] / b[i]) - std::log(a[i] / b[i]) - 1;

	return resultado;
}

float distKullbackLeibner(QVector<float> a, QVector<float> b)
{
	float resultado = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++)
		if ((a[i] != .0f) && (b[i] != .0f))
			resultado += a[i] * std::log(a[i] / b[i]);

	return resultado;
}

float distCosseno(QVector<float> a, QVector<float> b)
{
	float dot = 0, u = 0, v = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++) {
		dot += a[i] * b[i];
		u += a[i] * a[i];
		v += b[i] * b[i];
	}

	return dot/(std::sqrt(u) + std::sqrt(v));
}