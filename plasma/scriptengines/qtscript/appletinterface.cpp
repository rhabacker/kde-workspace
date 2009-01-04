/*
 *   Copyright 2008 Chani Armitage <chani@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
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

#include "appletinterface.h"

#include <QAction>
#include <QSignalMapper>

#include <KDE/KIcon>

#include <Plasma/Plasma>
#include <Plasma/Applet>
#include <Plasma/Context>
#include <Plasma/DataEngine>

#include "qscript.h"

AppletInterface::AppletInterface(QScriptApplet *parent)
    : QObject(parent),
      m_appletScriptEngine(parent),
      m_actionSignals(0)
{
    connect(this, SIGNAL(releaseVisualFocus()), m_appletScriptEngine->applet(), SIGNAL(releaseVisualFocus()));
    connect(this, SIGNAL(configNeedsSaving()), m_appletScriptEngine->applet(), SIGNAL(configNeedsSaving()));
}

AppletInterface::~AppletInterface()
{
}

KConfigGroup AppletInterface::config()
{
    return m_appletScriptEngine->applet()->config();
}

Plasma::DataEngine* AppletInterface::dataEngine(const QString &name)
{
    return m_appletScriptEngine->applet()->dataEngine(name);
}

AppletInterface::FormFactor AppletInterface::formFactor()
{
    return static_cast<FormFactor>(m_appletScriptEngine->applet()->formFactor());
}

AppletInterface::Location AppletInterface::location()
{
    return static_cast<Location>(m_appletScriptEngine->applet()->location());
}

QString AppletInterface::currentActivity()
{
    return m_appletScriptEngine->applet()->context()->currentActivity();
}

AppletInterface::AspectRatioMode AppletInterface::aspectRatioMode()
{
    return static_cast<AspectRatioMode>(m_appletScriptEngine->applet()->aspectRatioMode());
}

void AppletInterface::setAspectRatioMode(AppletInterface::AspectRatioMode mode)
{
    applet()->setAspectRatioMode(static_cast<Plasma::AspectRatioMode>(mode));
}

bool AppletInterface::shouldConserveResources()
{
    return m_appletScriptEngine->applet()->shouldConserveResources();
}

void AppletInterface::setFailedToLaunch(bool failed, const QString &reason)
{
    m_appletScriptEngine->setFailedToLaunch(failed, reason);
}

bool AppletInterface::isBusy()
{
    return m_appletScriptEngine->applet()->isBusy();
}

void AppletInterface::setBusy(bool busy)
{
    applet()->setBusy(busy);
}

void AppletInterface::setConfigurationRequired(bool needsConfiguring, const QString &reason)
{
    m_appletScriptEngine->setConfigurationRequired(needsConfiguring, reason);
}

void AppletInterface::setHasConfigurationInterface(bool hasConfigInterface)
{
    m_appletScriptEngine->setHasConfigurationInterface(hasConfigInterface);
}

void AppletInterface::update()
{
    applet()->update();
}

void AppletInterface::setLayout(QGraphicsLayout *layout)
{
    applet()->setLayout(layout);
}

QGraphicsLayout *AppletInterface::layout() const
{
    return m_appletScriptEngine->applet()->layout();
}

const Plasma::Package *AppletInterface::package() const
{
    return m_appletScriptEngine->package();
}

QList<QAction*> AppletInterface::contextualActions() const
{
    QList<QAction*> actions;
    Plasma::Applet *a = applet();

    foreach (const QString name, m_actions) {
        QAction *action = a->action(name);

        if (action) {
            actions << action;
        }
    }

    return actions;
}

QSizeF AppletInterface::size() const
{
    return m_appletScriptEngine->applet()->size();
}

void AppletInterface::setAction(const QString &name, const QString &text, const QString &icon, const QString &shortcut)
{
    Plasma::Applet *a = applet();
    QAction *action = a->action(name);

    if (action) {
        action->setText(text);
    } else {
        action = new QAction(text, this);
        a->addAction(name, action);

        Q_ASSERT(!m_actions.contains(name));
        m_actions.insert(name);

        if (!m_actionSignals) {
            m_actionSignals = new QSignalMapper(this);
            connect(m_actionSignals, SIGNAL(mapped(QString)),
                    m_appletScriptEngine, SLOT(executeAction(QString)));
        }

        connect(action, SIGNAL(triggered()), m_actionSignals, SLOT(map()));
        m_actionSignals->setMapping(action, name);
    }

    action->setIcon(icon.isEmpty() ? QIcon() : KIcon(icon));
    action->setShortcut(shortcut);
    action->setObjectName(name);
}

void AppletInterface::removeAction(const QString &name)
{
    Plasma::Applet *a = applet();
    QAction *action = a->action(name);

    if (action) {
        if (m_actionSignals) {
            m_actionSignals->removeMappings(action);
        }

        delete action;
    }

    m_actions.remove(name);
}

void AppletInterface::resize(qreal w, qreal h)
{
    applet()->resize(w,h);
}

void AppletInterface::setMinimumSize(qreal w, qreal h)
{
    applet()->setMinimumSize(w,h);
}

void AppletInterface::setPreferredSize(qreal w, qreal h)
{
    applet()->setPreferredSize(w,h);
}

void AppletInterface::dataUpdated(QString source, Plasma::DataEngine::Data data)
{
    m_appletScriptEngine->dataUpdated(source, data);
}

Plasma::Applet *AppletInterface::applet() const
{
    return m_appletScriptEngine->applet();
}

#include "appletinterface.moc"
