/* This file is part of the KDE project
   Copyright ( C ) 2003 Nadeem Hasan <nhasan@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (  at your option ) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/


#ifndef SENSORLOGGERDLG_H
#define SENSORLOGGERDLG_H

#include <kdialogbase.h>

#include <qstring.h>
#include <qcolor.h>

class SensorLoggerDlgWidget;

class SensorLoggerDlg : public KDialogBase
{
  Q_OBJECT

  public:

    SensorLoggerDlg( QWidget *parent=0, const char *name=0 );

    QString fileName();
    int timerInterval();
    bool lowerLimitActive();
    bool upperLimitActive();
    double lowerLimit();
    double upperLimit();

    void setFileName( const QString & );
    void setTimerInterval( int );
    void setLowerLimitActive( bool );
    void setUpperLimitActive( bool );
    void setLowerLimit( double );
    void setUpperLimit( double );

  private:

    SensorLoggerDlgWidget *m_loggerWidget;
};

#endif // SENSORLOGGERDLG_H

/* vim: et sw=2 ts=2
*/

