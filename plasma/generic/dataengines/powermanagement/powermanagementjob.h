/*
 *   Copyright 2011 Sebastian Kügler <sebas@kde.org>
 *   Copyright 2011 Viranch Mehta <viranch.mehta@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef POWERMANAGEMENT_JOB_H
#define POWERMANAGEMENT_JOB_H

// plasma
#include <Plasma/ServiceJob>

typedef QMap<QString, QString> StringStringMap;

class PowermanagementJob : public Plasma::ServiceJob
{

    Q_OBJECT

    public:
        PowerManagementJob(const QString &operation, QMap<QString, QVariant> &parameters,
                           QObject *parent = 0);
        ~PowerManagementJob();

    protected:
        void start();

    private:
        enum SuspendType { Ram, Disk, Hybrid };
        bool suspend(const SuspendType &type);
        void requestShutDown();
        QString callForType(const SuspendType &type);
};

Q_DECLARE_METATYPE (StringStringMap);

#endif // POWERMANAGEMENT_JOB_H
