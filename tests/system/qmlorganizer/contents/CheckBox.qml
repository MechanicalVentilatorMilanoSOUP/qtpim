/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0

        Rectangle {
            id: checkBox;
            property bool checked: false
            property string checkMark: "\u2717"

            signal clicked( bool checked )

            Component.onCompleted: {
                if (!checked)
                    text.text = ""
                else
                    text.text = checkMark
            }

            width: text.height + 5
            height: text.height + 5
            color: "lightgrey"
            opacity: 0.95
            border.width: 3
            border.color: "white"
            radius: 5
            smooth: true

            Text {
                id: text
                anchors.verticalCenter: checkBox.verticalCenter
                anchors.horizontalCenter: checkBox.horizontalCenter
                color: "black"
            }

            MouseArea {
                id: mouseArea

                anchors.fill: parent
                onClicked: {
                    checked = !checked
                    checkBox.clicked( checked )
                }
            }

            states: [
            State {
                id: stateChecked

                name: "checked"; when: checked

                PropertyChanges {
                    id: propertyChangeCheckMark

                    target: text
                    text: checkMark
                }
                PropertyChanges { target: checkBox; border.color: "white"; color: "white"}
                }
            ]
        }