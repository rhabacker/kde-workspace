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

#ifndef LISTVIEW_H
#define LISTVIEW_H

// Qt
#include <QAbstractItemView>

class ListView : public QAbstractItemView
{
Q_OBJECT

public:
    ListView(QWidget *parent = 0);
    virtual ~ListView();

    // reimplemented from QAbstractItemView 
    virtual QModelIndex indexAt(const QPoint& point) const;
    virtual void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible); 
    virtual QRect visualRect(const QModelIndex& index) const;
    virtual void setModel(QAbstractItemModel *model);
protected:
    // reimplemented from QAbstractItemView 
    virtual int horizontalOffset() const;
    virtual bool isIndexHidden(const QModelIndex& index) const;
    virtual QModelIndex moveCursor(CursorAction action,Qt::KeyboardModifiers modifiers);
    virtual void setSelection(const QRect& rect,QItemSelectionModel::SelectionFlags flags);
    virtual int verticalOffset() const;
    virtual QRegion visualRegionForSelection(const QItemSelection& selection) const;
    
    // reimplemented from QWidget
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private Q_SLOTS:
    // lays out all items in the view and sets the current index to the first
    // selectable item
    void updateLayout();

private:
    class Private;
    Private * const d;
};

#endif // LISTVIEW_H
