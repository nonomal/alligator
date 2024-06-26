/*
 * SPDX-FileCopyrightText: 2020 Tobias Fella <tobias.fella@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

#include <QDateTime>
#include <QObject>
#include <qqmlregistration.h>

class Feed : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString url READ url CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QString link READ link WRITE setLink NOTIFY linkChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString groupName READ groupName WRITE setGroupName NOTIFY groupNameChanged)
    Q_PROPERTY(QString authors READ authors WRITE setAuthors NOTIFY authorsChanged)
    Q_PROPERTY(bool refreshing READ refreshing WRITE setRefreshing NOTIFY refreshingChanged)
    Q_PROPERTY(int deleteAfterCount READ deleteAfterCount WRITE setDeleteAfterCount NOTIFY deleteAfterCountChanged)
    Q_PROPERTY(int deleteAfterType READ deleteAfterType WRITE setDeleteAfterType NOTIFY deleteAfterTypeChanged)
    Q_PROPERTY(QDateTime subscribed READ subscribed CONSTANT)
    Q_PROPERTY(QDateTime lastUpdated READ lastUpdated WRITE setLastUpdated NOTIFY lastUpdatedChanged)
    Q_PROPERTY(bool notify READ notify WRITE setNotify NOTIFY notifyChanged)
    Q_PROPERTY(int entryCount READ entryCount NOTIFY entryCountChanged)
    Q_PROPERTY(int unreadEntryCount READ unreadEntryCount NOTIFY unreadEntryCountChanged)
    Q_PROPERTY(int errorId READ errorId WRITE setErrorId NOTIFY errorIdChanged)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged)

public:
    explicit Feed(int index);

    QString url() const;
    QString name() const;
    QString displayName() const;
    QString image() const;
    QString link() const;
    QString description() const;
    QString groupName() const;
    QString authors() const;
    int deleteAfterCount() const;
    int deleteAfterType() const;
    QDateTime subscribed() const;
    QDateTime lastUpdated() const;
    bool notify() const;
    int entryCount() const;
    int unreadEntryCount() const;
    bool read() const;
    int errorId() const;
    QString errorString() const;

    bool refreshing() const;

    void setName(const QString &name);
    void setDisplayName(const QString &displayName);
    void setImage(const QString &image);
    void setLink(const QString &link);
    void setDescription(const QString &description);
    void setGroupName(const QString &groupName);
    void setAuthors(const QString &authors);
    void setDeleteAfterCount(int count);
    void setDeleteAfterType(int type);
    void setLastUpdated(const QDateTime &lastUpdated);
    void setNotify(bool notify);
    void setRefreshing(bool refreshing);
    void setErrorId(int errorId);
    void setErrorString(const QString &errorString);

    Q_INVOKABLE void refresh();
    void remove();

Q_SIGNALS:
    void nameChanged(const QString &name);
    void displayNameChanged(const QString &displayName);
    void imageChanged(const QString &image);
    void linkChanged(const QString &link);
    void descriptionChanged(const QString &description);
    void groupNameChanged(const QString &groupName);
    void authorsChanged(const QString &authors);
    void deleteAfterCountChanged(int count);
    void deleteAfterTypeChanged(int type);
    void lastUpdatedChanged(const QDateTime &lastUpdated);
    void notifyChanged(bool notify);
    void entryCountChanged();
    void unreadEntryCountChanged();
    void errorIdChanged(int errorId);
    void errorStringChanged(const QString &errorString);

    void refreshingChanged(bool refreshing);

private:
    QString m_url;
    QString m_name;
    QString m_display_name;
    QString m_image;
    QString m_link;
    QString m_description;
    QString m_group_name;
    QString m_authors;
    int m_deleteAfterCount;
    int m_deleteAfterType;
    QDateTime m_subscribed;
    QDateTime m_lastUpdated;
    bool m_notify;
    int m_errorId;
    QString m_errorString;

    bool m_refreshing = false;
};
