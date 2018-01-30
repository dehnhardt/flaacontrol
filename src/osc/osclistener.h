#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#define OSCPKT_OSTREAM_OUTPUT

#include "oschandler.h"

#include <QThread>
#include <map>
#include <string>

class OscListener : public QThread, public OscHandler
{

public:
	OscListener( int iPortNum);
	~OscListener() {}

	void setBRunning(bool bRunning);

protected:
	void run() override;

private:
	void runListener();

private:
	int m_iPortNum = 0;
	bool m_bRunning = false;
	std::map<std::string, OscHandler *> m_HandlerMap;

};

#endif // OSCLISTENER_H
