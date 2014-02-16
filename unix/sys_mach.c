/*
===========================================================================
    Copyright (C) 2010-2013  Ninja and TheKelm of the IceOps-Team
    Copyright (C) 1999-2005 Id Software, Inc.

    This file is part of CoD4X17a-Server source code.

    CoD4X17a-Server source code is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    CoD4X17a-Server source code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================
*/


#include "../q_shared.h"
#include "../q_platform.h"
#include "../qcommon_mem.h"
#include "../qcommon_io.h"
#include "../qcommon.h"
#include "../sys_main.h"
#include "../cmd.h"
#include "../sys_cod4defs.h"
#include "../sec_crypto.h"
#include "../sec_update.h"

#include <sys/resource.h>
#include <libgen.h>
#include <signal.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <execinfo.h>


static char homePath[MAX_OSPATH];

/*
==================
Sys_DefaultHomePath
==================
*/


const char *Sys_DefaultHomePath(void)
{
	char *p;

	if( !*homePath )
	{
		if( ( p = getenv( "HOME" ) ) != NULL )
		{
			Com_sprintf(homePath, sizeof(homePath), "%s%c", p, PATH_SEP);
			Q_strcat(homePath, sizeof(homePath),
				"Library/Application Support/");

			Q_strcat(homePath, sizeof(homePath), HOMEPATH_NAME_MACOSX);
		}
	}

	return homePath;
}

/*
==============
Sys_PlatformInit

Unix specific initialisation
==============
*/
void Sys_PlatformInit( void )
{


}

/*
=================
Sys_StripAppBundle

Discovers if passed dir is suffixed with the directory structure of a Mac OS X
.app bundle. If it is, the .app directory structure is stripped off the end and
the result is returned. If not, dir is returned untouched.
=================
*/
const char *Sys_StripAppBundle( const char *dir )
{
	static char cwd[MAX_OSPATH];

	Q_strncpyz(cwd, dir, sizeof(cwd));
	if(strcmp(Sys_Basename(cwd), "MacOS"))
		return dir;
	Q_strncpyz(cwd, Sys_Dirname(cwd), sizeof(cwd));
	if(strcmp(Sys_Basename(cwd), "Contents"))
		return dir;
	Q_strncpyz(cwd, Sys_Dirname(cwd), sizeof(cwd));
	if(!strstr(Sys_Basename(cwd), ".app"))
		return dir;
	Q_strncpyz(cwd, Sys_Dirname(cwd), sizeof(cwd));
	return cwd;
}