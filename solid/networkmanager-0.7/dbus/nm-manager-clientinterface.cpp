/*
 * This file was generated by dbusxml2cpp version 0.6
 * Command line was: dbusxml2cpp -N -m -p nm-manager-clientinterface /space/kde/sources/trunk/KDE/kdebase/workspace/solid/networkmanager-0.7/dbus/introspection/nm-manager-client.xml
 *
 * dbusxml2cpp is Copyright (C) 2006 Trolltech ASA. All rights reserved.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "nm-manager-clientinterface.h"

/*
 * Implementation of interface class OrgFreedesktopNetworkManagerInterface
 */

OrgFreedesktopNetworkManagerInterface::OrgFreedesktopNetworkManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

OrgFreedesktopNetworkManagerInterface::~OrgFreedesktopNetworkManagerInterface()
{
}


#include "nm-manager-clientinterface.moc"
