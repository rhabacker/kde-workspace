/*****************************************************************

Copyright 2008 Christian Mollekopf <robertknight@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

// Ownm_preferredInsertIndex
#include "abstractgroupableitem.h"
#include "taskgroup.h"
#include "taskmanager.h"


// KDE
#include <KDebug>



namespace TaskManager
{


class AbstractGroupableItem::Private
{
public:
    Private()
        :m_parentGroup(0)
    {
    }

    GroupPtr m_parentGroup;
};


AbstractGroupableItem::AbstractGroupableItem(QObject *parent)
:   QObject(parent),
    d(new Private)
{

}


AbstractGroupableItem::~AbstractGroupableItem()
{
    kDebug();
    emit destroyed(this);
    if (parentGroup()){
        kDebug() << "Error: item gets destroyed but still has a parent group";
    }
    delete d;
}


bool AbstractGroupableItem::grouped() const
{
    if (parentGroup()){
        if (parentGroup()->parentGroup()) { // not only member of rootGroup
            return true;
        }
    }
    return false;
}

GroupPtr AbstractGroupableItem::parentGroup() const
{
    //kDebug();
    return d->m_parentGroup;
}

void AbstractGroupableItem::addedToGroup(const GroupPtr group)
{
    kDebug() << "Added to group " ;
    if (!group) {
        kDebug() << "Null Group Pointer";
        return;
    }
    
    d->m_parentGroup = group;
}


void AbstractGroupableItem::removedFromGroup()
{
    kDebug() << "Remove Parent " ;
    d->m_parentGroup = 0;
}


//Item is member of group
bool AbstractGroupableItem::isGroupMember(const GroupPtr group)
{
    kDebug();
    if (!group) {
        kDebug() << "Null Group Pointer";
        return false;
    }
    if (!parentGroup()) {
        return false;
    }

    if (group->members().contains(this)) {
        return true;
    }
    return false;
}


} // TaskManager namespace

#include "abstractgroupableitem.moc"

