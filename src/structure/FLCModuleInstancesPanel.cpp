#include "FLCModuleInstancesPanel.h"
#include "FLCModuleWidget.h"
#include "FLCFlowControl.h"
#include "FLCModuleConnection.h"
#include "../Flaacontrol.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "../handler/FLCModuleInstancesHandler.h"
#include "../model/FLCModuleInstancesModel.h"
#include "../model/FLCModuleInstanceAttributesModel.h"
#include "../model/FLCRepositoryModuleModel.h"

#include <QWindow>
#include <QMimeData>
#include <QDrag>
#include <QIcon>
#include <QDebug>
#include <QLayout>
#include <QPainter>

static inline QString sMimeTypeAdd() { return QStringLiteral("application/x-flowcontrol-add"); }
static inline QString sMimeTypeMove() { return QStringLiteral("application/x-flowcontrol-move"); }

FLCModuleInstancesPanel::FLCModuleInstancesPanel(QWidget *parent) : QWidget(parent)
{
	m_pParent = static_cast<FLCFlowControl *>(parent);
	//TODO throw error when parent has wrong type
	if( !parent )
		qDebug() << "Wrong parent";
	setupUi();
}

FLCModuleInstancesPanel::~FLCModuleInstancesPanel()
{

}

void FLCModuleInstancesPanel::setModel(FLCModuleInstancesModel *model)
{
	m_pModel = model;
	initFomModel();
	connect(m_pModel, &FLCModuleInstancesModel::moduleInstanceAdded, this, &FLCModuleInstancesPanel::moduleWidgetAdded);
	connect(m_pModel, &FLCModuleInstancesModel::moduleInstanceModified, this, &FLCModuleInstancesPanel::moduleWidgetModified);
	connect(m_pModel, &FLCModuleInstancesModel::moduleInstanceRemoved, this, &FLCModuleInstancesPanel::moduleWidgetRemoved);
}

void FLCModuleInstancesPanel::setHandler(FLCModuleInstancesHandler *handler)
{
	this->m_pHandler = handler;
	connect(this, &FLCModuleInstancesPanel::addModuleInstance, this->m_pHandler, &FLCModuleInstancesHandler::addModuleInstance);
	connect(this, &FLCModuleInstancesPanel::modifyModuleInstance, this->m_pHandler, &FLCModuleInstancesHandler::modifyModuleInstance);
	connect(this, &FLCModuleInstancesPanel::removeModuleInstance, this->m_pHandler, &FLCModuleInstancesHandler::removeModuleInstance);
}

FLCModuleInstancesModel *FLCModuleInstancesPanel::model() const
{
	return m_pModel;
}

QIcon FLCModuleInstancesPanel::iconForModule(flaarlib::MODULE_TYPE moduleType, flaarlib::DATA_TYPE dataType)
{
	return m_pParent->getFlcModulesModelMap()[moduleType]->icon(dataType).value<QIcon>();
}

void FLCModuleInstancesPanel::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(sMimeTypeAdd()) || event->mimeData()->hasFormat(sMimeTypeMove()))
	{
		qDebug() << "Right MIME";
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

void FLCModuleInstancesPanel::dragMoveEvent(QDragMoveEvent *event)
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

void FLCModuleInstancesPanel::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat(sMimeTypeAdd()))
	{
		const QMimeData *mime = event->mimeData();
		QByteArray itemData = mime->data(sMimeTypeAdd());
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		QString text;
		QPoint offset;
		QIcon icon;
		int moduleType;
		unsigned int index;
		dataStream >> text >> icon >> offset >> moduleType >> index;
		FLOModuleInstanceDAO *moduleInstance = 0;
		if( !icon.isNull() )
		{
			FLCRepositoryModuleModel *m = m_pParent->getFlcModulesModelMap()[flaarlib::MODULE_TYPE(moduleType)];
			FLCRepositoryModule *repositoryModule = m->moduleAt(static_cast<int>(index));
			moduleInstance = new FLOModuleInstanceDAO(repositoryModule->moduleType(),repositoryModule->dataType(),
					repositoryModule->functionalName().c_str(), repositoryModule->moduleTypeName().c_str());
			moduleInstance->setPosition(snapToGrid(event->pos(), offset));
			emit( addModuleInstance(moduleInstance) );
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

void FLCModuleInstancesPanel::setupUi()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setAcceptDrops(true);
}

void FLCModuleInstancesPanel::initFomModel()
{
	if( m_pModel)
	{
		QMap<QUuid, FLOModuleInstanceDAO *> map = m_pModel->getModuleInstancesMap();
		for( auto module : map)
			moduleWidgetAdded(module);
	}
}

FLCModuleWidget *FLCModuleInstancesPanel::createModuleWidget(FLOModuleInstanceDAO *module)
{
	QIcon icon = iconForModule(module->moduleType(), module->dataType());
	FLCModuleWidget *moduleWidget = new FLCModuleWidget(this, icon, module);
	QUuid sUuid = module->uuid();
	moduleWidget->setAttribute(Qt::WA_DeleteOnClose);
	moduleWidget->setUuid(sUuid);
	moduleWidget->move(module->position());
	if( module->position().x() + moduleWidget->width() > width() )
		setMinimumWidth(module->position().x() + moduleWidget->width());
	if( module->position().y() + moduleWidget->height() > height() )
		setMinimumHeight(module->position().y() + moduleWidget->height());
	moduleWidget->show();
	this->m_flcWidgetMap[sUuid] = moduleWidget;
	moduleWidgetSelected(moduleWidget);
	connect(moduleWidget, &FLCModuleWidget::removeModuleWidget, this, &FLCModuleInstancesPanel::removeModuleWidget);
	connect(moduleWidget, &FLCModuleWidget::widgetSelected, this, &FLCModuleInstancesPanel::moduleWidgetSelected);
	connect(moduleWidget, &FLCModuleWidget::portClicked, this, &FLCModuleInstancesPanel::portClicked);
	connect(moduleWidget, &FLCModuleWidget::widgetMoved, this, &FLCModuleInstancesPanel::moveModuleWidget);
	return moduleWidget;
}

void FLCModuleInstancesPanel::deleteModuleWidget(FLCModuleWidget *moduleWidget)
{
	moduleWidget->hide();
	moduleWidget->deleteLater();
	this->m_flcWidgetMap.erase(this->m_flcWidgetMap.find(moduleWidget->getUuid()));
}

QPoint FLCModuleInstancesPanel::snapToGrid(QPoint position, QPoint offset)
{
	QPoint p = position - offset;
	p.setX(static_cast<int>(round(static_cast<double>(p.x())/snap)
							*static_cast<int>(snap)));
	p.setY(static_cast<int>(round(static_cast<double>(p.y())/snap)
							*static_cast<int>(snap)));
	return p;
}

void FLCModuleInstancesPanel::moduleWidgetSelected(FLCModuleWidget *flcModuleWidget)
{
	if(lastSelectedWidgt)
		lastSelectedWidgt->setSeleced(false);
	FLOModuleInstanceDAO *dao = this->m_pModel->getFLOModuleInstance(flcModuleWidget->getUuid());
	FLCModuleInstanceAttributesModel *attributesModel = new FLCModuleInstanceAttributesModel(dao, this);
	m_pParent->setAttributesTableModel(attributesModel);
	connect(attributesModel, &FLCModuleInstanceAttributesModel::modifyModuleInstance,
			this->m_pHandler, &FLCModuleInstancesHandler::modifyModuleInstance );
	flcModuleWidget->setSeleced(true);
	lastSelectedWidgt = flcModuleWidget;
}

void FLCModuleInstancesPanel::moduleWidgetAdded(FLOModuleInstanceDAO *module)
{
	FLCModuleWidget *moduleWidget = this->m_flcWidgetMap[module->uuid()];
	if( !moduleWidget )
		moduleWidget = createModuleWidget(module);
	moduleWidget->setValid(FLCModuleWidget::VALID);
}

void FLCModuleInstancesPanel::moduleWidgetModified(FLOModuleInstanceDAO *module)
{
	FLCModuleWidget *moduleWidget = this->m_flcWidgetMap[module->uuid()];
	//TODO this is an error and should be handled
	if( !moduleWidget )
		return;
	moduleWidget->setData(module);
	if( moduleWidget->pos() != module->position() )
		moduleWidget->move(module->position());
	else
		moduleWidget->repaint();
	moduleWidget->setValid(FLCModuleWidget::VALID);
}

void FLCModuleInstancesPanel::removeModuleWidget(QUuid uuid)
{
	FLCModuleWidget *moduleWidget = this->m_flcWidgetMap[uuid];
	if( moduleWidget )
	{
		moduleWidget->setValid(FLCModuleWidget::INVALID);
		emit(removeModuleInstance(uuid));
	}
}

void FLCModuleInstancesPanel::moduleWidgetRemoved(QUuid uuid)
{
	FLCModuleWidget *moduleWidget = this->m_flcWidgetMap[uuid];
	if( moduleWidget )
		deleteModuleWidget(moduleWidget);
}

void FLCModuleInstancesPanel::portClicked(FLCModuleWidget *flcModuleWidget, flaarlib::PORT_TYPE portType, int portNumber)
{
	grabMouse();
	FLCModuleWidget *storedWidget = m_flcWidgetMap[flcModuleWidget->getUuid()];
	qDebug() << "port " << portNumber << " (" << portType << ") clicked on widget " << flcModuleWidget->getUuid();
	qDebug() << "port " << portNumber << " (" << portType << ") clicked on widget " << storedWidget->getUuid();
	m_tmpModuleConnection = new FLCModuleConnection();
	if( portType == flaarlib::PORT_TYPE::OUTPUT_PORT)
	{
		m_tmpModuleConnection->outputUuid = flcModuleWidget->getUuid().toString();
		m_tmpModuleConnection->outputWidget = flcModuleWidget;
		m_tmpModuleConnection->outputPortNumber = portNumber;
	}
	if( portType == flaarlib::PORT_TYPE::INPUT_PORT)
	{
		m_tmpModuleConnection->inputUuid = flcModuleWidget->getUuid().toString();
		m_tmpModuleConnection->inputWidget = flcModuleWidget;
		m_tmpModuleConnection->inputPortNumber = portNumber;
	}
}

void FLCModuleInstancesPanel::moveModuleWidget(FLCModuleWidget *flcModuleWidget, QPoint pos)
{
	FLOModuleInstanceDAO *floModuleInstanceDAO = this->m_pModel->getFLOModuleInstance(flcModuleWidget->getUuid());
	floModuleInstanceDAO->setPosition(pos);
	emit( modifyModuleInstance(floModuleInstanceDAO));
}


void FLCModuleInstancesPanel::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	if( m_tmpModuleConnection)
	{
		qDebug() << "paint temp connection";
		drawModuleConnection(m_tmpModuleConnection);
	}
	for( FLCModuleConnection *moduleConnection : m_vModuleConnections )
		drawModuleConnection(moduleConnection);
}

void FLCModuleInstancesPanel::drawModuleConnection( FLCModuleConnection *moduleConnection)
{
	QPainter painter(this);
	QPoint start;
	QPoint end;
	if(moduleConnection->outputWidget )
	{
		if( !moduleConnection->outputWidget->paintingActive() )
			start = moduleConnection->outputWidget->getPortOrigin(flaarlib::PORT_TYPE::OUTPUT_PORT, moduleConnection->outputPortNumber);
		else
			return;
	}
	else
		start = m_tmpEndPoint;

	if(moduleConnection->inputWidget )
	{
		if(!moduleConnection->inputWidget->paintingActive() )
			end = moduleConnection->inputWidget->getPortOrigin(flaarlib::PORT_TYPE::INPUT_PORT, moduleConnection->inputPortNumber);
		else
			return;
	}
	else
		end = m_tmpEndPoint;

	painter.drawLine(start, end);
}

void FLCModuleInstancesPanel::mouseMoveEvent(QMouseEvent *event)
{
	qDebug() << "mouse move";
	if( m_tmpModuleConnection )
	{
		m_tmpEndPoint = event->pos();
		repaint();
	}
}

void FLCModuleInstancesPanel::mousePressEvent(QMouseEvent *event)
{
	qDebug() << "mouse press";
}

void FLCModuleInstancesPanel::mouseReleaseEvent(QMouseEvent *event)
{
	QPoint p = event->pos();

	if( m_tmpModuleConnection)
	{
		qDebug() << "Release Mouse";
		releaseMouse();
		for( auto widget :  m_flcWidgetMap )
		{
			QRect r = QRect(widget->pos(), widget->size());
			qDebug() << "is Point " << p << " contained in " << r;
			if( r.contains(p) )
			{
				qDebug() << "Point " << p << " is contained in " << r;
				int port;
				QPoint widgetPos = widget->pos();
				QPoint localPoint = p - widgetPos;
				qDebug() << "mapped point: " << localPoint;
				if( !m_tmpModuleConnection->inputPortNumber  &&  (port = widget->inInputPort(localPoint)))
				{
					qDebug() << "found mathing input port: " << port  << " in widget " << widget->getUuid();
					m_tmpModuleConnection->inputPortNumber = port;
					m_tmpModuleConnection->inputWidget = widget;
					m_tmpModuleConnection->inputUuid = widget->getUuid().toString();
					m_vModuleConnections.append(new FLCModuleConnection(m_tmpModuleConnection));
					break;
				}
				else if(!m_tmpModuleConnection->outputPortNumber  &&  (port = widget->inOutputPort(localPoint)))
				{
					qDebug() << "found mathing output port: " << port;
					m_tmpModuleConnection->outputPortNumber = port;
					m_tmpModuleConnection->outputWidget = widget;
					m_tmpModuleConnection->outputUuid = widget->getUuid().toString();
					m_vModuleConnections.append(new FLCModuleConnection(m_tmpModuleConnection));
					break;
				}
				break;
			}
		}
		qDebug() << "delete temp connection";
		delete m_tmpModuleConnection;
		m_tmpModuleConnection = 0;
	}
}
