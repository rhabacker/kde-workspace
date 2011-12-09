/*
 * Copyright 2011 Sebastian Kügler <sebas@kde.org>
 * Copyright 2011 Viranch Mehta <viranch.mehta@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusConnectionInterface>
#include <QDBusMessage>
#include <QDBusPendingReply>

#include <KAuthorized>

// kde-workspace/libs
#include <kworkspace/kworkspace.h>

// plasma
#include <Plasma/DataEngine>

#include "powermanagementjob.h"
#include "powermanagementengine.h"

#include <kdebug.h>

PowermanagementJob::PowermanagementJob(const QString &operation, QMap<QString, QVariant> &parameters, QObject *parent) :
    ServiceJob(parent->objectName(), operation, parameters, parent)
{
    setParent(parent);
}

PowermanagementJob::~PowermanagementJob()
{
}

void PowermanagementJob::start()
{
    const QString operation = operationName();
    //kDebug() << "starting operation  ... " << operation;

    if (operation == "lockScreen") {
        if (KAuthorized::authorizeKAction("lock_screen")) {
            const QString interface("org.freedesktop.ScreenSaver");
            QDBusInterface screensaver(interface, "/ScreenSaver");
            screensaver.asyncCall("Lock");
            setResult(true);
            return;
        }
        kDebug() << "operation denied " << operation;
        setResult(false);
        return;
    } else if (operation == "suspend" || operation == "suspendToRam") {
        setResult(suspend(Ram));
        return;
    } else if (operation == "suspendToDisk") {
        setResult(suspend(Disk));
        return;
    } else if (operation == "suspendHybrid") {
        setResult(suspend(Hybrid));
        return;
    } else if (operation == "requestShutDown") {
        requestShutDown();
        setResult(true);
        return;
    } else if (operation == "setBrightness") {
        if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.Solid.PowerManagement")) {
            QDBusMessage call = QDBusMessage::createMethodCall ("org.kde.Solid.PowerManagement",
                                                                "/org/kde/Solid/PowerManagement",
                                                                "org.kde.Solid.PowerManagement",
                                                                "setBrightness");
            int brightness = parameters().value("brightness").toInt();
            call.setArguments(QList<QVariant>() << QVariant::fromValue(brightness));
            QDBusConnection::sessionBus().asyncCall(call);
            setResult(true);
            return;
        } else {
            kDebug() << "set brightness: DBus org.kde.Solid.PowerMangement not available.";
        }
    } else if (operation == "switchUser") {
        if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.krunner")) {
            QDBusMessage call = QDBusMessage::createMethodCall ("org.kde.krunner",
                                                                "/App",
                                                                "org.kde.krunner.App",
                                                                "switchUser");
            QDBusConnection::sessionBus().asyncCall (call);
            setResult(true);
            return;
        } else {
            kDebug() << "switch user: DBus org.kde.krunner not available.";
        }
    } else if (operation == "setProfile") {
        if (QDBusConnection::sessionBus().interface()->isServiceRegistered("org.kde.Solid.PowerManagement")) {
            QDBusMessage call = QDBusMessage::createMethodCall ("org.kde.Solid.PowerManagement",
                                                                "/org/kde/Solid/PowerManagement",
                                                                "org.kde.Solid.PowerManagement",
                                                                "loadProfile");

            Plasma::DataEngine::Data data = qobject_cast<PowermanagementEngine*>(parent())->query("PowerDevil");
            StringStringMap availableProfiles = data["Available profiles"].value< StringStringMap >();
            QString profile = availableProfiles.key(parameters().value("profile").toString());

            call.setArguments(QList<QVariant>() << QVariant::fromValue(profile));
            QDBusConnection::sessionBus().asyncCall (call);
            setResult(true);
            return;
        } else {
            kDebug() << "set profile: DBus org.kde.Solid.PowerMangement not available.";
        }
    }

    kDebug() << "don't know what to do with " << operation;
    setResult(false);
}

bool PowermanagementJob::suspend(const SuspendType &type)
{
    QDBusMessage msg = QDBusMessage::createMethodCall("org.kde.Solid.PowerManagement",
                                                      "/org/kde/Solid/PowerManagement",
                                                      "org.kde.Solid.PowerManagement",
                                                      callForType(type));
    QDBusConnection::sessionBus().asyncCall(msg);
    return true;
}

QString PowermanagementJob::callForType(const SuspendType &type)
{
    switch (type) {
        case Disk:
            return "suspendToDisk";
        break;

        case Hybrid:
            return "suspendHybrid";
        break;

        default:
            return "suspendToRam";
        break;
    }
}

void PowermanagementJob::requestShutDown()
{
    KWorkSpace::requestShutDown();
}

#include "powermanagementjob.moc"
