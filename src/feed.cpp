/*
 * SPDX-FileCopyrightText: 2020 Tobias Fella <tobias.fella@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include <QVariant>

#include "database.h"
#include "debug.h"
#include "feed.h"
#include "fetcher.h"

#include <KLocalizedString>

Feed::Feed(int index)
    : QObject(nullptr)
{
    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT * FROM Feeds LIMIT 1 OFFSET :index;"));
    query.bindValue(QStringLiteral(":index"), index);
    Database::instance().execute(query);
    if (!query.next()) {
        qCWarning(ALLIGATOR) << "Failed to load feed" << index;
        return;
    }

    QSqlQuery authorQuery;
    QStringList authors;
    authorQuery.prepare(QStringLiteral("SELECT * FROM Authors WHERE id='' AND feed=:feed"));
    authorQuery.bindValue(QStringLiteral(":feed"), query.value(QStringLiteral("url")).toString());
    Database::instance().execute(authorQuery);
    while (authorQuery.next()) {
        authors += authorQuery.value(QStringLiteral("name")).toString();
    }

    if (authors.size() > 0) {
        m_authors = authors[0];
    }

    m_subscribed.setSecsSinceEpoch(query.value(QStringLiteral("subscribed")).toInt());

    m_lastUpdated.setSecsSinceEpoch(query.value(QStringLiteral("lastUpdated")).toInt());

    m_url = query.value(QStringLiteral("url")).toString();
    m_name = query.value(QStringLiteral("name")).toString();
    m_display_name = query.value(QStringLiteral("displayName")).toString();
    m_image = query.value(QStringLiteral("image")).toString();
    m_link = query.value(QStringLiteral("link")).toString();
    m_description = query.value(QStringLiteral("description")).toString();
    m_group_name = query.value(QStringLiteral("groupName")).toString();
    m_deleteAfterCount = query.value(QStringLiteral("deleteAfterCount")).toInt();
    m_deleteAfterType = query.value(QStringLiteral("deleteAfterType")).toInt();
    m_notify = query.value(QStringLiteral("notify")).toBool();
    m_errorId = 0;
    m_errorString = QLatin1String("");

    connect(&Fetcher::instance(), &Fetcher::startedFetchingFeed, this, [this](const QString &url) {
        if (url == m_url) {
            setRefreshing(true);
        }
    });
    connect(&Fetcher::instance(), &Fetcher::feedUpdated, this, [this](const QString &url) {
        if (url == m_url) {
            setRefreshing(false);
            Q_EMIT entryCountChanged();
            Q_EMIT unreadEntryCountChanged();
            setErrorId(0);
            setErrorString(QLatin1String(""));
        }
    });
    connect(&Fetcher::instance(), &Fetcher::error, this, [this](const QString &url, int errorId, const QString &errorString) {
        if (url == m_url) {
            setErrorId(errorId);
            setErrorString(errorString);
            setRefreshing(false);
        }
    });
    connect(&Fetcher::instance(), &Fetcher::imageDownloadFinished, this, [this](const QString &url) {
        if (url == m_image) {
            Q_EMIT imageChanged(url);
        }
    });
    connect(&Database::instance(), &Database::entryReadChanged, this, &Feed::unreadEntryCountChanged);
    connect(&Database::instance(), &Database::feedReadChanged, this, &Feed::unreadEntryCountChanged);
}

QString Feed::url() const
{
    return m_url;
}

QString Feed::name() const
{
    return m_name;
}

QString Feed::displayName() const
{
    return m_display_name;
}

QString Feed::image() const
{
    return m_image;
}

QString Feed::link() const
{
    return m_link;
}

QString Feed::description() const
{
    return m_description;
}

QString Feed::groupName() const
{
    return m_group_name;
}

QString Feed::authors() const
{
    return m_authors;
}

int Feed::deleteAfterCount() const
{
    return m_deleteAfterCount;
}

int Feed::deleteAfterType() const
{
    return m_deleteAfterType;
}

QDateTime Feed::subscribed() const
{
    return m_subscribed;
}

QDateTime Feed::lastUpdated() const
{
    return m_lastUpdated;
}

bool Feed::notify() const
{
    return m_notify;
}

int Feed::entryCount() const
{
    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT COUNT (id) FROM Entries where feed=:feed;"));
    query.bindValue(QStringLiteral(":feed"), m_url);
    Database::instance().execute(query);
    if (!query.next()) {
        return {};
    }
    return query.value(0).toInt();
}

int Feed::unreadEntryCount() const
{
    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT COUNT (id) FROM Entries where feed=:feed AND read=0;"));
    query.bindValue(QStringLiteral(":feed"), m_url);
    Database::instance().execute(query);
    if (!query.next()) {
        return {};
    }
    return query.value(0).toInt();
}

bool Feed::refreshing() const
{
    return m_refreshing;
}

int Feed::errorId() const
{
    return m_errorId;
}

QString Feed::errorString() const
{
    return m_errorString;
}

void Feed::setName(const QString &name)
{
    m_name = name;
    Q_EMIT nameChanged(m_name);
}

void Feed::setDisplayName(const QString &displayName)
{
    if (m_display_name != displayName) {
        m_display_name = displayName;

        Q_EMIT displayNameChanged(m_display_name);
    }
}

void Feed::setImage(const QString &image)
{
    m_image = image;
    Q_EMIT imageChanged(m_image);
}

void Feed::setLink(const QString &link)
{
    m_link = link;
    Q_EMIT linkChanged(m_link);
}

void Feed::setDescription(const QString &description)
{
    m_description = description;
    Q_EMIT descriptionChanged(m_description);
}

void Feed::setGroupName(const QString &groupName)
{
    if (m_group_name != groupName) {
        m_group_name = groupName;

        Q_EMIT groupNameChanged(groupName);
    }
}

void Feed::setAuthors(const QString &authors)
{
    m_authors = authors;
    Q_EMIT authorsChanged(m_authors);
}

void Feed::setDeleteAfterCount(int count)
{
    m_deleteAfterCount = count;
    Q_EMIT deleteAfterCountChanged(m_deleteAfterCount);
}

void Feed::setDeleteAfterType(int type)
{
    m_deleteAfterType = type;
    Q_EMIT deleteAfterTypeChanged(m_deleteAfterType);
}

void Feed::setLastUpdated(const QDateTime &lastUpdated)
{
    m_lastUpdated = lastUpdated;
    Q_EMIT lastUpdatedChanged(m_lastUpdated);
}

void Feed::setNotify(bool notify)
{
    m_notify = notify;
    Q_EMIT notifyChanged(m_notify);
}

void Feed::setRefreshing(bool refreshing)
{
    m_refreshing = refreshing;
    Q_EMIT refreshingChanged(m_refreshing);
}

void Feed::setErrorId(int errorId)
{
    m_errorId = errorId;
    Q_EMIT errorIdChanged(m_errorId);
}

void Feed::setErrorString(const QString &errorString)
{
    m_errorString = errorString;
    Q_EMIT errorStringChanged(m_errorString);
}

void Feed::refresh()
{
    Fetcher::instance().fetch(m_url);
}

void Feed::remove()
{
    // Delete Authors
    QSqlQuery query;
    query.prepare(QStringLiteral("DELETE FROM Authors WHERE feed=:feed;"));
    query.bindValue(QStringLiteral(":feed"), m_url);
    Database::instance().execute(query);

    // Delete Entries
    query.prepare(QStringLiteral("DELETE FROM Entries WHERE feed=:feed;"));
    query.bindValue(QStringLiteral(":feed"), m_url);
    Database::instance().execute(query);

    // TODO Delete Enclosures

    // Delete Feed
    query.prepare(QStringLiteral("DELETE FROM Feeds WHERE url=:url;"));
    query.bindValue(QStringLiteral(":url"), m_url);
    Database::instance().execute(query);
}
