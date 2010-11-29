#ifndef DISTANCIAS_H
#define DISTANCIAS_H

#include <QVector>

float distMinkowski(QVector<float> a, QVector<float> b);

float distItakuraSaito(QVector<float> a, QVector<float> b);

float distKullbackLeibner(QVector<float> a, QVector<float> b);

float distCosseno(QVector<float> a, QVector<float> b);

#endif // DISTANCIAS_H
