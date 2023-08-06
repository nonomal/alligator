/**
 * SPDX-FileCopyrightText: 2020 Tobias Fella <tobias.fella@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#pragma once

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QStringList>

#include "feed.h"

class Entry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString content READ content CONSTANT)
    Q_PROPERTY(QString authors READ authors CONSTANT)
    Q_PROPERTY(QDateTime created READ created CONSTANT)
    Q_PROPERTY(QDateTime updated READ updated CONSTANT)
    Q_PROPERTY(QString link READ link CONSTANT)
    Q_PROPERTY(QString baseUrl READ baseUrl CONSTANT)
    Q_PROPERTY(bool read READ read WRITE setRead NOTIFY readChanged)

public:
    Entry(Feed *feed, int index);
    ~Entry();

    QString id() const;
    QString title() const;
    QString content() const;
    QString authors() const;
    QDateTime created() const;
    QDateTime updated() const;
    QString link() const;
    bool read() const;

    Q_INVOKABLE void openLink(const QString &link);

    QString baseUrl() const;

    void setRead(bool read);

    Q_INVOKABLE QString adjustedContent(int width, int fontSize);

Q_SIGNALS:
    void readChanged(bool read);

private:
    Feed *m_feed;
    QString m_id;
    QString m_title;
    QString m_content;
    QString m_authors;
    QDateTime m_created;
    QDateTime m_updated;
    QString m_link;
    bool m_read;
};
