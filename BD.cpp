#include "BD.h"

#include <QtCore/QCryptographicHash>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QVector>
#include <QtGui/QImage>

#include <algorithm>
#include <cmath>
#include <iostream>

BD::BD(QString nomeBD): nome(nomeBD), dir_base("~/.CBIR/"), usaHistograma(false), usaMatrizCoOcorrencia(false), fDist(fDistCosseno)
{
	QFile arq(dir_base + nome + ".bd");
	if (!arq.open(QIODevice::ReadOnly | QIODevice::Text)) {
		std::cerr << arq.fileName().toStdString() << ": " << arq.error() << std::endl;
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
			if (l[0] == "fdist")
				alteraFuncaoDistancia(l[1].trimmed());
			else if (l[0] == "fext") {
				l = l[1].split(",", QString::SkipEmptyParts);
				for (auto it = l.begin(); it != l.end(); it++)
					adicionaFuncaoExtracao((*it).trimmed());
			}
		} else {
			imagens.insert(linha.trimmed());
		}
	}
}

BD::~BD()
{
	QFile arq(dir_base + nome + ".bd");
	if (!arq.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Append)) {
		std::cerr << arq.fileName().toStdString() << ": " << arq.error() << std::endl;
		return;
	}

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

bool BD::insereImagem(QString hash)
{
	bool ret = imagens.contains(hash);
	if (!ret) {
		imagens.insert(hash);
		calcCaracteristicas(hash);
	}
	return ret;
}

bool BD::removeImagem(QString hash)
{
	bool ret = imagens.contains(hash);
	if (ret)
		imagens.remove(hash);
	return ret;
}
/*
QVector<float> BD::buscaVC(QString hash)
{
	return ;
}
*/
void BD::calcCaracteristicas(QString hash)
{
	if (usaHistograma)
		for (auto it = imagens.begin(); it != imagens.end(); it++)
			if (!QFile::exists(dir_base + hash + ".histograma"))
				salvaVet(extrairHistograma(QImage(dir_base + hash + ".bmp")), dir_base + hash + ".histograma");

	if (usaMatrizCoOcorrencia)
		for (auto it = imagens.begin(); it != imagens.end(); it++)
			if (!QFile::exists(dir_base + hash + ".matrizcoocorrencia"))
				salvaVet(extrairMatrizCoOcorrencia(QImage(dir_base + hash + ".bmp")), dir_base + hash + ".matrizcoocorrencia");
}

QString BD::calcHash(QImage imagem) const
{
	QByteArray pixels = QByteArray(reinterpret_cast<const char *>(imagem.bits()), imagem.byteCount());
	QString hash(QCryptographicHash::hash(pixels, QCryptographicHash::Sha1));
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
	QFile arq(arquivo);
	if (!arq.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream saida(&arq);

	for (auto it = vet.begin(); it != vet.end(); it++)
		saida << *it << " ";
	saida.flush();
	arq.close();
}

QVector<float> BD::carregaVet(QString arquivo)
{
	QFile arq(arquivo);
	if (!arq.open(QIODevice::ReadOnly | QIODevice::Text))
		return QVector<float>(0);

	QTextStream entrada(&arq);

	float x;
	QVector<float> resultado;

	while (!arq.atEnd()) {
		entrada >> x;
		resultado.append(x);
	}
	arq.close();

	return resultado;
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
