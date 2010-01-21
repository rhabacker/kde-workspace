/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2006 Lubos Lunak <l.lunak@kde.org>
Copyright (C) 2010 Jorge Mata <matamax123@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef KWIN_TRACKMOUSE_H
#define KWIN_TRACKMOUSE_H

#include <kwineffects.h>
#include <kwinglutils.h>

class KAction;

namespace KWin
{

class TrackMouseEffect
    : public QObject, public Effect
    {
    Q_OBJECT
    public:
        TrackMouseEffect();
        virtual ~TrackMouseEffect();
        virtual void prePaintScreen( ScreenPrePaintData& data, int time );
        virtual void paintScreen( int mask, QRegion region, ScreenPaintData& data );
        virtual void postPaintScreen();
        virtual void mouseChanged( const QPoint& pos, const QPoint& old,
            Qt::MouseButtons buttons, Qt::MouseButtons oldbuttons,
            Qt::KeyboardModifiers modifiers, Qt::KeyboardModifiers oldmodifiers );
        virtual void reconfigure( ReconfigureFlags );
    private slots:
        void toggle();
    private:
        QRect starRect( int num ) const;
        void loadTexture();
        bool active, mousePolling;
        int angle;
        GLTexture* texture;
        QSize textureSize;
        KActionCollection* actionCollection;
        KAction* action;
        Qt::KeyboardModifiers modifier;
    };

} // namespace

#endif
