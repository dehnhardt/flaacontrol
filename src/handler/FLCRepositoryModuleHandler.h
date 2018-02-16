#ifndef FLCREPOSITORYMODULEHANDLER_H
#define FLCREPOSITORYMODULEHANDLER_H

#include "../flaaoscsdk/oschandler.h"
#include "../model/FLCRepositoryModule.h"

#include <map>
#include <vector>

class FLCRepositoryModuleModel;

class FLCRepositoryModuleHandler : public OscHandler
{
public:
	FLCRepositoryModuleHandler();

public: //methods
	bool handle(UdpSocket *socket, Message *message) override;
	void requestRepository();

	void setModel( flaarlib::MODULE_TYPE moduleType, FLCRepositoryModuleModel *repositoryModuleModel );

private: //methods
	void addToModel( FLCRepositoryModule *flcModule);

private: //members
	std::map<flaarlib::MODULE_TYPE, FLCRepositoryModuleModel *> m_flcModuleModels;
};

#endif // FLCREPOSITORYMODULEHANDLER_H
