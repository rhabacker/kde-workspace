/*  
    Copyright 2007 Alexis Ménard <darktears31@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

// Qt
#include <QtGui/QAbstractItemDelegate>

class ItemDelegate : public QAbstractItemDelegate
{
public:
    
    ItemDelegate();
    
    virtual void paint(QPainter *painter,const QStyleOptionViewItem& option,const QModelIndex& index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem& option , const QModelIndex& index) const;

    // margin is equivalent to ItemDelegate::BACK_ARROW_WIDTH + ItemDelegate::BACK_ARROW_SPACING
    static const int HEADER_LEFT_MARGIN = 5;
    static const int HEADER_TOP_MARGIN = 10;
    static const int HEADER_BOTTOM_MARGIN = 10;
    static const int HEADER_HEIGHT = 35;
    static const int FIRST_HEADER_HEIGHT = 20;

    static const int ICON_TEXT_MARGIN = 10;
    static const int ICON_SIZE = 32;

    static const int ITEM_LEFT_MARGIN = 5;
    static const int ITEM_RIGHT_MARGIN = 7;
    static const int ITEM_HEIGHT = 32;
    static const int TOP_OFFSET = 5;

    static const int BACK_ARROW_WIDTH = 20;
    static const int BACK_ARROW_SPACING = 5;

private:
    QPainterPath roundedRectangle(const QRectF& rect,qreal radius) const;
    QFont fontForSubTitle(const QFont& titleFont) const;
};

#endif // ITEMDELEGATE_H 
