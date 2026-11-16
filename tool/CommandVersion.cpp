/*****************************************************************************
 *
 *  $Id$
 *
 *  Copyright (C) 2006-2009  Florian Pose, Ingenieurgemeinschaft IgH
 *
 *  This file is part of the IgH EtherCAT Master.
 *
 *  The IgH EtherCAT Master is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License version 2, as
 *  published by the Free Software Foundation.
 *
 *  The IgH EtherCAT Master is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the IgH EtherCAT Master; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  ---
 *
 *  The license mentioned above concerns the source code only. Using the
 *  EtherCAT technology and brand is only permitted in compliance with the
 *  industrial property and similar rights of Beckhoff Automation GmbH.
 *
 ****************************************************************************/

#include <fstream>
#include <iostream>
using namespace std;

#include "CommandVersion.h"
#include "ecrt.h"

/*****************************************************************************/

CommandVersion::CommandVersion():
    Command("version", "Show version information.")
{
}

/****************************************************************************/

string CommandVersion::helpString(const string &binaryBaseName) const
{
    stringstream str;

    str << binaryBaseName << " " << getName() << " [OPTIONS]" << endl
        << endl
        << getBriefDescription() << endl;

    return str.str();
}

/****************************************************************************/

void CommandVersion::execute(const StringVector &args)
{
    if (args.size()) {
        stringstream err;
        err << "'" << getName() << "' takes no arguments!";
        throwInvalidUsageException(err);
    }

    // The first line is unchanged so that scripts parsing it keep working.
    cout << "IgH EtherCAT master " << EC_MASTER_VERSION << endl;

    // The package version stays 1.5.2 while this fork's interfaces differ
    // from upstream 1.5.2 (and from 1.6). Applications must be built against
    // the same ecrt interface as the installed library, and this tool talks
    // to the kernel module over the ioctl interface.
    cout << "ecrt interface " << ECRT_VER_MAJOR << "." << ECRT_VER_MINOR
        << "." << ECRT_VER_PATCH
        << ", ioctl interface " << EC_IOCTL_VERSION_MAGIC << endl;

    // The tool and the loaded module may come from different builds.
    ifstream module_version("/sys/module/ec_master/version");
    string line;
    if (module_version && getline(module_version, line)) {
        cout << "kernel module " << line << endl;
    }
}

/*****************************************************************************/
