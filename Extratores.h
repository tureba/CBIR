#ifndef EXTRATORES_H
#define EXTRATORES_H

#include <QImage>
#include <QVector>

QVector<float> extrairHistograma(QImage imagem);
QVector<float> extrairMatrizCoOcorrencia(QImage imagem);

#endif // EXTRATORES_H
