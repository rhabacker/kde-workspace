/***************************************************************************
 *   Copyright (C) 2011 Aaron Seigo                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "showActivityManager.h"

#include <QGraphicsLinearLayout>
#include <QDBusConnection>
#include <QDBusMessage>

#include <Plasma/IconWidget>

ShowActivityManager::ShowActivityManager(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);

    m_icon = new Plasma::IconWidget(this);
    m_icon->setSvg("widgets/activities");
    connect(m_icon, SIGNAL(clicked()), this, SLOT(showManager()));

    layout->addItem(m_icon);
}

void ShowActivityManager::showManager()
{
    QDBusMessage message = QDBusMessage::createMethodCall("org.kde.plasma-desktop",
                                                          "/App",
                                                          QString(),
                                                          "toggleActivityManager");
    QDBusConnection::sessionBus().call(message, QDBus::NoBlock);
}

K_EXPORT_PLASMA_APPLET(org.kde.showActivityManager, ShowActivityManager)

#include "showActivityManager.moc"

