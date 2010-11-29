#ifndef BD_H
#define BD_H

#include <QImage>
#include <QSet>
#include <QString>

#include "Extratores.h"
#include "Distancias.h"

class BD
{
private:
	QString nome, dir_base;

	typedef QVector<float> (* FcExtracaoDeCaracteristicas)(QImage);
	typedef float (* FcDistancia)(QVector<float>, QVector<float>);

	bool usaHistograma;
	bool usaMatrizCoOcorrencia;

	FcDistancia fDist;

	QSet<QString> imagens;
public:
	BD(QString nomeBD);

	void alteraFuncaoDistancia(QString f);
	void alteraFuncaoDistancia(FcDistancia f);

	void adicionaFuncaoExtracao(QString f);

	bool insereImagem(QString hash);
	bool removeImagem(QString hash);

	QVector<float> buscaVC(QString hash);

	void calcCaracteristicas(QString hash);

	QString calcHash(QImage imagem) const;
	QString calcHash(QString nomeImagem) const;

	void salvaVet(QVector<float> vet, QString arquivo);
	QVector<float> CarregaVet(QString arquivo);
};

#endif // BD_H
