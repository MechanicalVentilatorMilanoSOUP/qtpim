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

#ifndef QDECLARATIVECONTACTTYPE_H
#define QDECLARATIVECONTACTTYPE_H

#include "qdeclarativecontactdetail_p.h"
#include "qcontacttype.h"

QTCONTACTS_BEGIN_NAMESPACE

class QDeclarativeContactType : public QDeclarativeContactDetail
{
    Q_OBJECT
    Q_PROPERTY(ContactType type READ type WRITE setType NOTIFY valueChanged)
    Q_ENUMS(FieldType)
    Q_ENUMS(ContactType)
    Q_CLASSINFO("DefaultProperty", "type")
public:
    enum FieldType {
        TypeField = 0
    };

    enum ContactType {
        Contact = 0,
        Group
    };


    ContactDetailType detailType() const
    {
        return QDeclarativeContactDetail::Type;
    }
    static QString fieldNameFromFieldType(int fieldType)
    {
        switch (fieldType) {
        case TypeField:
            return QContactType::FieldType;
        default:
            break;
        }
        return "";
    }
    QDeclarativeContactType(QObject* parent = 0)
        :QDeclarativeContactDetail(parent)
    {
        setDetail(QContactType());
        connect(this, SIGNAL(valueChanged()), SIGNAL(detailChanged()));
    }

    void setType(ContactType newType)
    {
         if (!readOnly())  {
            if (newType == Contact)
                detail().setValue(QContactType::FieldType, QContactType::TypeContact);
            else if (newType == Group)
                detail().setValue(QContactType::FieldType, QContactType::TypeGroup);
         }
    }
    ContactType type() const
    {
        QString typeString = detail().value(QContactType::FieldType);
        if (typeString == QContactType::TypeContact)
            return Contact;
        return Group;
    }
signals:
    void valueChanged();
};

QTCONTACTS_END_NAMESPACE

QML_DECLARE_TYPE(QTCONTACTS_PREPEND_NAMESPACE(QDeclarativeContactType))

#endif

