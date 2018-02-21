#include "FlowControl.h"
#include "ui_flowcontrol.h"
#include "../Flaacontrol.h"
#include "../flaaoscsdk/osclistener.h"
#include "../handler/FLCRepositoryModuleHandler.h"
#include "../model/FLCRepositoryModuleModel.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "../model/FLCModuleInstancesModel.h"
#include "FLCModuleWidget.h"

#include <QWindow>
#include <QIcon>
#include <QString>
#include <QLabel>
#include <QPoint>
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QDrag>
#include <QTextCodec>
#include <QtDebug>

static inline QString sMimeTypeAdd() { return QStringLiteral("application/x-flowcontrol-add"); }
static inline QString sMimeTypeMove() { return QStringLiteral("application/x-flowcontrol-move"); }

FlowControl::FlowControl(QWidget *parent) :
	QDialog(parent),
	m_pUi(new Ui::FlowControl)
{
	m_pUi->setupUi(this);
	setupUi();
	if( ! m_bDataLoaded )
		getRepositoryModules();
	connectSlots();
	setModel(Flaacontrol::instance()->moduleInstancesModel());
}

FlowControl::~FlowControl()
{
	clearModuleMap();
	delete m_pUi;
}

void FlowControl::setModel(FLCModuleInstancesModel *model)
{
	m_pModel = model;
	initFomModel();
	connect(m_pModel, &FLCModuleInstancesModel::moduleAdded, this, &FlowControl::addModuleWidget);
}



void FlowControl::clearModuleMap()
{
	for( auto it : m_flcModulesModelMap )
		delete it.second;
	m_flcModulesModelMap.clear();
}

void FlowControl::initFomModel()
{
	if( m_pModel)
	{
		QMap<QUuid, FLOModuleInstanceDAO *> map = m_pModel->getModuleInstancesMap();
		for( auto module : map)
			addModuleWidget(module);
	}
}

void FlowControl::saveStructure()
{
	QFile *file = new QFile("/home/dehnhardt/structure.xml");
	if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QXmlStreamWriter *w = new QXmlStreamWriter(file);
	w->setAutoFormatting(true);
	w->setCodec(QTextCodec::codecForName("utf-8"));
	w->writeStartDocument("1.0");
	w->writeStartElement("ModuleInstances");
	m_pModel->serialize(w);
	w->writeEndElement();
	w->writeEndDocument();
	file->close();
}

void FlowControl::addModuleWidget(FLOModuleInstanceDAO *module)
{
	QIcon icon = iconForModule(module->moduleType(), module->dataType());
	FLCModuleWidget *moduleWidget = new FLCModuleWidget(this, module->moduleFunctionalName(), icon);
	QUuid sUuid = module->uuid();
	moduleWidget->setAttribute(Qt::WA_DeleteOnClose);
	moduleWidget->setUuid(sUuid);
	moduleWidget->move(module->position());
	moduleWidget->show();
	this->m_flcWidgetMap[sUuid] = moduleWidget;
}

void FlowControl::removeModuleWidget(QUuid uuid)
{
	FLCModuleWidget *moduleWidget = this->m_flcWidgetMap[uuid];
	if( moduleWidget )
	{
		moduleWidget->hide();
		moduleWidget->deleteLater();
		this->m_flcWidgetMap.erase(this->m_flcWidgetMap.find(uuid));
	}
}

FLCModuleInstancesModel *FlowControl::model() const
{
	return m_pModel;
}

QIcon FlowControl::iconForModule(flaarlib::MODULE_TYPE moduleType, flaarlib::DATA_TYPE dataType)
{
	return m_flcModulesModelMap[moduleType]->icon(dataType).value<QIcon>();
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

void FlowControl::mousePressEvent(QMouseEvent *event)
{
	QPoint dragStartPosition;
	if (event->button() == Qt::LeftButton )
	{
		dragStartPosition = event->pos();
		QWidget *w = childAt(dragStartPosition);
		FLCModuleWidget *m = dynamic_cast<FLCModuleWidget *>(w);
		if( !m )
			m = dynamic_cast<FLCModuleWidget *>(w->parentWidget());
		if( m )
		{
			QPoint hotSpot = dragStartPosition - m->pos();
			QByteArray encodedData;
			QDataStream stream(&encodedData, QIODevice::WriteOnly);
			QDrag *drag = new QDrag(this);
			QMimeData *mimeData = new QMimeData;
			QString text = m->functionalName();
			const QIcon icon = m->moduleIcon();
			QUuid uuid = m->getUuid();
			stream << text << icon << hotSpot << uuid;
			mimeData->setData(sMimeTypeMove(), encodedData);

			qreal dpr = window()->windowHandle()->devicePixelRatio();
			QPixmap pixmap(m->size() * dpr);
			pixmap.setDevicePixelRatio(dpr);
			m->render(&pixmap);

			drag->setMimeData(mimeData);
			drag->setPixmap(pixmap);
			drag->setHotSpot(hotSpot);

			Qt::DropAction dropAction = drag->exec();
			if( dropAction == Qt::MoveAction)
			{
				m->hide();
				m->deleteLater();
			}
		}
	}
}

void FlowControl::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(sMimeTypeAdd()) || event->mimeData()->hasFormat(sMimeTypeMove()))
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
	if (event->mimeData()->hasFormat(sMimeTypeAdd()) || event->mimeData()->hasFormat(sMimeTypeMove() ))
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
	if (event->mimeData()->hasFormat(sMimeTypeAdd()))
	{
		const QMimeData *mime = event->mimeData();
		QByteArray itemData = mime->data(sMimeTypeAdd());
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QString text;
		QPoint offset;
		QIcon icon;
		QString sUuid;
		int moduleType;
		unsigned int index;
		dataStream >> text >> icon >> offset >> moduleType >> index;
		FLOModuleInstanceDAO *moduleInstance = 0;
		if( !icon.isNull() )
		{
			FLCRepositoryModuleModel *m = m_flcModulesModelMap[flaarlib::MODULE_TYPE(moduleType)];
			FLCRepositoryModule *repositoryModule = m->moduleAt(index);
			moduleInstance = new FLOModuleInstanceDAO(repositoryModule->moduleType(),repositoryModule->dataType(),
					repositoryModule->functionalName().c_str(), repositoryModule->moduleTypeName().c_str());
			sUuid = moduleInstance->uuid().toString();
			moduleInstance->setPosition(event->pos() - offset);
			m_pModel->addFLOModuleInstance(moduleInstance);
		}
		if (event->source() == this)
		{
			event->setDropAction(Qt::CopyAction);
			event->accept();
		}
		else
			event->acceptProposedAction();
	}
	else if (event->mimeData()->hasFormat(sMimeTypeMove()))
	{
		const QMimeData *mime = event->mimeData();
		QByteArray itemData = mime->data(sMimeTypeMove());
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QString text;
		QPoint offset;
		QIcon icon;
		QUuid uuid;
		dataStream >> text >> icon >> offset >>uuid;
		if( !icon.isNull() && uuid != "" )
		{
			FLCModuleWidget *moduleWidget = new FLCModuleWidget(this, text, icon);
			FLOModuleInstanceDAO *moduleInstance = m_pModel->getFLOModuleInstance(uuid);
			moduleInstance->setPosition(event->pos() - offset);
			moduleWidget->setUuid(uuid);
			moduleWidget->move(event->pos() - offset);
			moduleWidget->show();
			moduleWidget->setAttribute(Qt::WA_DeleteOnClose);
			this->m_flcWidgetMap[uuid] = moduleWidget;
		}
		if (event->source() == this)
		{
			event->setDropAction(Qt::CopyAction);
			event->accept();
		}
		else
			event->acceptProposedAction();
	}
	else
		event->ignore();

}

void FlowControl::connectSlots()
{
	connect(this->m_pUi->buttonBox, &QDialogButtonBox::accepted, this, &FlowControl::saveStructure);
}

