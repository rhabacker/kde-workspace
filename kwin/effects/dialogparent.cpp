/*****************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2006 Rivo Laks <rivolaks@hot.ee>

You can Freely distribute this program under the GNU General Public
License. See the file "COPYING" for the exact licensing terms.
******************************************************************/


#include "dialogparent.h"

#include <client.h>

// Note that currently effects need to be manually enabled in the EffectsHandler
// class constructor (in effects.cpp).

namespace KWinInternal
{

void DialogParentEffect::prePaintWindow( Scene::Window* w, int* mask, QRegion* region, int time )
    {
    // How long does it take for the effect to get it's full strength (in ms)
    const float changeTime = 200;

    // Check if this window has a modal dialog and change the window's
    //  effect's strength accordingly
    bool hasDialog = hasModalWindow(w->window());
    if( hasDialog )
        {
        // Increase effect strength of this window
        effectStrength[w] = qMin(1.0f, effectStrength[w] + time/changeTime);
        }
    else
        {
        effectStrength[w] = qMax(0.0f, effectStrength[w] - time/changeTime);
        }

    // Call the next effect
    effects->prePaintWindow( w, mask, region, time );
    }

void DialogParentEffect::paintWindow( Scene::Window* w, int mask, QRegion region, WindowPaintData& data )
    {
    float s = effectStrength[w];
    if(s > 0.0f)
        {
        // Brightness will be within [1.0; 0.6]
        data.brightness *= (1.0 - s * 0.4);
        // Saturation within [1.0; 0.4]
        data.saturation *= (1.0 - s * 0.6);
        }

    // Call the next effect.
    effects->paintWindow( w, mask, region, data );
    }

void DialogParentEffect::postPaintWindow( Scene::Window* w )
    {
    float s = effectStrength[w];

    // If strength is between 0 and 1, the effect is still in progress and the
    //  window has to be repainted during the next pass
    if( s > 0.0 && s < 1.0 )
        w->window()->addDamageFull(); // trigger next animation repaint

    // Call the next effect.
    effects->postPaintWindow( w );
    }

void DialogParentEffect::windowActivated( Toplevel* t )
    {
    // If this window is a dialog, we need to damage it's parent window, so
    //  that the effect could be run for it
    // Set the window to be faded (or NULL if no window is active).
    Client* c = qobject_cast<Client *>(t);
    if ( c && c->isModal() )
        {
        // c is a modal dialog
        Client* parent = c->transientFor();
        if ( parent )
            parent->addDamageFull();
        }
    }

void DialogParentEffect::windowDeleted( Toplevel* t )
    {
    // If this window is a dialog, we need to damage it's parent window, so
    //  that the effect could be run for it
    // Set the window to be faded (or NULL if no window is active).
    Client* c = qobject_cast<Client *>(t);
    if ( c && c->isModal() )
        {
        // c is a modal dialog
        Client* parent = c->transientFor();
        if ( parent )
            parent->addDamageFull();
        }
    }

bool DialogParentEffect::hasModalWindow( Toplevel* t )
    {
    Client* c = qobject_cast<Client *>(t);
    if( !c )
        return false;

    // Check if any of the direct transients (children) of this window is modal
    for( ClientList::ConstIterator it = c->transients().begin(); it != c->transients().end(); ++it )
        if( (*it)->isModal() && (*it)->transientFor() == c )
            return true;

    return false;
    }


} // namespace
