////////////////////////////////////////////////////////////////////////
// proxylayout.cpp                                                     //
//                                                                     //
// Copyright(C) 2009 Igor Trindade Oliveira <igor.oliveira@indt.org.br>//
// Copyright(C) 2009 Adenilson Cavalcanti <adenilson.silva@idnt.org.br>//
//                                                                     //
// This library is free software; you can redistribute it and/or       //
// modify it under the terms of the GNU Lesser General Public          //
// License as published by the Free Software Foundation; either        //
// version 2.1 of the License, or (at your option) any later version.  //
//                                                                     //
// This library is distributed in the hope that it will be useful,     //
// but WITHOUT ANY WARRANTY; without even the implied warranty of      //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU   //
// Lesser General Public License for more details.                     //
//                                                                     //
// You should have received a copy of the GNU Lesser General Public    //
// License along with this library; if not, write to the Free Software //
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA       //
// 02110-1301  USA                                                     //
/////////////////////////////////////////////////////////////////////////
#include "proxylayout.h"

#include <QPropertyAnimation>
#include <QGraphicsWidget>

#include <QWeakPointer>

#include <KDebug>

class ProxyLayoutPrivate
{
public:
    QWeakPointer<QGraphicsWidget> widget;
    QWeakPointer<QPropertyAnimation> animation;
};

ProxyLayout::ProxyLayout(QGraphicsWidget *widget, QGraphicsLayoutItem *parent)
    : QGraphicsLayoutItem(parent, false), d(new ProxyLayoutPrivate)
{

    setOwnedByLayout(true);

    d->widget = widget;
    setGraphicsItem(widget);

    QPropertyAnimation *animation = new QPropertyAnimation(widget, "geometry", widget);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->setDuration(1000);

    d->animation = animation;
}

ProxyLayout::~ProxyLayout()
{
    if(d->widget.data())
        d->widget.clear();
}

void ProxyLayout::setGeometry(const QRectF &rect)
{
    if(!d->animation.data()) {
        return;
    }

    QPropertyAnimation *animation = d->animation.data();

    if(animation->state() == QAbstractAnimation::Running)
        animation->stop();

    QGraphicsWidget *widget = d->widget.data();

    if(!widget) {
        QGraphicsLayoutItem::setGeometry(rect);
        return;
    }

    animation->setEndValue(rect);
    animation->start();
}

QSizeF ProxyLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_UNUSED(which);
    Q_UNUSED(constraint);

    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    QGraphicsWidget *widget = d->widget.data();
    QSizeF currentWidgetSize = widget->effectiveSizeHint(which, constraint);

    return QSizeF( left + right + currentWidgetSize.width(), right + bottom + currentWidgetSize.height());
}

QGraphicsLayoutItem *ProxyLayout::widget()
{
    return d->widget.data();
}
