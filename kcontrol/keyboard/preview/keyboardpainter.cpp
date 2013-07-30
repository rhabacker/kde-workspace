/*
 *  Copyright (C) 2012 Shivam Makkar (amourphious1992@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "keyboardpainter.h"
#ifdef NEW_GEOMETRY
#include "geometry_components.h"
#endif

#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include <KLocale>


KeyboardPainter::KeyboardPainter():
    kbDialog(new QDialog(this)),
    kbframe(new KbPreviewFrame(this)),
    exitButton(new QPushButton(i18n("Close"),this))
{
    kbDialog->setFixedSize( 1100, 490);
    kbframe->setFixedSize( 1100, 490 );
    exitButton->setFixedSize(120, 30);

    QVBoxLayout* vLayout = new QVBoxLayout( this );
    vLayout->addWidget(kbframe);
    vLayout->addWidget(exitButton);

    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));

    setWindowTitle(kbframe->getLayoutName());
}


void KeyboardPainter::generateKeyboardLayout(const QString& layout, const QString& variant, const QString& model)
{
    kbframe->generateKeyboardLayout(layout, variant, model);
#ifdef NEW_GEOMETRY
    kbDialog->setFixedSize((2.5 * kbframe->getWidth() + 20), (2.5 *  kbframe->getHeight() + 50));
    kbframe->setFixedSize((2.5 * kbframe->getWidth() + 20), (2.5 * kbframe->getHeight() ));
#endif
}


KeyboardPainter::~KeyboardPainter()
{
    delete kbframe;
    delete exitButton;
}
