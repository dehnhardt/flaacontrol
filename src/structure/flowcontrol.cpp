#include "flowcontrol.h"
#include "ui_flowcontrol.h"
#include "draggablebutton.h"
#include "../Flaacontrol.h"
#include "../osc/osclistener.h"
#include "../handler/FLCRepositoryModuleHandler.h"
#include "../model/FLCRepositoryModuleModel.h"
#include "FLCModule.h"

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
	if( ! m_bDataLoaded )
		getRepositoryModules();
	connectSlots();
}

FlowControl::~FlowControl()
{
	clearModuleMap();
	delete m_pUi;
}

void FlowControl::clearModuleMap()
{
	for( auto it : m_flcModulesModelMap )
		delete it.second;
	m_flcModulesModelMap.clear();
}

void FlowControl::setupUi()
{
	setAcceptDrops(true);
}

void FlowControl::getRepositoryModules()
{
	using namespace flaarlib;
	m_flcModulesModelMap.clear();
	Flaacontrol *flaaControl = Flaacontrol::instance();
	FLCRepositoryModuleHandler *handler = static_cast<FLCRepositoryModuleHandler *> (flaaControl->udpListener()->handlerFor("/ws/repository/module"));
	for( int i = MODULE_TYPE::INPUT; i <= MODULE_TYPE::PROCESSOR; i++ )
	{
		FLCRepositoryModuleModel *m = new FLCRepositoryModuleModel();
		handler->setModel(MODULE_TYPE(i), m);
		m_flcModulesModelMap[MODULE_TYPE(i)] = m;
	}
	m_bDataLoaded = true;
	setupStructureObjects();
	handler->requestRepository();
}

void FlowControl::setupStructureObjects()
{
	this->m_pUi->inputsListView->setModel( m_flcModulesModelMap[flaarlib::MODULE_TYPE::INPUT] );
	this->m_pUi->processorsListView->setModel( m_flcModulesModelMap[flaarlib::MODULE_TYPE::PROCESSOR] );
	this->m_pUi->outputsListView->setModel( m_flcModulesModelMap[flaarlib::MODULE_TYPE::OUTPUT] );
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
		QIcon icon;
		dataStream >> text >> icon;

		if( !icon.isNull() )
		{
			FLCModule *module = new FLCModule(this, text, &icon);
			module->move(event->pos() - offset);
			module->show();
			module->setAttribute(Qt::WA_DeleteOnClose);
		}
		else
		{
			QLabel *newLabel = new QLabel(text, this);
			newLabel->move(event->pos() - offset);
			newLabel->show();
			newLabel->setAttribute(Qt::WA_DeleteOnClose);
		}
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

void FlowControl::connectSlots()
{
	//connect(m_pUi->inputsListView, &QAbstractItemView::clicked, this, &FlowControl::repositoryItemClicked);
	//connect(m_pUi->processorsListView, &QAbstractItemView::clicked, this, &FlowControl::repositoryItemClicked);
	//connect(m_pUi->outputsListView, &QAbstractItemView::clicked, this, &FlowControl::repositoryItemClicked);
}

