#include "bd.h"

#include <QCryptographicHash>
#include <QFile>
#include <QStringList>
#include <QTextStream>

BD::BD(QString nomeBD): nome(nomeBD), dir_base("~/CBIR/bd/"), usaHistograma(false), usaMatrizCoOcorrencia(false), fDist(distCosseno)
{
	QFile arq(dir_base + nome + ".bd");
	if (!arq.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

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

void BD::alteraFuncaoDistancia(QString f)
{
	if (f == "Minkowski")
		fDist = distMinkowski;
	else if (f == "ItakuraSaito")
		fDist = distItakuraSaito;
	else if (f == "KullbackLeibner")
		fDist = distKullbackLeibner;
	else if (f == "Cosseno")
		fDist = distCosseno;
	else
		fDist = distCosseno;

}

void BD::alteraFuncaoDistancia(FcDistancia f)
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
	QByteArray _hash = QCryptographicHash::hash(pixels, QCryptographicHash::Sha1);
	QString hash(_hash);
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

QVector<float> BD::CarregaVet(QString arquivo)
{
	QFile arq(arquivo);
	if (!arq.open(QIODevice::ReadOnly | QIODevice::Text))
		return QVector<float>::QVector(0);

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