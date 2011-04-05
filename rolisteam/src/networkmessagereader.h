/*************************************************************************
 *     Copyright (C) 2011 by Joseph Boudou                               *
 *     http://www.rolisteam.org/                                         *
 *                                                                       *
 *   rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/


#ifndef DATA_READER_H
#define DATA_READER_H

#include <QString>
#include <QColor>

#include "networkmessage.h"

class NetworkMessageReader : public NetworkMessage
{
    public:
        NetworkMessageReader(const NetworkMessageHeader & header, const char * buffer);
        NetworkMessageReader(const NetworkMessageReader & other);
        ~NetworkMessageReader();

        NetMsg::Category category() const;
        NetMsg::Action action() const;

        void reset();

        size_t left() const;

        quint8  uint8();
        quint16 uint16();
        quint32 uint32();

        QString string8();
        QString string16();
        QString string32();

        QString string(int size);

        QRgb rgb();

    protected:
        NetworkMessageHeader * buffer();

    private:
        NetworkMessageHeader * m_header;
        char * m_buffer;
        const char * m_pos;
        const char * m_end;
};

#endif
