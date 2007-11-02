/*
 *   Copyright 2006-2007 Aaron Seigo <aseigo@kde.org>
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

#ifndef SEARCHACTION_H
#define SEARCHACTION_H

#include <QtGui/QAction>

#include <plasma/plasma_export.h>

namespace Plasma
{

class SearchContext;
class AbstractRunner;

class PLASMA_EXPORT SearchAction : public QAction
{
    Q_OBJECT

    public:
        enum Type { InformationalMatch,
                    ExactMatch,
                    PossibleMatch };

        SearchAction(SearchContext *search, AbstractRunner *runner);
        ~SearchAction();

        /**
         * Sets the type of match this action represents.
         */
        void setType(Type type);

        /**
         * The type of action this is. Defaults to ExactMatch.
         */
        Type type() const;

        /**
         * Sets the mimetype, if any, associated with this match
         *
         * @arg mimetype the mimetype
         */
        void setMimetype(const QString &mimetype);

        /**
         * The mimetype associated with this action, if any
         */
        QString mimetype() const;

        /**
         * The search term that triggered this action
         */
        QString searchTerm() const;

        /**
         * Sets the relevance of this action for the search
         * it was created for.
         *
         * @param relevance a number between 0 and 1.
         */
        void setRelevance(qreal relevance);

        /**
         * The relevance of this action to the search. By default,
         * the relevance is 1.
         *
         * @return a number between 0 and 1
         */
        qreal relevance() const;

        /**
         * The runner associated with this action
         */
        AbstractRunner* runner() const;

        bool operator<(const SearchAction& other) const;

    protected Q_SLOTS:
        void exec();

    private:
        class Private;
        Private * const d;
};

}

#endif
