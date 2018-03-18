#include "FLCFlowControl.h"
#include "ui_flowcontrol.h"
#include "../Flaacontrol.h"
#include "../flaaoscsdk/osclistener.h"
#include "../handler/FLCRepositoryModuleHandler.h"
#include "../model/FLCRepositoryModuleModel.h"
#include "../flaaoscsdk/FLOModuleInstanceDAO.h"
#include "../model/FLCModuleInstancesModel.h"
#include "../handler/FLCModuleInstancesHandler.h"
#include "FLCModuleWidget.h"
#include "FLCMainModuleInstancesPanel.h"

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

FLCFlowControl::FLCFlowControl(QWidget *parent) :
	QDialog(parent),
	m_pUi(new Ui::FlowControl)
{
	m_pUi->setupUi(this);
	setupUi();
	if( ! m_bDataLoaded )
		getRepositoryModules();
	connectSlots();
	FLCMainModuleInstancesPanel *mainPanel = new FLCMainModuleInstancesPanel(this);
	m_pModel = Flaacontrol::instance()->moduleInstancesModel();
	mainPanel->setModel(m_pModel);
	mainPanel->setHandler(Flaacontrol::instance()->moduleInstancesHandler());
	this->m_pUi->centralScrollArea->setWidget( mainPanel);
	mainPanel->show();
}

FLCFlowControl::~FLCFlowControl()
{
	clearModuleMap();
	delete m_pUi;
}

void FLCFlowControl::clearModuleMap()
{
	for( auto it : m_flcModulesModelMap )
		delete it.second;
	m_flcModulesModelMap.clear();
}

void FLCFlowControl::saveStructure()
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

std::map<flaarlib::MODULE_TYPE, FLCRepositoryModuleModel *> FLCFlowControl::getFlcModulesModelMap() const
{
	return m_flcModulesModelMap;
}

void FLCFlowControl::setAttributesTableModel(FLCModuleInstanceAttributesModel *moduleInstanceAttributeModel)
{
	this->m_pUi->moduleInstanceAtrributeTable->setModel(moduleInstanceAttributeModel);
}

QIcon FLCFlowControl::iconForModule(flaarlib::MODULE_TYPE moduleType, flaarlib::DATA_TYPE dataType)
{
	return m_flcModulesModelMap[moduleType]->icon(dataType).value<QIcon>();
}

void FLCFlowControl::setupUi()
{
	setAcceptDrops(true);
}

void FLCFlowControl::getRepositoryModules()
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

void FLCFlowControl::setupStructureObjects()
{
	this->m_pUi->inputsListView->setModel( m_flcModulesModelMap[flaarlib::MODULE_TYPE::INPUT] );
	this->m_pUi->processorsListView->setModel( m_flcModulesModelMap[flaarlib::MODULE_TYPE::PROCESSOR] );
	this->m_pUi->outputsListView->setModel( m_flcModulesModelMap[flaarlib::MODULE_TYPE::OUTPUT] );
}

void FLCFlowControl::connectSlots()
{
	connect(this->m_pUi->buttonBox, &QDialogButtonBox::accepted, Flaacontrol::instance()->moduleInstancesHandler(), &FLCModuleInstancesHandler::requestSave);
}

