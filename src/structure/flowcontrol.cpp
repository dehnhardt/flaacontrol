#include "flowcontrol.h"
#include "ui_flowcontrol.h"
#include "draggablebutton.h"
#include "../Flaacontrol.h"
#include "../osc/osclistener.h"
#include "../handler/FLCRepositoryModuleHandler.h"

#include <QIcon>
#include <QString>
#include <QLabel>
#include <QPoint>
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QDrag>
#include <QtDebug>

static inline QString sMimeType() { return QStringLiteral("application/x-flowcontrol"); }

FlowControl::FlowControl(QWidget *parent) :
	QDialog(parent),
	m_pUi(new Ui::FlowControl)
{
	m_pUi->setupUi(this);
	setupUi();
	getRepositoryModules();
	setupDraggableObjects();
}

FlowControl::~FlowControl()
{
	delete m_pUi;
}

void FlowControl::setupUi()
{
	setAcceptDrops(true);
}

void FlowControl::setupDraggableObjects()
{
	using namespace flaarlib;
	for( auto outerIt : m_flcModulesMap )
	{
		MODULE_TYPE t = outerIt.first;
		QVBoxLayout *currentLayout = 0;
		switch( t )
		{
			case INPUT:
				currentLayout = this->m_pUi->inputsLayout;
				break;
			case OUTPUT:
				currentLayout = this->m_pUi->outputsLayout;
				break;
			case PROCESSOR:
				currentLayout = this->m_pUi->processorsLayout;
				break;
		}
		std::vector<FLCRepositoryModule *> *v = outerIt.second;
		for( auto innerIt : *v )
		{
			FLCRepositoryModule *m = innerIt;
			qDebug() << "Found " <<  m->functionalName().c_str();
			DraggableButton *btn = new DraggableButton( m->functionalName().c_str(), QIcon(":/icons/midijack"), this);
			btn->setToolTip(m->description().c_str());
			currentLayout->insertWidget(-1, btn);
			connect(btn, SIGNAL(buttonDragStart(DraggableButton *, QMouseEvent *)), this, SLOT(buttonDragStart(DraggableButton *, QMouseEvent *)));
		}
	}
	/*
	DraggableButton *btn1 = new DraggableButton( "MIDI-In", QIcon(":/icons/midijack"), this);
	this->m_pUi->inputsLayout->insertWidget(0, btn1);
	connect(btn1, SIGNAL(buttonDragStart(DraggableButton *, QMouseEvent *)), this, SLOT(buttonDragStart(DraggableButton *, QMouseEvent *)));
	DraggableButton *btn2 = new DraggableButton( "Audio-In", QIcon(":/icons/audiocombojack"), this);
	this->m_pUi->inputsLayout->insertWidget(1, btn2);
	connect(btn2, SIGNAL(buttonDragStart(DraggableButton *, QMouseEvent *)), this, SLOT(buttonDragStart(DraggableButton *, QMouseEvent *)));
	*/
}

void FlowControl::getRepositoryModules()
{
	m_flcModulesMap.clear();
	Flaacontrol *flaaControl = Flaacontrol::instance();
	FLCRepositoryModuleHandler *handler = static_cast<FLCRepositoryModuleHandler *> (flaaControl->pUdpListener()->handlerFor("/ws/repository/module"));
	handler->requestRepository();
	m_flcModulesMap = handler->getRepository();
}


void FlowControl::buttonDragStart(DraggableButton *draggableButton, QMouseEvent *event)
{
	DraggableButton *child = draggableButton;
	if (!child)
		return;

	QPoint hotSpot = event->pos() - child->pos();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << child->text() << QPoint(hotSpot);
	QMimeData *mimeData = new QMimeData;
	mimeData->setData(sMimeType(), itemData);
	mimeData->setText(child->text());

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setHotSpot(hotSpot);

	//child->hide();
	if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
		child->close();
	else
		child->show();

}


void FlowControl::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(sMimeType()))
	{
		if (children().contains(event->source()))
		{
			event->setDropAction(Qt::CopyAction);
			event->accept();
		}
		else
			event->acceptProposedAction();
	}
	else if (event->mimeData()->hasText())
		event->acceptProposedAction();
	else
		event->ignore();

}

void FlowControl::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(sMimeType()))
	{
		if (children().contains(event->source()))
		{
			event->setDropAction(Qt::CopyAction);
			event->accept();
		}
		else
			event->acceptProposedAction();
	}
	else if (event->mimeData()->hasText())
		event->acceptProposedAction();
	else
		event->ignore();

}

void FlowControl::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat(sMimeType()))
	{
		const QMimeData *mime = event->mimeData();
		QByteArray itemData = mime->data(sMimeType());
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QString text;
		QPoint offset;
		dataStream >> text >> offset;
		QLabel *newLabel = new QLabel(text, this);
		newLabel->move(event->pos() - offset);
		newLabel->show();
		newLabel->setAttribute(Qt::WA_DeleteOnClose);

		if (event->source() == this)
		{
			event->setDropAction(Qt::CopyAction);
			event->accept();
		}
		else
			event->acceptProposedAction();
	}
	else if (event->mimeData()->hasText())
	{
		QStringList pieces = event->mimeData()->text().split(QRegularExpression(QStringLiteral("\\s+")),
							 QString::SkipEmptyParts);
		QPoint position = event->pos();

		for (const QString &piece : pieces)
		{
			QLabel *newLabel = new QLabel(piece, this);
			newLabel->move(position);
			newLabel->show();
			newLabel->setAttribute(Qt::WA_DeleteOnClose);

			position += QPoint(newLabel->width(), 0);
		}

		event->acceptProposedAction();
	}
	else
		event->ignore();

}

