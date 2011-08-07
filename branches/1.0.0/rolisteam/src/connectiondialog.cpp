/***************************************************************************
 *     Copyright (C) 2010 by Joseph Boudou                                 *
 *     http://www.rolisteam.org/                                           *
 *                                                                         *
 *   rolisteam is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "connectiondialog.h"

#include <QGroupBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QProgressBar>

#include "colorbutton.h"


/**************************
 * ConnectionConfigDialog *
 **************************/

ConnectionConfigDialog::ConnectionConfigDialog()
    : QDialog()
{
    setUI();
}

ConnectionConfigDialog::ConnectionConfigDialog(
        const QString & name, const QColor & color, bool master,
        const QString & host, quint16 port, bool server)
    : QDialog()
{
    setUI();
    m_name->setText(name);
    m_color->setColor(color);
    m_gm->setCheckState(master ? Qt::Checked : Qt::Unchecked);
    m_host->setText(host);
    m_port->setValue(port);
    m_server->setCheckState(server ? Qt::Checked : Qt::Unchecked);
}

ConnectionConfigDialog::~ConnectionConfigDialog()
{
    // QObject should remove all for us.
}

QString ConnectionConfigDialog::getName() const
{
    return m_name->text();
}

QColor ConnectionConfigDialog::getColor() const
{
    return m_color->color();
}

bool ConnectionConfigDialog::isGM() const
{
    return (m_gm->checkState() == Qt::Checked);
}

QString ConnectionConfigDialog::getHost() const
{
    return m_host->text();
}

quint16 ConnectionConfigDialog::getPort() const
{
    return m_port->value();
}

bool ConnectionConfigDialog::isServer() const
{
    return (m_server->checkState() == Qt::Checked);
}

void ConnectionConfigDialog::changeConnectionType(int state)
{
    switch (state)
    {
        case Qt::Unchecked:
            m_hostLabel->setEnabled(true);
            m_host->setEnabled(true);
            break;
        case Qt::Checked:
            m_hostLabel->setEnabled(false);
            m_host->setEnabled(false);
            break;
    }
}


void ConnectionConfigDialog::setUI()
{

    setWindowIcon(QIcon(":/logo.svg"));
    m_name  = new QLineEdit;
    m_color = new ColorButton;
    m_gm    = new QCheckBox(tr("Demander à être le MJ."));
    m_gm->setCheckState(Qt::Unchecked);

    QFormLayout * form = new QFormLayout;
    form->addRow(tr("Nom : "), m_name);
    form->addRow(tr("Couleur : "), m_color);
    form->addRow(m_gm);

    QGroupBox * playerGroup = new QGroupBox(tr("Joueur"));
    playerGroup->setLayout(form);

    m_hostLabel = new QLabel(tr("Adresse : "));
    m_host      = new QLineEdit;

    m_port      = new QSpinBox;
    m_port->setMinimum(1024);
    m_port->setMaximum(32767);

    m_server    = new QCheckBox(tr("Héberger la partie."));
    m_server->setCheckState(Qt::Unchecked);
    connect(m_server, SIGNAL(stateChanged(int)), this, SLOT(changeConnectionType(int)));

    form = new QFormLayout;
    form->addRow(m_hostLabel, m_host);
    form->addRow(tr("Port : "), m_port);
    form->addRow(m_server);

    QGroupBox * connectionGroup = new QGroupBox(tr("Connexion"));
    connectionGroup->setLayout(form);

    QDialogButtonBox * buttonBox = new QDialogButtonBox;
    buttonBox->addButton(tr("Connexion"), QDialogButtonBox::AcceptRole)->setDefault(true);
    buttonBox->addButton(tr("Quitter"),   QDialogButtonBox::RejectRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(playerGroup);
    mainLayout->addWidget(connectionGroup);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
    setSizeGripEnabled(true);
}

/************************
 * ConnectionWaitDialog *
 ************************/

// QTextCodec not initialized now. We have to wait before using tr().
const char * ConnectionWaitDialog::s_message[4] =
    {
        "Non connecté",
        "Résolution de l'adresse",
        "Connexion à l'hôte",        
        "Connecté"
    };

ConnectionWaitDialog::ConnectionWaitDialog()
    : QDialog(), m_socket(NULL)
{
    setUI();
}

ConnectionWaitDialog::~ConnectionWaitDialog()
{
    // Qt should delete m_label
    delete m_socket;
}

QString ConnectionWaitDialog::getError() const
{
    return m_error;
}

QTcpSocket * ConnectionWaitDialog::connectTo(const QString & host, quint16 port)
{
    m_error = QString();

    if (m_socket == NULL)
    {
        m_socket = new QTcpSocket();
        connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(changeState(QAbstractSocket::SocketState)));
        connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(socketError(QAbstractSocket::SocketError)));
    }

    m_socket->connectToHost(host, port);
    if (exec() == QDialog::Accepted)
    {
        QTcpSocket * tmp_socket = m_socket;
        m_socket->disconnect(this);
        m_socket = NULL;
        return tmp_socket;
    }
    else
    {
        return NULL;
    }
}

void ConnectionWaitDialog::setUI()
{
    m_label = new QLabel(tr(s_message[0]));

    QProgressBar * progress = new QProgressBar();
    progress->setMinimum(0);
    progress->setMaximum(0);
    progress->setTextVisible(false);

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Abort);
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(canceledConnection()));

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_label);
    mainLayout->addWidget(progress);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void ConnectionWaitDialog::changeState(QAbstractSocket::SocketState socketState)
{
    m_label->setText(tr(s_message[socketState]));
    if (socketState == QAbstractSocket::ConnectedState)
    {
        accept();
    }
}

void ConnectionWaitDialog::socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    m_error = m_socket->errorString();
    reject();
}

void ConnectionWaitDialog::canceledConnection()
{
    if (m_socket != NULL)
        m_socket->abort();
}