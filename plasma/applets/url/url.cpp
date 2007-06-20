/***************************************************************************
 *   Copyright (C) 2007 by Aaron Seigo <aseigo@kde.org                     *
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

#include "url.h"

#include <QGraphicsSceneDragDropEvent>

#include <KDebug>
#include <KLocale>
#include <KIconLoader>
#include <KMimeType>
#include <KPropertiesDialog>
#include <KRun>
#include <KSharedConfig>
#include <KUrl>

#include <plasma/widgets/icon.h>

Url::Url(QObject *parent, const QStringList &args)
    : Plasma::Applet(parent, args),
      m_dialog(0)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setAcceptDrops(true);

    m_icon = new Plasma::Icon(this);
    connect(m_icon, SIGNAL(clicked()), this, SLOT(openUrl()));

    KConfigGroup globalConfig = globalAppletConfig();
    int size = Plasma::Icon::boundsForIconSize(IconSize(K3Icon::Desktop));
    size = globalAppletConfig().readEntry("IconSize", size);

    KConfigGroup cg = appletConfig();
    size = cg.readEntry("IconSize", size);
    m_url = cg.readEntry("Url");

    m_icon->setSize(size, size);
    setUrl(m_url);
}

Url::~Url()
{
    KConfigGroup cg = appletConfig();
    cg.writeEntry("IconSize", m_icon->iconSize());
}

/*
#include <QPainter>
void Url::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->fillRect(boundingRect(), Qt::white);
}
*/

QRectF Url::boundingRect() const
{
//    kDebug() << "boundingRect is " << m_icon->boundingRect() << endl;
    return m_icon->boundingRect();
}

void Url::constraintsUpdated()
{
}

void Url::setUrl(const KUrl& url)
{
    m_url = url;
    KMimeType::Ptr mime = KMimeType::findByUrl(m_url);
    m_mimetype = mime->name();
    m_icon->setIcon(KMimeType::iconNameForUrl(url));
}

void Url::openUrl()
{
    if (m_url.isValid()) {
        KRun::runUrl(m_url, m_mimetype, 0);
    }
}

void Url::propertiesDialog()
{
    if (m_dialog == 0) {
        m_dialog = new KPropertiesDialog(m_url);
        connect(m_dialog, SIGNAL(applied()), this, SLOT(acceptedPropertiesDialog()));
    }

    m_dialog->show();
}

void Url::acceptedPropertiesDialog()
{
    KConfigGroup cg = appletConfig();
    cg.writeEntry("Url", m_dialog->kurl());
    setUrl(m_dialog->kurl());
    update();
    delete m_dialog;
    m_dialog = 0;
}

void Url::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (KUrl::List::canDecode(event->mimeData())) {
        KUrl::List urls = KUrl::List::fromMimeData(event->mimeData());

        if (urls.count() > 0) {
            event->accept();
            setUrl(urls.first());
        }
    }
}

#include "url.moc"

