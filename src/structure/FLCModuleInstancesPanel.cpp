#include "FLCModuleInstancesPanel.h"
#include "FLCModuleWidget.h"
#include "FLCFlowControl.h"
#include "../Flaacontrol.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "../model/FLCModuleInstancesModel.h"
#include "../model/FLCRepositoryModuleModel.h"
#include "../handler/FLCModuleInstancesHandler.h"
#include "../model/FLCModuleInstanceAttributesModel.h"

#include <QWindow>
#include <QMimeData>
#include <QDrag>
#include <QIcon>
#include <QDebug>
#include <QLayout>

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

void FLCModuleInstancesPanel::mousePressEvent(QMouseEvent *event)
{
	QPoint dragStartPosition;
	if (event->button() == Qt::LeftButton )
	{
		dragStartPosition = event->pos();
		QWidget *w = childAt(dragStartPosition);
		FLCModuleWidget *m = dynamic_cast<FLCModuleWidget *>(w);
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
			FLOModuleInstanceDAO *moduleInstance = m_pModel->getFLOModuleInstance(uuid);
			moduleInstance->setPosition(snapToGrid(event->pos(), offset));
			FLCModuleWidget *moduleWidget = createModuleWidget(moduleInstance);
			this->m_flcWidgetMap[uuid] = moduleWidget;
			emit( modifyModuleInstance(moduleInstance));
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
	FLCModuleWidget *moduleWidget = new FLCModuleWidget(this, module->moduleFunctionalName(), icon, module->moduleName());
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
	if( moduleWidget->pos() != module->position() )
		moduleWidget->move(module->position());
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
