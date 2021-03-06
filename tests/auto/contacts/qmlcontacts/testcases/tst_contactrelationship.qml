/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtPim module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import QtTest 1.0
import QtContacts 5.0

TestCase {

    name: "ContactRelationship"

    Contact {
        id: contact1
        Name {
            id: contact1Name
            firstName: "contact1"
        }
    }

    Contact {
        id: contact2
        Name {
            id: contact2Name
            firstName: "contact2"
        }
    }

    ContactModel {
        id: model
        manager: "memory"
        autoUpdate: true
    }

    function initTestCase() {
        compare(model.contacts.length, 0)
        model.saveContact(contact1)
        model.saveContact(contact2)
        tryCompare(model.contacts, 'length', 2)
    }

    function setAndVerifyRelationship(first, second, relationship) {
        relationship.first = first.contactId
        relationship.second = second.contactId

        compare(relationship.first, first.contactId)
        compare(relationship.second, second.contactId)
    }

    Relationship {
        id: relationshipHasMember
        type: Relationship.HasMember
    }

    Relationship {
        id: relationshipHasMemberTypeAsString
        type: "HasMember"
    }

    function test_hasMember() {
        compare(relationshipHasMember.type, "HasMember")
        compare(relationshipHasMemberTypeAsString.type, "HasMember")

        setAndVerifyRelationship(contact1, contact2, relationshipHasMember)
        setAndVerifyRelationship(contact1, contact2, relationshipHasMemberTypeAsString)
    }

    Relationship {
        id: relationshipAggregates
        type: Relationship.Aggregates
    }

    Relationship {
        id: relationshipAggregatesTypeAsString
        type: "Aggregates"
    }

    function test_aggregates() {
        compare(relationshipAggregates.type, "Aggregates")
        compare(relationshipAggregatesTypeAsString.type, "Aggregates")

        setAndVerifyRelationship(contact1, contact2, relationshipAggregates)
    }

    Relationship {
        id: relationshipIsSameAs
        type: Relationship.IsSameAs
    }

    Relationship {
        id: relationshipIsSameAsTypeAsString
        type: "IsSameAs"
    }

    function test_isSameAs() {
        compare(relationshipIsSameAs.type, "IsSameAs")
        compare(relationshipIsSameAsTypeAsString.type, "IsSameAs")

        setAndVerifyRelationship(contact1, contact2, relationshipIsSameAs)
    }

    Relationship {
        id: relationshipHasAssistant
        type: Relationship.HasAssistant
    }

    Relationship {
        id: relationshipHasAssistantTypeAsString
        type: "HasAssistant"
    }

    function test_hasAssistant() {
        compare(relationshipHasAssistant.type, "HasAssistant")
        compare(relationshipHasAssistantTypeAsString.type, "HasAssistant")

        setAndVerifyRelationship(contact1, contact2, relationshipHasAssistant)
    }

    Relationship {
        id: relationshipHasManager
        type: Relationship.HasManager
    }

    Relationship {
        id: relationshipHasManagerTypeAsString
        type: "HasManager"
    }

    function test_hasManager() {
        compare(relationshipHasManager.type, "HasManager")
        compare(relationshipHasManagerTypeAsString.type, "HasManager")

        setAndVerifyRelationship(contact1, contact2, relationshipHasManager)
    }

    Relationship {
        id: relationshipHasSpouse
        type: Relationship.HasSpouse
    }

    Relationship {
        id: relationshipHasSpouseTypeAsString
        type: "HasSpouse"
    }

    function test_hasSpouse() {
        compare(relationshipHasSpouse.type, "HasSpouse")
        compare(relationshipHasSpouseTypeAsString.type, "HasSpouse")

        setAndVerifyRelationship(contact1, contact2, relationshipHasSpouse)
    }

    Relationship {
        id: relationshipForNullContacts
        type: "HasSpouse"
    }

    function test_nullParticipants() {
        //just check we do not crash in these cases
        relationshipForNullContacts.first = ''
        relationshipForNullContacts.second = ''
        try {
            //next two lines will throw an exception, so we catch it)
            relationshipForNullContacts.first = undefined
            relationshipForNullContacts.second = undefined
        } catch(err) {
        }
    }
}
