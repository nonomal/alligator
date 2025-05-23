/*
* SPDX-FileCopyrightText: 2021 Dimitris Kardarakos <dimkard@posteo.net>
*
* SPDX-License-Identifier: GPL-3.0-or-later
*/

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.kde.alligator

Kirigami.Dialog {
    id: root
    title: i18n("Edit Feed")

    property Feed feed
    onFeedChanged: groupCombo.currentIndex = (root.feed !== null) ? groupCombo.indexOfValue(root.feed.groupName) : groupCombo.indexOfValue("")

    standardButtons: Kirigami.Dialog.Ok | Kirigami.Dialog.Cancel
    padding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing + Kirigami.Units.smallSpacing
    preferredWidth: Kirigami.Units.gridUnit * 20

    onAccepted: {
        Database.editFeed(feed.url, displayName.text, groupCombo.currentValue);
    }

    Kirigami.FormLayout {
        Controls.Label {
            Layout.fillWidth: true
            text: root.feed.url
            Kirigami.FormData.label: i18n("Feed URL:")
        }

        Controls.TextField {
            id: displayName
            Layout.fillWidth: true

            text: (root.feed !== null) ? (root.feed.displayName || root.feed.name) : ""
            Kirigami.FormData.label: i18n("Display Name:")
        }

        Controls.ComboBox {
            id: groupCombo
            Layout.fillWidth: true

            model: FeedGroupsModel {}
            textRole: "name"
            valueRole: "name"
            Kirigami.FormData.label: i18n("Group:")
        }
    }
}
