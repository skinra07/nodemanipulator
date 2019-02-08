#include "nodemanipulator.h"
#include "..\TreeViewModel\treeviewmodel.h"
#include "..\TreeViewModel\itemtreemodel.h"

#include <QAbstractListModel>
#include <QString>
#include <QFileDialog>
#include <QDEBUG>
#include <QMessageBox>


NodeManipulator::NodeManipulator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	createActions();

	initalizeModel();
	
}

NodeManipulator::~NodeManipulator()
{
	delete _model;
}

void NodeManipulator::createActions()
{
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionLoad, &QAction::triggered, this, &NodeManipulator::load);
	connect(ui.actionSave, &QAction::triggered, this, &NodeManipulator::save);
	connect(ui.actionSaveAs, &QAction::triggered, this, &NodeManipulator::saveAs);
	connect(ui.RemoveActionNode, &QAction::triggered, this, &NodeManipulator::removeNode);
}

void NodeManipulator::initalizeModel()
{
	_model = new ItemTreeModel(this);
	ui.treeView->setModel(_model);
}

// signals/slots
void NodeManipulator::load()
{
	_model->clear();

	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty())
	{
		QFile file(filename);
		if (!file.open(QFile::ReadOnly))
		{
			QMessageBox::warning(this, tr("NodeManipulator"),
				tr("Cannot read file %1:\n%2.")
				.arg(QDir::toNativeSeparators(filename), file.errorString()));

			return;
		}

		if (_model->load(filename))
		{
			this->currentFile = filename;
		}
		else
		{
			QMessageBox::warning(this, tr("NodeManipulator"),
				tr("Invalid file %1\n")
				.arg(QDir::toNativeSeparators(filename)));
			return;
		}
	}
}

bool NodeManipulator::save()
{
	if (currentFile.isEmpty())
	{
		return (saveAs());
	}
	
	return (saveFile(currentFile));
}

bool NodeManipulator::saveAs()
{
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() != QDialog::Accepted)
	{
		return false;
	}
	return (saveFile(dialog.selectedFiles().first()));
}

bool NodeManipulator::saveFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QFile::WriteOnly | QFile::Text)) 
	{
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(QDir::toNativeSeparators(filename),
				file.errorString()));
		return false;
	}

	if (_model->save(filename))
	{
		this->currentFile = filename;
		statusBar()->showMessage(tr("File saved"), 2000);
		return true;
	}
	
	return false;

}

void NodeManipulator::removeNode()
{
	QModelIndex index = ui.treeView->selectionModel()->currentIndex();

	_model->removeNode(index);
}

