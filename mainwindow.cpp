#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	bd(NULL)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete bd;
	delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::enterImagem()
{
	QString nome = ui->lineEdit->text();
	if (QFile::exists(nome)) {
		QImage image(nome);
		if (image.isNull()) {
			QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(nome));
			return;
		}
		files.clear();
		files.append(nome);
		ui->imageLabel->setPixmap(QPixmap::fromImage(image));
	}
}

void MainWindow::abreDialogImagem()
{
	files = QFileDialog::getOpenFileNames(this, tr("Open File"), QDir::currentPath(), "Imagens (*.bmp *.png *.xpm *.jpg)");
	for (auto it = files.begin(); it != files.end(); it++)
		if (!QFile::exists(*it)) {
			QImage image(*it);
			if (image.isNull()) {
				QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(*it));
				continue;
			}
		}
	ui->lineEdit->setText(files.join("; "));
	if (QFile::exists(ui->lineEdit->text()))
		ui->imageLabel->setPixmap(QPixmap::fromImage(QImage(ui->lineEdit->text())));
}

void MainWindow::enterBD()
{
	QString nome = ui->lineEdit_2->text();
	delete bd;
	bd = new BD(nome);
	atualizaBD();
}

void MainWindow::abreDialogBD()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
	ui->lineEdit_2->setText(fileName);
	enterBD();
}

void MainWindow::escolheVC(int valor)
{
	if (bd != NULL) {
		switch (valor) {
		case 1:
			bd->adicionaFuncaoExtracao("hist");
			bd->adicionaFuncaoExtracao("matco");
			break;
		case 2:
			bd->adicionaFuncaoExtracao("matco");
			bd->removeFuncaoExtracao("hist");
			break;
		case 3:
			bd->adicionaFuncaoExtracao("hist");
			bd->removeFuncaoExtracao("matco");
			break;
		}
	}
}

void MainWindow::escolheMetrica(int valor)
{
	if (bd != NULL) {
		switch (valor) {
		case 1:
			bd->alteraFuncaoDistancia(BD::fDistCosseno);
			break;
		case 2:
			bd->alteraFuncaoDistancia(BD::fDistMinkowski);
			break;
		case 3:
			bd->alteraFuncaoDistancia(BD::fDistItakuraSaito);
			break;
		case 4:
			bd->alteraFuncaoDistancia(BD::fDistKullbackLeibler);
			break;
		}
	}
}

void MainWindow::adicionaImagem()
{
	if (bd != NULL) {
		for (auto it = files.begin(); it != files.end(); it++)
			if (QFile::exists(*it))
				bd->insereImagem(bd->calcHash(*it));
		atualizaBD();
	}
}

void MainWindow::atualizaBD()
{
	if (bd != NULL) {
		switch (bd->fDist) {
		case BD::fDistCosseno:
			ui->comboBox->setCurrentIndex(0);
			break;
		case BD::fDistMinkowski:
			ui->comboBox->setCurrentIndex(1);
			break;
		case BD::fDistItakuraSaito:
			ui->comboBox->setCurrentIndex(2);
			break;
		case BD::fDistKullbackLeibler:
			ui->comboBox->setCurrentIndex(3);
			break;
		}

		if (bd->usaHistograma && bd->usaMatrizCoOcorrencia)
			ui->comboBox_2->setCurrentIndex(0);
		else if (bd->usaMatrizCoOcorrencia)
			ui->comboBox_2->setCurrentIndex(1);
		else if (bd->usaHistograma)
			ui->comboBox_2->setCurrentIndex(2);

		while (ui->gridLayout->count() > 0)
			delete ui->gridLayout->takeAt(0);

		QLabel * imagem;
		for (auto it = bd->imagens.begin(); it != bd->imagens.end(); it++) {
			imagem = new QLabel();
			imagem->setPixmap(QPixmap::fromImage(bd->retornaImagem(*it).scaledToWidth(100)));
			ui->gridLayout->addWidget(imagem);
		}
	}
}

void MainWindow::buscaImagem()
{
	while (ui->gridLayout_3->count() > 0)
		delete ui->gridLayout_3->takeAt(0);

	if ((bd != NULL) && (QFile::exists(ui->lineEdit->text()))) {
		QStringList respostas;
		if (ui->radioButton->isChecked())
			respostas = bd->buscaN(QImage(ui->lineEdit->text()), ui->spinBox->value());
		else
			respostas = bd->buscaR(QImage(ui->lineEdit->text()), ui->doubleSpinBox->value());

		QLabel * imagem;
		for (auto it = respostas.begin(); it != respostas.end(); it++) {
			imagem = new QLabel();
			imagem->setPixmap(QPixmap::fromImage(bd->retornaImagem(*it).scaledToWidth(100)));
			ui->gridLayout->addWidget(imagem);
		}
	}
}
