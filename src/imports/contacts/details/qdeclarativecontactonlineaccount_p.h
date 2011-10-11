/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QDECLARATIVECONTACTONLINEACCOUNT_H
#define QDECLARATIVECONTACTONLINEACCOUNT_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontactonlineaccount.h"
#include <QSet>

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactOnlineAccount : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(QString accountUri READ accountUri WRITE setAccountUri NOTIFY valueChanged)
    Q_PROPERTY(QString serviceProvider READ serviceProvider WRITE  setServiceProvider NOTIFY valueChanged)
    Q_PROPERTY(QStringList capabilities READ capabilities WRITE  setCapabilities NOTIFY valueChanged)
    Q_PROPERTY(QVariant subTypes READ subTypes WRITE  setSubTypes NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(OnlineAccountSubType)

    Q_CLASSINFO("DefaultProperty", "accountUri")
public:
    enum FieldType {
        AccountUri = 0,
        ServiceProvider,
        Capabilities,
        SubTypes
    };

    enum OnlineAccountSubType {
        Unknown = 0,
        Sip,
        SipVoip,
        Impp,
        VideoShare
    };

    QDeclarativeContactOnlineAccount(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactOnlineAccount());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }

    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::OnlineAccount;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case AccountUri:
            return QContactOnlineAccount::FieldAccountUri;
        case ServiceProvider:
            return QContactOnlineAccount::FieldServiceProvider;
        case Capabilities:
            return QContactOnlineAccount::FieldCapabilities;
        case SubTypes:
            return QContactOnlineAccount::FieldSubTypes;
        default:
            break;
        }
        qmlInfo(0) << tr("Unknown field type.");
        return QString();
    }
    void setAccountUri(const QString& v)
    {
        if (!readOnly() && v != accountUri()) {
            detail().setValue(QContactOnlineAccount::FieldAccountUri, v);
            emit valueChanged();
        }
    }
    QString accountUri() const {return detail().value(QContactOnlineAccount::FieldAccountUri);}

    void setServiceProvider(const QString& v)
    {
        if (!readOnly() && v != serviceProvider()) {
            detail().setValue(QContactOnlineAccount::FieldServiceProvider, v);
            emit valueChanged();
        }
    }
    QString serviceProvider() const {return detail().value(QContactOnlineAccount::FieldServiceProvider);}

    void setCapabilities(const QStringList& v)
    {
        if (!readOnly() && v.toSet() != capabilities().toSet()) {
            detail().setValue(QContactOnlineAccount::FieldCapabilities, v);
            emit valueChanged();
        }
    }
    QStringList capabilities() const {return detail().value<QStringList>(QContactOnlineAccount::FieldCapabilities);}

    void setSubTypes(const QVariant& v)
    {
        QStringList savedList;
        foreach (const QVariant subType, v.toList()) {
            switch (static_cast<OnlineAccountSubType>(subType.value<int>()))
            {
            case Sip:
                savedList << QContactOnlineAccount::SubTypeSip;
                break;
            case SipVoip:
                savedList << QContactOnlineAccount::SubTypeSipVoip;
                break;
            case Impp:
                savedList << QContactOnlineAccount::SubTypeImpp;
                break;
            case VideoShare:
                savedList << QContactOnlineAccount::SubTypeVideoShare;
                break;
            default:
                qmlInfo(this) << tr("Unknown sub type.");
                break;

            }
        }
        QStringList oldList = detail().value<QStringList>(QContactOnlineAccount::FieldSubTypes);
        if (!readOnly() && oldList.toSet() != savedList.toSet()) {
            detail().setValue(QContactOnlineAccount::FieldSubTypes, savedList);
            emit valueChanged();
        }
    }

    QVariant subTypes() const
    {
        return QVariant::fromValue(detail().value<QStringList>(QContactOnlineAccount::FieldSubTypes));
    }

signals:
    void valueChanged();
};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactOnlineAccount))

#endif

