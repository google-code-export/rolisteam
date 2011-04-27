/*************************************************************************
 *   Copyright (C) 2007 by Romain Campioni                               *
 *   Copyright (C) 2009 by Renaud Guezennec                              *
 *   Copyright (C) 2011 by Joseph Boudou                                 *
 *                                                                       *
 *   http://www.rolisteam.org/                                           *
 *                                                                       *
 *   Rolisteam is free software; you can redistribute it and/or modify   *
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


#include "chatwindow.h"

#include <QDateTime>
#include <QScrollBar>
#include <QTextStream>

#include "chat.h"
#include "networkmessagewriter.h"
#include "initialisation.h"
#include "MainWindow.h"
#include "persons.h"
#include "playersList.h"
#include "TextEditAmeliore.h"

#include "types.h"
#include "variablesGlobales.h"

/******************
 * Static members *
 ******************/

QStringList ChatWindow::m_keyWordList;

/********************************************************************/
/* Constructeur                                                     */
/********************************************************************/    
ChatWindow::ChatWindow(AbstractChat * chat, MainWindow * parent)
 : QSplitter(parent), m_chat(chat), m_filename("%1/%2.html")
{
    if (m_chat == NULL)
        qFatal("ChatWindow with NULL chat");

    if (parent == NULL)
        parent = G_mainWindow;

    // Initialisation des variables
    m_warnedEmoteUnavailable = false;
    m_hasUnseenMessage = false;

    // static members
    if (m_keyWordList.size() == 0)
        m_keyWordList << "e " << "em " << "me " << "emote ";

    // create and connect toggleViewAction
    m_toggleViewAction = new QAction(this);
    m_toggleViewAction->setCheckable(true);
    connect(m_toggleViewAction, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));

    // Mise a 0 de l'historique des messages
    historiqueMessages.clear();
    numHistorique = 0;

    // Les 2 parties du tchat seront positionnees verticalement dans la fenetre
    setOrientation(Qt::Vertical);
    // Les widgets contenus ne peuvent pas avoir une taille de 0
    setChildrenCollapsible(false);

    // Creation de la zone affichant le texte des utilisateurs tiers
    zoneAffichage = new QTextEdit();
    zoneAffichage->setReadOnly(true);
    zoneAffichage->setMinimumHeight(30);
    zoneAffichage->setFocusPolicy(Qt::NoFocus);
    
    // Creation de la zone editable contenant les messages de l'utilisateur local
    zoneEdition = new TextEditAmeliore();
    zoneEdition->setReadOnly(false);
    zoneEdition->setMinimumHeight(30);
    zoneEdition->setAcceptRichText(false);
    zoneEdition->installEventFilter(this);

    // Ajout des 2 zones dans le splitter
    addWidget(zoneAffichage);
    addWidget(zoneEdition);

    // Initialisation des tailles des 2 widgets
    QList<int> tailles;
    tailles.append(200);
    tailles.append(40);
    setSizes(tailles);

    // Connexion du signal de validation du text de la zone d'edition a la demande d'emission
    connect(zoneEdition, SIGNAL(entreePressee()), this, SLOT(emettreTexte()));
    // Connexion des signaux haut et bas au defilement des anciens messages
    connect(zoneEdition, SIGNAL(hautPressee()), this, SLOT(monterHistorique()));
    connect(zoneEdition, SIGNAL(basPressee()), this, SLOT(descendreHistorique()));

    connect(parent, SIGNAL(closing()), this, SLOT(save()));

    // Window initialisation
    setObjectName("ChatWindow");
    setWindowIcon(QIcon(":/icones/vignette tchat.png"));
    setAttribute(Qt::WA_DeleteOnClose, false);
    parent->registerSubWindow(this);
    hide();

    updateTitleFromChat();
    connect(m_chat, SIGNAL(changedName()), this, SLOT(updateTitleFromChat()));
}

/********************************************************************/    
/* Destructeur                                                      */
/********************************************************************/    
ChatWindow::~ChatWindow()
{
    delete m_toggleViewAction;

    if (G_client || !m_chat->inherits("PrivateChat"))
        delete m_chat;
}

AbstractChat * ChatWindow::chat() const
{
    return m_chat;
}

/********************************************************************/    
/* La zone d'edition est recopiee dans la zone d'affichage, puis    */
/* envoyee aux autres utilisateurs, avant d'etre effacee            */
/********************************************************************/    
void ChatWindow::emettreTexte()
{
    //NetMsg::ChatMessageAction, NetMsg::DiceMessageAction, NetMsg::EmoteMessageAction
    NetMsg::Action action = NetMsg::DiceMessageAction;

    // On recupere le texte de la zone d'edition
    QString message = zoneEdition->toPlainText();
    
    // Si le message est issu de l'historique, on supprime le dernier element de la liste
    if (numHistorique < historiqueMessages.size())
        historiqueMessages.removeLast();
    
    // On ajoute le message a l'historique
    historiqueMessages.append(message);
    numHistorique = historiqueMessages.size();

    bool ok=true;
    QString tirage;
    int result;
    zoneEdition->clear();
    QTextStream out(stderr,QIODevice::WriteOnly);

    PlayersList & g_playersList = PlayersList::instance();
    Player * localPlayer = g_playersList.localPlayer();

    QString tmpmessage=message.simplified();
    QString messageCorps="";
    QString messageTitle="";
    QColor color;

    switch(tmpmessage[0].toAscii())
    {
        case '!':
            result = calculerJetDes(message, tirage, ok);
            if(ok)
            {
                           messageCorps = tr("avez obtenu %1 à votre jet de dés [%2]").arg(result).arg(tirage);
                        messageTitle = tr("Vous");
                        color = localPlayer->color();
                        afficherMessage(messageTitle, color, messageCorps,NetMsg::DiceMessageAction);
                            message = QString(tr("a obtenu %1 à  son jet de dés [%2]").arg(result).arg(tirage));
            }
            else
            {
                messageCorps = tr("!1d6 ou !5d10+3 ou !2d20-3d10+1d6+5 etc... Le jet de dés est public (utilisez & pour un jet privé).");
                messageTitle = tr("Syntaxe");
                color = Qt::red;
                afficherMessage(messageTitle, color, messageCorps);
            }
            break;
        case '&':
            result = calculerJetDes(message, tirage, ok);
            if (ok)
            {
                messageCorps = tr("vous avez obtenu %1 à votre jet de dés secret [%2]").arg(result).arg(tirage);
                messageTitle = tr("Jet secret :");
                color = Qt::magenta;
                afficherMessage(messageTitle, color,messageCorps ,NetMsg::DiceMessageAction);
            }
            else
            {
                messageCorps = tr("!1d6 ou !5d10+3 ou !2d20-3d10+1d6+5 etc... Le jet de dés est public (utilisez & pour un jet privé).");
                messageTitle = tr("Syntaxe");
                color = Qt::red;
                afficherMessage(messageTitle, color, messageCorps);
            }
            return;
            break;
        case '*':
            {
                QRegExp exp("\\*[0-9]+[dD].*");
                if(exp.exactMatch(message))
                {
                    QString glitch;
                    result = calculerJetDesSR4(message, tirage, glitch, ok);
                    if (ok)
                    {
                        messageCorps = tr("avez obtenu %1 succès %2%3").arg(result).arg(glitch).arg(tirage);
                        messageTitle = tr("Vous");
                        // On affiche le resultat du tirage dans la zone d'affichage
                        afficherMessage(messageTitle, localPlayer->color(),messageCorps ,NetMsg::DiceMessageAction);
                        // On cree un nouveau message a envoyer aux autres utilisateurs
                        message = QString(tr("a obtenu %1 succès %2%3").arg(result).arg(glitch).arg(tirage));
                    }
                    else
                    {
                        messageCorps = tr("*12D ... ajoutez R pour rusher, G3 pour les Gremlins d'indice 3 et + pour relancer les 6 ... ajouter C pour ne pas afficher les détails du lancer, et S pour n'afficher que les résultats.");
                        messageTitle = tr("Syntaxe SR4");
                         color = Qt::red;
                        afficherMessage(messageTitle, color, messageCorps);
                    }
                    break;
                }
                else
                {
                        messageTitle = tr("Vous");
                        afficherMessage(messageTitle, localPlayer->color(), message);
                        // action is messageChatWindow only if there are no dices
                        action = NetMsg::ChatMessageAction;
                        break;
                }
            }

        case '/':
            {
                bool emote = false;

                for(int i = 0;((i < m_keyWordList.size())&&(!emote)); i++)
                {
                    QString s = m_keyWordList[i];

                    if(tmpmessage.indexOf(s) == 1)
                    {
                        emote = true;

                        message.remove(0,s.length()+1);
                    }

                }
                if(emote)
                {
                    // Warn if some users don't have Emote(0) feature
                    if (!m_warnedEmoteUnavailable && !m_chat->everyPlayerHasFeature(QString("Emote")))
                    {
                        messageTitle = tr("Attention");
                        messageCorps = tr("Certains utilisateurs risquent de ne pas voir vos emotes.");
                        color = Qt::red;
                        afficherMessage(messageTitle, color, messageCorps);
                        m_warnedEmoteUnavailable = true;
                    }
                        
                    
                    afficherMessage(localPlayer->name(), localPlayer->color(), message,NetMsg::EmoteMessageAction);
                    action = NetMsg::EmoteMessageAction;
                    break;
                }
            }

        default:
            messageTitle = localPlayer->name();
            afficherMessage(messageTitle, localPlayer->color(), message);
            // action is messageChatWindow only if there are no dices
            action = NetMsg::ChatMessageAction;
            break;
    }

    if(!ok)
        return;

    // Emission du message
    NetworkMessageWriter data(NetMsg::ChatCategory, action);
    data.string8(localPlayer->uuid());
    data.string8(m_chat->identifier());
    data.string32(message);
    m_chat->sendThem(data);
}


QAction * ChatWindow::toggleViewAction() const
{
    return m_toggleViewAction;
}

/********************************************************************/    
/* Ecrit le message dans la zone d'affichage, precede par le nom de */
/* l'emetteur (avec la couleur passee en parametre)                 */
/********************************************************************/    
void ChatWindow::afficherMessage(const QString& utilisateur, const QColor& couleur, const QString& message, NetMsg::Action msgtype)
{
    // On repositionne le curseur a la fin du QTexEdit
    zoneAffichage->moveCursor(QTextCursor::End);
    // Affichage du nom de l'utilisateur
    zoneAffichage->setTextColor(couleur);
    switch(msgtype)
    {
    case NetMsg::ChatMessageAction:
        zoneAffichage->setFontItalic(false);
        zoneAffichage->append(QString("%1 : ").arg(utilisateur));
        break;
    case NetMsg::EmoteMessageAction:
        zoneAffichage->setFontItalic(true);
        zoneAffichage->append(QString("%1 ").arg(utilisateur));
        break;
    case NetMsg::DiceMessageAction:
        zoneAffichage->setFontItalic(false);
        zoneAffichage->append(QString("%1 ").arg(utilisateur));
        break;
    default:
        qWarning("Can't type unknown message type %d", msgtype);
        return;
    }

    if (!zoneEdition->hasFocus() && !m_hasUnseenMessage)
    {
        m_hasUnseenMessage = true;
        emit changed(this);
    }

    // Affichage du message
    zoneAffichage->setTextColor(Qt::black);

    zoneAffichage->insertPlainText(message);
    if(msgtype==NetMsg::EmoteMessageAction)
        zoneAffichage->setFontItalic(false);
    // On repositionne la barre de defilement, pour pouvoir voir le texte qui vient d'etre affiche
    zoneAffichage->verticalScrollBar()->setSliderPosition(zoneAffichage->verticalScrollBar()->maximum());
}

/********************************************************************/
/* Cache la fenetre au lieu de la detruire et demande le decochage  */
/* de la case de l'utilisateur correspondant dans la liste des      */
/* utilisateurs                                                     */
/********************************************************************/    
void ChatWindow::closeEvent(QCloseEvent *event)
{
    // Decoche l'action associee dans le sous-menu ChatWindows
    m_toggleViewAction->setChecked(false);
    QWidget::closeEvent(event);
}

int ChatWindow::calculerJetDes(QString &message, QString &tirage, bool &ok)
{
    //1 by default to add, -1 to substract.
    short signOperator=1;
    int result = 0;
    int nombre=0; ///  @attention : Initialize value Otherwise something wrong could happen
    int faces=0;
    int garde=0;
    bool relance10;

    // Mise a 0 de la chaine tirage
    tirage.clear();
    // Suppression des espaces dans le message
    message.remove(QChar(' '));
    // Suppresion du "!" en tete du message
    message.remove(0, 1);
    // Si la taille est nulle, on quitte avec une erreur
    if (!message.size())
        return (ok = false);

    while(message.size())
    {
    
        if(GetNumber(message,&nombre))///  @attention : former fashion did not take care of the return value.
        {
            // 4 cas de figure :
            // le nombre est suivi par un "D" : on recupere le nombre qui suit (nbr de faces du de)
            if (message[0] == QChar('d') || message[0] == QChar('D'))
            {
                // On supprime le "D"
                message.remove(0, 1);
                // Si la taille est nulle, on quitte avec une erreur
                if (!message.size())
                    return (ok = false);
                ok = GetNumber(message,&faces);
                // S'il y a un nombre de des ou de faces nul, on quitte
                if (!nombre || !faces)
                    return (ok = false);
                // The dices rolling
                QList<int> listDices;
                unsigned short dice, sumDice=0;
                for(unsigned short u=0;u<nombre;u++)
                {
                    dice=(qrand()%faces)+1;
                    listDices.append(dice);
                    sumDice+=dice;
                }
                // Formatting the "tirage" text
                tirage.append(tr("%1d%2 (%3").arg(nombre).arg(faces).arg(listDices[0]));
                for(unsigned short u=1;u<listDices.size();u++)
                {
                    tirage.append(QString(","));
                    tirage.append(QString::number(listDices[u]));
                }
                tirage.append(QString(")"));
                result+=signOperator*sumDice;
            } // Fin de la lecture et du tirage des des
            // le nombre est suivi par un "G" : on recupere le nombre qui suit (nbr de des gardés)
            else if (message[0] == QChar('g') || message[0] == QChar('G'))
            {
                relance10=(message[0] == QChar('G'));
                // On supprime le "G"
                message.remove(0, 1);
                // Si la taille est nulle, on quitte avec une erreur
                if (!message.size())
                    return (ok = false);
                ok = GetNumber(message,&garde);
                // S'il y a un nombre de des ou de faces nul, on quitte
                if (!nombre || !garde)
                    return (ok = false);
                if(garde > nombre)
                    garde = nombre;
                // The dices rolling
                QList<int> listDices;
                unsigned short tmpDice, dice, sumDice=0;
                for(unsigned short u=0;u<nombre;u++)
                {
                    tmpDice=(qrand()%10)+1;
                    dice=tmpDice;
                    if(relance10)
                        while((tmpDice==10))
                        {
                            tmpDice=(qrand()%10)+1;
                            dice+=tmpDice;
                        }
                    listDices.append(dice);
                    //sumDice+=dice;
                }
                qSort(listDices.begin(), listDices.end(), qGreater<unsigned short>());
                for(int it=0;it<garde;++it)
                {
                    sumDice+=listDices[it];
                }
                // Formatting the "tirage" text
                tirage.append(tr("%1g%2 (%3").arg(nombre).arg(garde).arg(listDices[0]));
                for(unsigned short u=1;u<listDices.size();u++)
                {
                    tirage.append(QString(","));
                    tirage.append(QString::number(listDices[u]));
                }
                tirage.append(QString(")"));
                result+=signOperator*sumDice;
            } // Fin de la lecture et du tirage des des
            // le nombre est suivi par un "s" : on recupere pas le nombre qui suit (il n'y en a pas)
            else if (message[0] == QChar('s') || message[0] == QChar('S'))
            {
                // On supprime le "S"
                message.remove(0, 1);
                // S'il y a un nombre de des ou de faces nul, on quitte
                if (!nombre)
                    return (ok = false);
                // The dices rolling
                QList<int> listDices;
                unsigned short dice, sumDice=0;
                for(unsigned short u=0;u<nombre;u++)
                {
                    dice=(qrand()%6)+1;
                    listDices.append(dice);
                    if(dice>=5)
                       ++sumDice ;
                }
                // Formatting the "tirage" text
                tirage.append(tr("%1s (%3").arg(nombre).arg(listDices[0]));
                for(unsigned short u=1;u<listDices.size();u++)
                {
                    tirage.append(QString(","));
                    tirage.append(QString::number(listDices[u]));
                }
                tirage.append(QString(")"));
                result+=signOperator*sumDice;
            } // Fin de la lecture et du tirage des des
            // Le caractere suivant n'est pas un "D" : on ajoute ou on soustrait le nombre
            else
            {
                result+=signOperator*nombre;
                tirage+=QString("%1").arg(nombre);
            }
            // On regarde si le caractere suivant est un signe + ou -
            if (message.size())
            {
                if (message[0] == QChar('+'))
                    signOperator = 1;
                else if (message[0] == QChar('-'))
                    signOperator = -1;
                else
                    return (ok = false);/// @attention
                tirage+=' ';
                tirage+=message[0];
                tirage+=' ';
                // On supprime l'operateur
                message.remove(0, 1);
                // Le message ne doit pas etre vide apres
                if (message.size() == 0)
                    return (ok = false);
            }
        }
        else
        {
            ok=false;
            return 0;
        }
    }

    ok = true;
    return result;
}

// Ultyme
/********************************************************************/    
/* Analyse le message et calcule le jet de des selon les regles de  */
/* SR4 si la syntaxe est correcte (ok == true). Une Qstring est ge- */
/* neree pour expliquer le tirage, un autre pour le glitch.         */
/* Les Gremlins et la regle du 6 again (usage de l'edge) sont geres.*/
/********************************************************************/    
int ChatWindow::calculerJetDesSR4(QString &message, QString &tirage, QString &glitch, bool ok)
{
    //Initialisation du nombre de succes, de glitches, et des parametres du lancer
    int nbDes;
    int nbDesEnCours;
    int nbDesEnsuite;
    QString nbDesStr;
    int nbSucces = 0;
    int nbGlitch = 0;
    bool sixAgainActif = false;
    bool gremlinsActif = false;
    bool rushActif = false;
    bool modeCourtActif = false;
    bool modeSecretActif = false;
    int indiceGremlins = 0;
    QString indiceGremlinsStr;
    int nbFaces = 6;
    QString nbFacesStr = "6";
    int seuilSucces = 5;
    int seuilGlitch = 1;
    int nbPasses = 0;
    int de;
    bool estOK;

    tirage.clear();
    glitch.clear();

    message.remove(QChar(' '));
    message.remove(0, 1);

    if (message.size() == 0)
        return (ok=false);

    ok=GetNumber(message,&nbDes);

    if (message.size() == 0 || (message[0] != QChar('d') && message[0] != QChar('D')))
        return (ok=false);
    message.remove(0, 1);

    while (message.size()>0)
    {
        QChar firstChar=message[0];
        //if the first character of message is a letter, put it to lowercase
        if(firstChar.isLetter())
            firstChar.toCaseFolded ();
            
        switch(firstChar.toAscii())
        {
            case '+':
                if (sixAgainActif)
                    return (ok=false);

                sixAgainActif = true;
                break;
            case 'g':
                if (gremlinsActif)
                    return (ok=false);

                gremlinsActif = true;
                message.remove(0, 1);

                // the function need to have a message and that message[0] is a digit to continue
                if (!(message.size() && message[0].isDigit()))
                    return (ok=false);
                indiceGremlinsStr = message.left(1);
                indiceGremlins = indiceGremlinsStr.toUInt(&estOK, 10);
                break;
            case 'r':
                if (rushActif)
                    return (ok=false);

                rushActif = true;
                seuilGlitch = 2;
                break;
            case 'c':
                if (modeCourtActif)
                    return (ok=false);

                modeCourtActif = true;
                break;
            case 's':
                if (modeSecretActif)
                    return (ok=false);

                modeSecretActif = true;
                break;
            default:
                return (ok=false);
        }
        message.remove(0, 1);
    }



    if (nbDes == 0)
        return (ok=false);

    if (!modeSecretActif)
    {
                tirage.append(QString(tr(" (%1 dés").arg(nbDes)));
        if (rushActif)
            tirage.append(QString(tr(" en rushant")));
        if (gremlinsActif)
            tirage.append(QString(tr(" avec Gremlins %1").arg(indiceGremlinsStr)));
        if (sixAgainActif)
                        tirage.append(QString(tr(" - les 6 sont relancés")));
        if (!modeCourtActif)
            tirage.append(QString(tr(" : ")));
        else
            tirage.append(QString(tr(")")));
    }

    nbDesEnsuite = nbDes;
    while (nbDesEnsuite != 0)
    {
        nbDesEnCours = nbDesEnsuite;
        nbDesEnsuite = 0;
        nbPasses++;

        for (int u=0; u<nbDesEnCours ; u++)
        {
            // Tirage du de
            de = 1 + (int)((double)rand() / ((double)RAND_MAX + 1) * nbFaces); //rand()%nbFaces + 1;
            // Verification du succes
            if (de >= seuilSucces)
            {
                nbSucces++;
                //Si la regle des 6 again est activee, augmentation de la prochaine serie
                if (sixAgainActif && de == 6)
                    nbDesEnsuite++;
            }
            // Verification du glitch (uniquement sur la premiere serie de lancer)
            // this is strange... nbPasses '=' 1 ??? not '==' ?
            if ((nbPasses = 1) && (de <= seuilGlitch))
                nbGlitch++;
            // Ajout du resultat a la chaine Tirage si pas en Mode court ou Mode Secret
            if (!modeCourtActif && !modeSecretActif)
            {
                tirage.append(QString::number(de));
                // Ajout d'un espace, d'un "puis" entre deux series ou d'une parenthese pour finir
                if (u < nbDesEnCours-1)
                    tirage.append(QString(" "));
                else if (nbDesEnsuite > 0)
                    tirage.append(QString(" puis "));
                else
                    tirage.append(QString(")"));
            }
        }
    }

    if (nbGlitch>0 && nbGlitch>((nbDes/2)-indiceGremlins))
    {
        if (nbSucces == 0)
            glitch.append(QString(" et une complication critique !! "));
        else
            glitch.append(QString(" et une complication !! "));
    }


    ok = true;
    return nbSucces;
}
// FIN Ultyme

/********************************************************************/    
/* Affiche le message precedent                                     */
/********************************************************************/    
void ChatWindow::monterHistorique()
{
    // Ne s'applique que si la liste n'est pas vide
    if (historiqueMessages.isEmpty())
        return;
    
    // C'est la 1ere fois que l'utilisateur appuie sur la touche : on memorise la ligne actuelle
    if (numHistorique == historiqueMessages.size())
        historiqueMessages.append(zoneEdition->toPlainText());

    // Si on a atteint le 1er message on arrete de remonter
    if (numHistorique)
        numHistorique--;
        
    // Affichage de l'ancien message dans la zone d'edition
    zoneEdition->clear();
    zoneEdition->append(historiqueMessages[numHistorique]);
}
    
/********************************************************************/    
/* Affiche le message suivant                                       */
/********************************************************************/    
void ChatWindow::descendreHistorique()
{
    // Uniquement si on a deja appuye sur fleche haut
    if (numHistorique < historiqueMessages.size() - 1)
    {
        numHistorique++;
        // Affichage du nouveau message dans la zone d'edition
        zoneEdition->clear();
        zoneEdition->append(historiqueMessages[numHistorique]);
    }
}

bool ChatWindow::hasUnseenMessage() const
{
    return m_hasUnseenMessage;
}

void ChatWindow::setVisible(bool visible)
{
    emit changed(this);
    QWidget::setVisible(visible);
}

void ChatWindow::save()
{
    QString filename = m_filename.arg(G_initialisation.dossierTchats, m_chat->name());
    QFile file (filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning("Unable to open %s", qPrintable(filename));
        return;
    }

    QTextStream stream (&file);
    stream << zoneAffichage->document()->toHtml(QByteArray("utf-8"));

    file.close();
}

void ChatWindow::updateTitleFromChat()
{
    const QString & name = m_chat->name();

    setWindowTitle(tr("%1 (Tchat)").arg(name));
    m_toggleViewAction->setText(name);
}

/********************************************************************/
/* Lorsque la fenetre s'ouvre, on positionne le curseur sur la zone */
/* d'edition                                                        */
/********************************************************************/
void ChatWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    // On place le curseur sur la zone d'edition
    zoneEdition->setFocus(Qt::OtherFocusReason);
}

bool ChatWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == zoneEdition)
        if (event->type() == QEvent::FocusIn)
            if (m_hasUnseenMessage)
            {
                m_hasUnseenMessage = false;
                emit changed(this);
            }

    return false;
}

bool ChatWindow::GetNumber(QString &str, int* value)
{
    bool error;
    int i=0;
    while(str[i].isDigit() && i<str.size())
        i++;
    // Length of 0 (No digit): exit with error
    if(!i)
        return false;

    *value = str.left(i).toUInt(&error, 10);
    str.remove(0, i);
    return error;
}
