#ifndef oxygen_helper_h
#define oxygen_helper_h

/*
 * Copyright 2009-2010 Hugo Pereira Da Costa <hugo@oxygen-icons.org>
 * Copyright 2008 Long Huynh Huu <long.upcase@googlemail.com>
 * Copyright 2007 Matthew Woehlke <mw_triad@users.sourceforge.net>
 * Copyright 2007 Casper Boemann <cbr@boemann.dk>
 * Copyright 2007 Fredrik Höglund <fredrik@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <KSharedConfig>
#include <KComponentData>
#include <KColorScheme>

#include <QtGui/QColor>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>
#include <QtGui/QLinearGradient>
#include <QtCore/QCache>

#include "oxygentileset.h"

namespace Oxygen
{

    template<typename T> class Cache
    {

        public:

        //! constructor
        Cache()
        {}

        //! destructor
        ~Cache()
        {}

        //! return cache matching a given key
        typedef QCache<quint64, T> Value;
        Value* get( const QColor& color )
        {
            quint64 key = (quint64(color.rgba()) << 32);
            Value *cache = data_.object(key);

            if (!cache)
            {
                cache = new Value( data_.maxCost() );
                data_.insert(key, cache);
            }

            return cache;
        }

        //! clear
        void clear( void )
        { data_.clear(); }

        //! max cache size
        void setMaxCacheSize( int value )
        {
            data_.setMaxCost( value );
            foreach( quint64 key, data_.keys() )
            { data_.object( key )->setMaxCost( value ); }
        }

        private:

        QCache<quint64, Value> data_;

    };

    //! oxygen style helper class.
    /*! contains utility functions used at multiple places in both oxygen style and oxygen window decoration */
    class OXYGEN_EXPORT Helper
    {
        public:

        //! constructor
        Helper(const QByteArray &componentName);

        //! destructor
        virtual ~Helper()
        {}

        //! reload configuration
        virtual void reloadConfig();

        //! pointer to shared config
        KSharedConfigPtr config() const;

        //! reset all caches
        virtual void invalidateCaches();

        //! update maximum cache size
        virtual void setMaxCacheSize( int );

        //!@name window background gradients
        //@{
        /*!
        \par y_shift: shift the background gradient upwards, to fit with the windec
        \par gradientHeight: the height of the generated gradient.
        for different heights, the gradient is translated so that it is always at the same position from the bottom
        */
        void renderWindowBackground(QPainter *p, const QRect &clipRect, const QWidget *widget, const QPalette & pal, int y_shift=-23, int gradientHeight = 64)
        { renderWindowBackground( p, clipRect, widget, pal.color( widget->window()->backgroundRole() ), y_shift, gradientHeight ); }

        /*!
        y_shift: shift the background gradient upwards, to fit with the windec
        gradientHeight: the height of the generated gradient.
        for different heights, the gradient is translated so that it is always at the same position from the bottom
        */
        void renderWindowBackground(QPainter *p, const QRect &clipRect, const QWidget *widget, const QWidget* window, const QPalette & pal, int y_shift=-23, int gradientHeight = 64)
        { renderWindowBackground( p, clipRect, widget, window, pal.color( window->backgroundRole() ), y_shift, gradientHeight ); }

        //! render window background using a given color as a reference
        void renderWindowBackground(QPainter *p, const QRect &clipRect, const QWidget *widget, const QColor& color, int y_shift=-23, int gradientHeight = 64)
        { renderWindowBackground( p, clipRect, widget, widget->window(), color, y_shift, gradientHeight ); }

        //! render window background using a given color as a reference
        void renderWindowBackground(QPainter *p, const QRect &clipRect, const QWidget *widget, const QWidget* window, const QColor& color, int y_shift=-23, int gradientHeight = 64);

        //! dots
        void renderDot(QPainter*, const QPoint&, const QColor&);

        //@}

        bool lowThreshold(const QColor &color);
        bool highThreshold(const QColor &color);

        static QColor alphaColor(QColor color, qreal alpha);

        virtual const QColor& calcLightColor(const QColor &color);
        virtual const QColor& calcDarkColor(const QColor &color);
        virtual const QColor& calcShadowColor(const QColor &color);

        //! returns menu background color matching position in a given top level widget
        virtual const QColor& backgroundColor(const QColor &color, const QWidget* w, const QPoint& point )
        {
            if( !( w && w->window() ) || checkAutoFillBackground( w ) ) return color;
            else return backgroundColor( color, w->window()->height(), w->mapTo( w->window(), point ).y() );
        }

        //! returns menu background color matching position in a top level widget of given height
        virtual const QColor& backgroundColor(const QColor &color, int height, int y)
        { return backgroundColor( color, qMin(qreal(1.0), qreal(y)/qMin(300, 3*height/4) ) ); }

        virtual const QColor& backgroundRadialColor(const QColor &color);
        virtual const QColor& backgroundTopColor(const QColor &color);
        virtual const QColor& backgroundBottomColor(const QColor &color);

        virtual QPixmap verticalGradient(const QColor &color, int height, int offset = 0 );
        virtual QPixmap radialGradient(const QColor &color, int width, int height = 64);

        //! merge background and front color for check marks, arrows, etc. using _contrast
        virtual QColor decoColor(const QColor &background, const QColor &color) const;

        //! returns a region matching given rect, with rounded corners, based on the multipliers
        /*! setting any of the multipliers to zero will result in no corners shown on the corresponding side */
        virtual QRegion roundedMask( const QRect&, int left = 1, int right = 1, int top = 1, int bottom = 1 ) const;

        //! draw frame that mimics some sort of shadows around a panel
        /*! it is used for menus, detached dock panels and toolbar, as well as window decoration when compositing is disabled */
        virtual void drawFloatFrame(
            QPainter *p, const QRect r, const QColor &color,
            bool drawUglyShadow=true, bool isActive=false,
            const QColor &frameColor=QColor(),
            TileSet::Tiles tiles = TileSet::Ring
            );

        virtual void drawSeparator(QPainter *p, const QRect &r, const QColor &color, Qt::Orientation orientation);

        virtual TileSet *slab(const QColor&, qreal shade, int size = 7);

        //! focus brush
        const KStatefulBrush& viewFocusBrush( void ) const
        { return _viewFocusBrush; }

        //! hover brush
        const KStatefulBrush& viewHoverBrush( void ) const
        { return _viewHoverBrush; }

        //! negative text brush (used for close button hover)
        const KStatefulBrush& viewNegativeTextBrush( void ) const
        { return _viewNegativeTextBrush; }

        /*!
        returns first widget in parent chain that sets autoFillBackground to true,
        or NULL if none
        */
        const QWidget* checkAutoFillBackground( const QWidget* ) const;

        protected:

        virtual void drawSlab(QPainter&, const QColor&, qreal shade);
        virtual void drawShadow(QPainter&, const QColor&, int size);
        virtual void drawOuterGlow(QPainter&, const QColor&, int size);

        //! return background adjusted color matching relative vertical position in window
        const QColor& backgroundColor( const QColor&, qreal ratio );

        static const qreal _glowBias;
        static const qreal _slabThickness;
        static const qreal _shadowGain;
        qreal _contrast;

        typedef QCache<quint64, QPixmap> PixmapCache;
        PixmapCache m_windecoButtonCache;
        PixmapCache m_windecoButtonGlowCache;
        Oxygen::Cache<TileSet> m_slabCache;

        //! shortcut to color caches
        /*! it is made protected because it is also used in the style helper */
        typedef QCache<quint64, QColor> ColorCache;

        private:

        //! brushes
        KStatefulBrush _viewFocusBrush;
        KStatefulBrush _viewHoverBrush;
        KStatefulBrush _viewNegativeTextBrush;

        KComponentData _componentData;
        KSharedConfigPtr _config;
        qreal _bgcontrast;

        //!@name color caches
        //@{
        ColorCache m_lightColorCache;
        ColorCache m_darkColorCache;
        ColorCache m_shadowColorCache;
        ColorCache m_backgroundTopColorCache;
        ColorCache m_backgroundBottomColorCache;
        ColorCache m_backgroundRadialColorCache;
        ColorCache m_backgroundColorCache;
        //@}

        PixmapCache m_backgroundCache;
        PixmapCache m_dotCache;

        //! high threshold colors
        typedef QMap<quint32, bool> ColorMap;
        ColorMap m_highThreshold;
        ColorMap m_lowThreshold;
     };

}

#endif
