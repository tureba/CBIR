#ifndef BD_H
#define BD_H

#include <QtCore/QFile>
#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtGui/QImage>

#include <arboretum/stTypes.h>
#include <arboretum/stMetricEvaluator.h>
#include <arboretum/stUserLayerUtil.h>
#include <arboretum/stUtil.h>

class BD
{
public:
	QFile arq;
	QString dir_base;

	bool usaHistograma;
	bool usaMatrizCoOcorrencia;

	QSet<QString> imagens;
	typedef enum {
			fDistMinkowski = 1,
			fDistItakuraSaito = 2,
			fDistKullbackLeibler = 3,
			fDistCosseno = 4,
		} t_fDist;
	t_fDist fDist;

	typedef float(*BDFcDist)(QVector<float>, QVector<float>);

	BD(QString nomeBD);
	~BD();

	void alteraFuncaoDistancia(QString f);
	void alteraFuncaoDistancia(t_fDist f);

	void adicionaFuncaoExtracao(QString f);
	void removeFuncaoExtracao(QString f);

	bool insereImagem(QString hash);
	bool removeImagem(QString hash);
	QImage retornaImagem(QString hash);

	void calcCaracteristicas(QString hash);

	QString calcHash(QImage imagem) const;
	QString calcHash(QString nomeImagem) const;

	void salvaVet(QVector<float> vet, QString arquivo);
	QVector<float> carregaVet(QString arquivo);
	QVector<float> buscaVC(QString hash);

	static float distMinkowski(QVector<float> a, QVector<float> b);
	static float distItakuraSaito(QVector<float> a, QVector<float> b);
	static float distKullbackLeibler(QVector<float> a, QVector<float> b);
	static float distCosseno(QVector<float> a, QVector<float> b);

	QVector<float> extrairHistograma(QImage imagem);
	QVector<float> extrairMatrizCoOcorrencia(QImage imagem);

	QStringList buscaN(QImage imagem, int n);
	QStringList buscaR(QImage imagem, float r);
};

class HImagem
{

public:
	QString _hash;
	QVector<float> _VC;

	HImagem(QString hash, QVector<float> VC);

	const stByte * Serialize();
	void Unserialize (const stByte *data, stSize datasize);
	stSize GetSerializedSize();
	HImagem * Clone();
	bool IsEqual(HImagem * outra);

};

class HImagemCmpCosseno
{
public:
	static stDistance GetDistance(HImagem *i1, HImagem *i2)
	{
		return BD::distCosseno(i1->_VC, i2->_VC);
	}

	static stDistance GetDistance2(HImagem *i1, HImagem *i2)
	{
		return BD::distCosseno(i1->_VC, i2->_VC);
	}
};

class HImagemCmpMinkowski
{
public:
	static stDistance GetDistance(HImagem *i1, HImagem *i2)
	{
		return BD::distMinkowski(i1->_VC, i2->_VC);
	}

	static stDistance GetDistance2(HImagem *i1, HImagem *i2)
	{
		return BD::distMinkowski(i1->_VC, i2->_VC);
	}
};

class HImagemCmpItakuraSaito
{
public:
	static stDistance GetDistance(HImagem *i1, HImagem *i2)
	{
		return BD::distItakuraSaito(i1->_VC, i2->_VC);
	}

	static stDistance GetDistance2(HImagem *i1, HImagem *i2)
	{
		return BD::distItakuraSaito(i1->_VC, i2->_VC);
	}
};

class HImagemCmpKullbackLeibler
{
public:
	static stDistance GetDistance(HImagem *i1, HImagem *i2)
	{
		return BD::distKullbackLeibler(i1->_VC, i2->_VC);
	}

	static stDistance GetDistance2(HImagem *i1, HImagem *i2)
	{
		return BD::distKullbackLeibler(i1->_VC, i2->_VC);
	}
};



#endif // BD_H
