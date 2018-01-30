#ifndef FLCREPOSITORYMODULEHANDLER_H
#define FLCREPOSITORYMODULEHANDLER_H

#include "../osc/oschandler.h"
#include "../model/FLCRepositoryModule.h"

#include <map>
#include <vector>

class FLCRepositoryModuleHandler : public OscHandler
{
public:
	FLCRepositoryModuleHandler();

public: //methods
	bool handle(UdpSocket *socket, Message *message) override;
	void requestRepository();

	std::map<flaarlib::MODULE_TYPE, std::vector<FLCRepositoryModule *>*> getRepository();

private: //methods
	void addToRepository( FLCRepositoryModule *flcModule);

private: //members
	std::map<flaarlib::MODULE_TYPE, std::vector<FLCRepositoryModule *>*> m_flcModulesMap;
};

#endif // FLCREPOSITORYMODULEHANDLER_H
