#ifndef FLCPINGHANDLER_H
#define FLCPINGHANDLER_H

#include "../flaaoscsdk/oschandler.h"

class FLCPingHandler : public OscHandler
{
public:
	FLCPingHandler();
	bool handle(oscpkt::UdpSocket *socket, oscpkt::Message *message) override;
	void sendPing();

private:
	int m_iPing;
};

#endif // FLCPINGHANDLER_H
