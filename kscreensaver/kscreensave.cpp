//-----------------------------------------------------------------------------
// $Id$
// KDE screen saver
//-----------------------------------------------------------------------------

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <kapp.h>

#include "kscreensave.h"
#include <kconfig.h>
#include <kstddirs.h>

int sendSignal()
{
	QString buffer(getenv("HOME"));

	buffer.append("/.kss-install.pid.");
        char ksshostname[200];
        gethostname(ksshostname, 200);
        buffer.append(ksshostname);
	int r = -1;
	FILE *fp;

	if ( (fp = fopen( buffer, "r" ) ) != NULL )
	{
		// a screen saver is running - just tell it to lock
		int pid;

		fscanf( fp, "%d", &pid );
		fclose( fp );

                // But only kill it if the pid isn't -1!
                if (pid > 0) {
                  if( kill( pid, SIGUSR1 ) == 0 )
			  r = 0;
		}
	}

	return r;
}

void kForceLocker()
{
	if( sendSignal() != 0 )
	{
		KConfig *kdisplayConfig = new KConfig( "kdisplayrc");
		kdisplayConfig->setGroup("ScreenSaver");
		bool allowRoot = kdisplayConfig->readBoolEntry( "allowRoot", false );
		delete kdisplayConfig;
		const char *root = "-allow-root";
		if( !allowRoot )
			root = 0;

		// either no saver is running or an old pidFile was not removed
		QString buffer = locate("exe", "kblankscrn.kss");
	    
		if ( fork() == 0 )
		{
			execlp( buffer, buffer, "-install", "-delay", "0", "-lock", root, 0 );

			// if we make it here then try again using default path
			execlp("kblankscrn.kss","kblankscrn.kss", "-install", "-delay", "0", "-lock", root, 0);
		    
			// uh oh - failed
			fprintf( stderr, "Could not invoke kblankscrn.kss in %s\n", buffer.ascii());
			exit (1);
		}
	}
}
