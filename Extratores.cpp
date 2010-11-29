#include "Extratores.h"

QVector<float> extrairHistograma(QImage imagem)
{
	QVector<float> valores(256, 0);

	for (int i = 0; i < imagem.width(); i++)
		for (int j = 0; j < imagem.height(); j++)
			valores[qGray(imagem.pixel(i,j))]++;

	return valores;

}

QVector<float> extrairMatrizCoOcorrencia(QImage imagem)
{
	QVector<float> valores1(256 * 256, 0);
	QVector<float> valores2(256 * 256, 0);
	QVector<float> valores3(256 * 256, 0);
	QVector<float> valores4(256 * 256, 0);

	for (int i = 1; i < imagem.width() - 1; i++) {
		for (int j = 1; j < imagem.height() - 1; j++) {
			valores1[qGray(imagem.pixel(i,j)) + qGray(imagem.pixel(i+1,j)) * 256]++;
			valores2[qGray(imagem.pixel(i,j)) + qGray(imagem.pixel(i+1,j+1)) * 256]++;
			valores3[qGray(imagem.pixel(i,j)) + qGray(imagem.pixel(i,j+1)) * 256]++;
			valores4[qGray(imagem.pixel(i,j)) + qGray(imagem.pixel(i-1,j+1)) * 256]++;
		}
	}

	QVector<float> resultado(valores1 + valores2 + valores3 + valores4);
	for (int i = 0; i < resultado.size(); i++)
		resultado[i] /= ((imagem.width() - 2) * (imagem.height() - 2));
	return resultado;
}