#ifndef BD_H
#define BD_H

#include <QImage>
#include <QSet>
#include <QString>

class BD
{
private:
	QString nome, dir_base;

	bool usaHistograma;
	bool usaMatrizCoOcorrencia;

	typedef enum {
			fDistMinkowski = 1,
			fDistItakuraSaito = 2,
			fDistKullbackLeibler = 3,
			fDistCosseno = 4,
		} t_fDist;
	t_fDist fDist;

	QSet<QString> imagens;
public:
	BD(QString nomeBD);

	void alteraFuncaoDistancia(QString f);
	void alteraFuncaoDistancia(t_fDist f);

	void adicionaFuncaoExtracao(QString f);

	bool insereImagem(QString hash);
	bool removeImagem(QString hash);

	QVector<float> buscaVC(QString hash);

	void calcCaracteristicas(QString hash);

	QString calcHash(QImage imagem) const;
	QString calcHash(QString nomeImagem) const;

	void salvaVet(QVector<float> vet, QString arquivo);
	QVector<float> CarregaVet(QString arquivo);

	float distMinkowski(QVector<float> a, QVector<float> b);
	float distItakuraSaito(QVector<float> a, QVector<float> b);
	float distKullbackLeibler(QVector<float> a, QVector<float> b);
	float distCosseno(QVector<float> a, QVector<float> b);

	QVector<float> extrairHistograma(QImage imagem);
	QVector<float> extrairMatrizCoOcorrencia(QImage imagem);
};

#endif // BD_H
