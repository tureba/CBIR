#include "BD.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QVector>
#include <QtGui/QImage>

#include <algorithm>
#include <cmath>
#include <iostream>

#include <arboretum/stTypes.h>
#include <arboretum/stUserLayerUtil.h>
#include <arboretum/stBasicMetricEvaluators.h>
#include <arboretum/stBasicObjects.h>
#include <arboretum/stDummyTree.h>
#include <arboretum/stPlainDiskPageManager.h>
#include <arboretum/stPage.h>


BD::BD(QString nomeBD): arq(nomeBD), dir_base(QFileInfo(nomeBD).path() + "/"), usaHistograma(false), usaMatrizCoOcorrencia(false), fDist(fDistCosseno)
{
	if (!arq.open(QIODevice::ReadWrite | QIODevice::Text)) {
		std::cerr << "BD::BD(" << arq.fileName().toStdString() << "): erro ao abrir o arquivo de BD." << std::endl;
		return;
	}

	QTextStream in(&arq);
	while (!in.atEnd()) {
		//processa linha
		QString linha = in.readLine();
		if (linha[0] == '#')
			//ignora comentários
			continue;
		//se a linha tem uma atribuição
		//então é da forma chave=valor
		if (linha.contains('=')) {
			QStringList l(linha.split('='));
			if (l[0].trimmed() == "fdist")
				alteraFuncaoDistancia(l[1].trimmed());
			else if (l[0].trimmed() == "fext") {
				l = l[1].split(",", QString::SkipEmptyParts);
				for (auto it = l.begin(); it != l.end(); it++)
					adicionaFuncaoExtracao((*it).trimmed());
			}
		} else {
			if (!linha.trimmed().isEmpty())
				imagens.insert(linha.trimmed());
		}
	}

#ifdef DEBUG
	std::cerr << "BD::BD(" << arq.fileName().toStdString() << "):" << std::endl;
	std::cerr << "\tdir_base = " << dir_base.toStdString() << std::endl;
	std::cerr << "\tfdist = " << fDist << std::endl;
	std::cerr << "\tfext(hist) = " << usaHistograma << std::endl;
	std::cerr << "\tfext(matco) = " << usaMatrizCoOcorrencia << std::endl;
	for (auto it = imagens.begin(); it != imagens.end(); it++)
		std::cerr << "\t\t" << (*it).toStdString() << std::endl;
#endif
}

BD::~BD()
{
#ifdef DEBUG
	std::cerr << "BD::~BD() (arq = " << arq.fileName().toStdString() << "):" << std::endl;
	std::cerr << "\tfdist = " << fDist << std::endl;
	std::cerr << "\tfext(hist) = " << usaHistograma << std::endl;
	std::cerr << "\tfext(matco) = " << usaMatrizCoOcorrencia << std::endl;
	for (auto it = imagens.begin(); it != imagens.end(); it++)
		std::cerr << "\t\t" << (*it).toStdString() << std::endl;
#endif

	arq.seek(0);
	arq.resize(0);

	QTextStream saida(&arq);

	saida << "fdist = ";
	switch (fDist) {
	case 1:
		saida << "Minkowski";
		break;
	case 2:
		saida << "ItakuraSaito";
		break;
	case 3:
		saida << "KullbackLeibler";
		break;
	case 4:
		saida << "Cosseno";
		break;
	}
	saida << '\n';

	saida << "fext = ";
	if (usaHistograma)
		saida << "hist";
	if (usaHistograma && usaMatrizCoOcorrencia)
		saida << ",";
	if (usaMatrizCoOcorrencia)
		saida << "matco";
	saida << '\n';

	for (auto it = imagens.begin(); it != imagens.end(); it++)
		saida << *it << '\n';
	saida.flush();
	arq.close();
}

void BD::alteraFuncaoDistancia(QString f)
{
	if (f == "Minkowski")
		fDist = fDistMinkowski;
	else if (f == "ItakuraSaito")
		fDist = fDistItakuraSaito;
	else if (f == "KullbackLeibler")
		fDist = fDistKullbackLeibler;
	else if (f == "Cosseno")
		fDist = fDistCosseno;
	else
		fDist = fDistCosseno;
}

void BD::alteraFuncaoDistancia(t_fDist f)
{
	fDist = f;
}

void BD::adicionaFuncaoExtracao(QString f)
{
	if (f == "hist")
		usaHistograma = true;
	else if (f == "matco")
		usaMatrizCoOcorrencia = true;
}

void BD::removeFuncaoExtracao(QString f)
{
	if (f == "hist")
		usaHistograma = false;
	else if (f == "matco")
		usaMatrizCoOcorrencia = false;
}

bool BD::insereImagem(QString hash)
{
	if (!hash.isEmpty()) {
		bool ret = imagens.contains(hash);
		if (!ret) {
#ifdef DEBUG
			std::cerr << "BD::insereImagem: " << hash.toStdString() << std::endl;
#endif
			imagens.insert(hash);
			calcCaracteristicas(hash);
		}
		return ret;
	} else
		return false;
}

bool BD::removeImagem(QString hash)
{
	bool ret = imagens.contains(hash);
	if (ret)
		imagens.remove(hash);
	return ret;
}

QImage BD::retornaImagem(QString hash)
{
	return QImage(dir_base + hash + ".bmp");
}

void BD::calcCaracteristicas(QString hash)
{
	if ((usaHistograma) && (!QFile::exists(dir_base + hash + ".histograma")))
		salvaVet(extrairHistograma(QImage(dir_base + hash + ".bmp")), dir_base + hash + ".histograma");

	if ((usaMatrizCoOcorrencia) && (!QFile::exists(dir_base + hash + ".matrizcoocorrencia")))
		salvaVet(extrairMatrizCoOcorrencia(QImage(dir_base + hash + ".bmp")), dir_base + hash + ".matrizcoocorrencia");
}

QString BD::calcHash(QImage imagem) const
{
	QByteArray pixels = QByteArray(reinterpret_cast<const char *>(imagem.bits()), imagem.byteCount());
	QString hash(QCryptographicHash::hash(pixels, QCryptographicHash::Sha1).toHex());
	//coloca a imagem no disco endereçando pelo hash
	imagem.save(dir_base + hash + ".bmp");
	return hash;
}

QString BD::calcHash(QString nomeImagem) const
{
	QImage imagem(nomeImagem);
	if (imagem.isNull())
		return QString();
	else
		return calcHash(imagem);
}

void BD::salvaVet(QVector<float> vet, QString arquivo)
{
	QFile _arq(arquivo);
#ifdef DEBUG
	std::cerr << "BD::salvaVet(): salvando " << arquivo.toStdString() << "..." << std::endl;
#endif
	if (!_arq.open(QIODevice::WriteOnly | QIODevice::Text)) {
		std::cerr << "BD::salvaVet(vet, " << arquivo.toStdString() << "): erro ao abrir o arquivo par escrita." << std::endl;
		return;
	}

	QDataStream saida(&_arq);

	saida << vet;
	_arq.close();
}

QVector<float> BD::carregaVet(QString arquivo)
{
	QFile _arq(arquivo);
	if (!_arq.open(QIODevice::ReadOnly | QIODevice::Text))
		return QVector<float>(0);

	QDataStream entrada(&_arq);

	QVector<float> resultado;

	entrada >> resultado;
	_arq.close();

	return resultado;
}

QVector<float> BD::buscaVC(QString hash)
{
	if (usaHistograma && usaMatrizCoOcorrencia)
		return carregaVet(dir_base + hash + ".histograma") + carregaVet(dir_base + hash + ".matrizcoocorrencia");
	else if (usaHistograma)
		return carregaVet(dir_base + hash + ".histograma");
	else if (usaMatrizCoOcorrencia)
		return carregaVet(dir_base + hash + ".matrizcoocorrencia");
	else
		return QVector<float>();
}

float BD::distMinkowski(QVector<float> a, QVector<float> b)
{
	float resultado = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++)
		resultado += std::pow(std::abs(a[i] - b[i]), 2);

	return std::sqrt(resultado);
}

float BD::distItakuraSaito(QVector<float> a, QVector<float> b)
{
	float resultado = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++)
		if ((a[i] != .0f) && (b[i] != .0f))
			resultado += (a[i] / b[i]) - std::log(a[i] / b[i]) - 1;

	return resultado;
}

float BD::distKullbackLeibler(QVector<float> a, QVector<float> b)
{
	float resultado = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++)
		if ((a[i] != .0f) && (b[i] != .0f))
			resultado += a[i] * std::log(a[i] / b[i]);

	return resultado;
}

float BD::distCosseno(QVector<float> a, QVector<float> b)
{
	float dot = 0, u = 0, v = 0;

	for (int i = 0; i < std::min(a.size(), b.size()); i++) {
		dot += a[i] * b[i];
		u += a[i] * a[i];
		v += b[i] * b[i];
	}

	return dot/(std::sqrt(u) + std::sqrt(v));
}

QVector<float> BD::extrairHistograma(QImage imagem)
{
	QVector<float> valores(256, 0);

	for (int i = 0; i < imagem.width(); i++)
		for (int j = 0; j < imagem.height(); j++)
			valores[qGray(imagem.pixel(i,j))]++;

	return valores;

}

QVector<float> BD::extrairMatrizCoOcorrencia(QImage imagem)
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

QStringList BD::buscaN(QImage imagem, int n)
{
	if ((imagem.isNull()) || (n <= 0))
		return QStringList();

	QStringList lista;
	QString hash = calcHash(imagem);
	QVector<float> VC;
	if (usaHistograma)
		VC += extrairHistograma(imagem);
	if (usaMatrizCoOcorrencia)
		VC += extrairMatrizCoOcorrencia(imagem);
	HImagem alvo(hash, VC);
	stResult<HImagem> * resultado;

	switch (fDist) {
	case fDistMinkowski: {
			stDummyTree<HImagem, HImagemCmpMinkowski> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.NearestQuery(&alvo, n);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	case fDistItakuraSaito: {
			stDummyTree<HImagem, HImagemCmpItakuraSaito> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.NearestQuery(&alvo, n);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	case fDistKullbackLeibler: {
			stDummyTree<HImagem, HImagemCmpKullbackLeibler> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.NearestQuery(&alvo, n);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	case fDistCosseno: {
			stDummyTree<HImagem, HImagemCmpCosseno> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.NearestQuery(&alvo, n);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	}

	return lista;
}

QStringList BD::buscaR(QImage imagem, float r)
{
	if ((imagem.isNull()) || (r <= .0))
		return QStringList();

	QStringList lista;
	QString hash = calcHash(imagem);
	QVector<float> VC;
	if (usaHistograma)
		VC += extrairHistograma(imagem);
	if (usaMatrizCoOcorrencia)
		VC += extrairMatrizCoOcorrencia(imagem);
	HImagem alvo(hash, VC);
	stResult<HImagem> * resultado;

	switch (fDist) {
	case fDistMinkowski: {
			stDummyTree<HImagem, HImagemCmpMinkowski> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.RangeQuery(&alvo, r);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	case fDistItakuraSaito: {
			stDummyTree<HImagem, HImagemCmpItakuraSaito> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.RangeQuery(&alvo, r);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	case fDistKullbackLeibler: {
			stDummyTree<HImagem, HImagemCmpKullbackLeibler> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.RangeQuery(&alvo, r);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	case fDistCosseno: {
			stDummyTree<HImagem, HImagemCmpCosseno> arvore(new stPlainDiskPageManager("/tmp/CBIR.db", 1024));
			for (auto it = imagens.begin(); it != imagens.end(); it++)
				arvore.Add(new HImagem(*it, buscaVC(*it)));
			resultado = arvore.RangeQuery(&alvo, r);

			for (unsigned int i = 0; i < resultado->GetNumOfEntries(); i++)
				lista.append((*resultado)[i].GetObject()->_hash);
			delete resultado;
		}
		break;
	}

	return lista;
}

HImagem::HImagem(QString hash = QString(""), QVector<float> VC = QVector<float>()): _hash(hash), _VC(VC)
{
	//construtor vazio
}

const stByte * HImagem::Serialize()
{

	stByte *bytes = new stByte[GetSerializedSize()];
	memcpy(bytes, _hash.data(), 40);
	memcpy(bytes + 40, _VC.data(), sizeof(float) * _VC.size());
	return bytes;

}

void HImagem::Unserialize (const stByte *data, stSize datasize)
{

	_hash = QString(QByteArray(reinterpret_cast<const char *>(data), 40));
	_VC.reserve(1 + (datasize - 40)/sizeof(float));
	memcpy(_VC.data(), data + 40, datasize - 40);
	delete [] data;

}

stSize HImagem::GetSerializedSize()
{

	return 40 + sizeof(float) * _VC.count();

}

HImagem * HImagem::Clone()
{
	return new HImagem(_hash, _VC);
}

bool HImagem::IsEqual(HImagem * outra)
{
	return (_hash == outra->_hash);
}

